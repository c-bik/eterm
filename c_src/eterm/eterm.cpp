#include "eterm.h"

using namespace std;

static void erlterm_to_stl(ETERM *, term &);
static ETERM * stl_to_erlterm(term &);

eterm::eterm(void)
{
	erl_init(NULL, 0);
#ifdef __WIN32__
    transcoder_lock = CreateMutex(NULL, FALSE, NULL);
    if (NULL == transcoder_lock) {
        return;
    }
#else
    if(pthread_mutex_init(&transcoder_lock, NULL) != 0) {
        return;
    }
#endif
}

mutex_type eterm::transcoder_lock;

bool eterm::lock()
{
#ifdef __WIN32__
	return (WAIT_OBJECT_0 == WaitForSingleObject((transcoder_lock),INFINITE));
#else
	return (0 == pthread_mutex_lock(&transcoder_lock));
#endif
}

void eterm::unlock()
{
#ifdef __WIN32__
    ReleaseMutex(transcoder_lock);
#else
    pthread_mutex_unlock(&transcoder_lock);
#endif
}

term eterm::decode(void *buf)
{
	unsigned long allocated, freed;
	term t;
	if(lock()) {
		ETERM * etermp = erl_decode((unsigned char *)buf);

		erlterm_to_stl(etermp, t);

		// If allocated != 0 after freeing the term
		// we have a memory leak
		// release the mutex in either case
		erl_free_compound(etermp);
		get_stats(allocated, freed);
		if (allocated != 0) {
			unlock();
			return t;
		}
		unlock();
	}
	return t;
}

vector<unsigned char> eterm::encode(term & t)
{
	unsigned long allocated, freed;
	vector<unsigned char> buf;
	if(lock()) {
		ETERM * etermp = stl_to_erlterm(t);

		// If allocated != 0 after freeing the term
		// we have a memory leak
		// release the mutex in either case
		int len = erl_term_len(etermp);
		buf.resize(len);

		erl_encode(etermp, &buf[0]);
		erl_free_compound(etermp);
		get_stats(allocated, freed);
		if (allocated != 0) {
			unlock();
			return buf;
		}
		unlock();
	}
	return buf;
}

static void erlterm_to_stl(ETERM *et, term & t)
{
	if(0);
	else if(ERL_IS_ATOM(et))				t.set(ERL_ATOM,			"ERL_ATOM",			ERL_ATOM_PTR_UTF8(et));
	else if(ERL_IS_FLOAT(et))				t.set(ERL_FLOAT,		"ERL_FLOAT",		ERL_FLOAT_VALUE(et));
	else if(ERL_IS_PID(et))					t.set(ERL_PID,			"ERL_PID",			ERL_PID_NODE_UTF8(et),	ERL_PID_NUMBER(et),		ERL_PID_SERIAL(et),	ERL_PID_CREATION(et));
	else if(ERL_IS_PORT(et))				t.set(ERL_PORT,			"ERL_PORT",			ERL_PORT_NODE_UTF8(et),	ERL_PORT_NUMBER(et),	ERL_PORT_CREATION(et));
	else if(ERL_IS_REF(et))					t.set(ERL_REF,			"ERL_REF",			ERL_REF_NODE_UTF8(et),	ERL_REF_NUMBER(et),		ERL_REF_CREATION(et));
	else if(ERL_IS_BINARY(et))				t.set(ERL_BINARY,		"ERL_BINARY",		ERL_BIN_PTR(et), ERL_BIN_SIZE(et));
	else if(ERL_IS_INTEGER(et))				t.set(ERL_INTEGER,		"ERL_INTEGER",		ERL_INT_VALUE(et));
	else if(ERL_IS_UNSIGNED_INTEGER(et))	t.set(ERL_U_INTEGER,	"ERL_U_INTEGER",	ERL_INT_UVALUE(et));
	else if(ERL_IS_LONGLONG(et))			t.set(ERL_LONGLONG,		"ERL_LONGLONG",		ERL_LL_VALUE(et));
	else if(ERL_IS_UNSIGNED_LONGLONG(et))	t.set(ERL_U_LONGLONG,	"ERL_U_LONGLONG",	ERL_LL_UVALUE(et));
	else if(ERL_IS_EMPTY_LIST(et))			t.set(ERL_LIST,			"ERL_LIST",			NULL, 0);
	else if(ERL_IS_CONS(et) || ERL_IS_LIST(et)) {
		ETERM *erl_list = et;
		unsigned long long list_idx = 0;
		do {
			ETERM *et1 = ERL_CONS_HEAD(erl_list);
			term t1;
			erlterm_to_stl(et1, t1);
			t.set(ERL_LIST, "ERL_LIST", t1, list_idx);
			erl_list = ERL_CONS_TAIL(erl_list);
			++list_idx;
		} while(erl_list && !ERL_IS_EMPTY_LIST(erl_list));
	}
	else if(ERL_IS_TUPLE(et)) {
		for(unsigned long long tuple_idx = 0; tuple_idx < ERL_TUPLE_SIZE(et); ++tuple_idx) {
			term t1;
			erlterm_to_stl(ERL_TUPLE_ELEMENT(et, tuple_idx), t1);
			t.set(ERL_TUPLE, "ERL_TUPLE", t1, tuple_idx);
		}
	}
}

static ETERM *stl_to_erlterm(term & t)
{
	ETERM * et = NULL;
	switch (t.type_code) {
		case ERL_ATOM:
			et = erl_mk_atom(t.v.ppr.v->c_str());
			break;
		case ERL_FLOAT:
			et = erl_mk_float(t.v.d);
			break;
		case ERL_PID:
			et = erl_mk_pid(t.v.ppr.v->c_str(), t.v.ppr.n, t.v.ppr.s, t.v.ppr.c);
			break;
		case ERL_PORT:
			et = erl_mk_port(t.v.ppr.v->c_str(), t.v.ppr.n, t.v.ppr.c);
			break;
		case ERL_REF:
			et = erl_mk_ref(t.v.ppr.v->c_str(), t.v.ppr.n, t.v.ppr.c);
			break;
		case ERL_BINARY:
			et = erl_mk_binary(t.v.ppr.v->c_str(), (int)t.v.ppr.v->length());
			break;
		case ERL_INTEGER:
			et = erl_mk_int(t.v.i);
			break;
		case ERL_U_INTEGER:
			et = erl_mk_uint(t.v.ui);
			break;
		case ERL_LONGLONG:
			et = erl_mk_longlong(t.v.ll);
			break;
		case ERL_U_LONGLONG:
			et = erl_mk_ulonglong(t.v.ull);
			break;
		case ERL_LIST: {
			ETERM ** et_ms = new ETERM*[t.v.lt->size()];
			for(int i = 0; i < t.v.lt->size(); ++i)
				et_ms[i] = stl_to_erlterm(t.v.lt->at(i));
			et = erl_mk_list(et_ms, (int)t.v.lt->size());
					   }
			break;
		case ERL_TUPLE: {
			ETERM ** et_ms = new ETERM*[t.v.lt->size()];
			for(int i = 0; i < t.v.lt->size(); ++i)
				et_ms[i] = stl_to_erlterm(t.v.lt->at(i));
			et = erl_mk_tuple(et_ms, (int)t.v.lt->size());
						}
			break;
	}
	return et;
}