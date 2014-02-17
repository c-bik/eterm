#pragma once
#ifndef _TERM_H_
#define _TERM_H_

#include <regex>
#ifndef _REGEX_
#error "regex not included"
#endif
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "erl_interface.h"
#include "ei.h"

class term {
public:
	typedef enum _type {
		UNKNOWN,
		TUPLE_START,
		TUPLE_END,
		LIST_START,
		LIST_END,
		ATOM,
		INTEGER,
		LONG,
		LONGLONG,
		UINTEGER,
		ULONG,
		ULONGLONG,
		DOUBLE,
		STRING,
		BINARY,
		TERM,
		PORT,
		REF,
		PID,
		VALUE
	} TYPE;

	class  TOKEN
	{
	public:
		TYPE t;
		string val;
		TOKEN(TYPE _t) { t =_t; }
		TOKEN(TYPE _t, string v) { t =_t; val = v; }
	};

	vector<term> lt; // list or tuple
	string str;
	union {
		int	i;
		unsigned int ui;
		double d;
		long l;
		long long ll;
		unsigned long ul;
		unsigned long long ull;
		struct {
			int n;
			int s;
			int c;
		} ppr; // pid, port, ref, string or binary
	} v;
	string type;
	unsigned char type_code;

	inline term() { };
	inline ~term() { };

	inline term(int i)					{ v.i = i;		type = "ERL_INTEGER",		type_code = ERL_INTEGER;	};
	inline term(long l)					{ v.l = l;		type = "ERL_INTEGER",		type_code = ERL_INTEGER;	};
	inline term(long long ll)			{ v.ll = ll;	type = "ERL_LONGLONG",		type_code = ERL_LONGLONG;	};
	inline term(unsigned int ui)		{ v.ui = ui;	type = "ERL_U_INTEGER",		type_code = ERL_U_INTEGER;	};
	inline term(unsigned long ul)		{ v.ul = ul;	type = "ERL_U_INTEGER",		type_code = ERL_U_INTEGER;	};
	inline term(unsigned long long ull)	{ v.ull = ull;	type = "ERL_U_LONGLONG",	type_code = ERL_U_LONGLONG;	};
	inline term(float f)				{ v.d = f;		type = "ERL_FLOAT",			type_code = ERL_FLOAT;		};
	inline term(double d)				{ v.d = d;		type = "ERL_FLOAT",			type_code = ERL_FLOAT;		};

	inline term(TYPE t, string & s)
	{
		switch (t) {
			case ATOM:
				type = "ERL_ATOM";
				type_code = ERL_ATOM;
				str = s;
				break;
			case BINARY:
				type = "ERL_BINARY";
				type_code = ERL_BINARY;
				str = s;
				break;
			case STRING:
				type = "ERL_LIST";
				type_code = ERL_LIST;
				for(int idx = 0; idx < s.size(); ++idx)
					lt.push_back(term((int)s[idx]));
				break;
			default:
				break;
		};
	};

	inline term(TYPE t, string & s, int n, int c)
	{
		switch (t) {
			case PORT:
				type = "ERL_PORT";
				type_code = ERL_PORT;
				v.ppr.n = n;
				v.ppr.c = c;
				str = s;
				break;
			case REF:
				type = "ERL_REF";
				type_code = ERL_REF;
				v.ppr.n = n;
				v.ppr.c = c;
				str = s;
				break;
		}
	};

	inline term(string & _str, int n, int s, int c)
	{
		type = "ERL_PID";
		type_code = ERL_PID;
		v.ppr.n = n;
		v.ppr.s = s;
		v.ppr.c = c;
		str = _str;
	};

	inline bool is_atom()		{ return type_code == ERL_ATOM;			}
	inline bool is_float()		{ return type_code == ERL_FLOAT;		}
	inline bool is_pid()		{ return type_code == ERL_PID;			}
	inline bool is_port()		{ return type_code == ERL_PORT;			}
	inline bool is_ref()		{ return type_code == ERL_REF;			}
	inline bool is_binary()		{ return type_code == ERL_BINARY;		}
	inline bool is_integer()	{ return type_code == ERL_INTEGER;		}
	inline bool is_u_integer()	{ return type_code == ERL_U_INTEGER;	}
	inline bool is_u_longlong()	{ return type_code == ERL_U_LONGLONG;	}
	inline bool is_longlong()	{ return type_code == ERL_LONGLONG;		}
	inline bool is_list()		{ return type_code == ERL_LIST;			}
	inline bool is_tuple()		{ return type_code == ERL_TUPLE;		}

	void set(unsigned char, string, char *);
	void set(unsigned char, string, char *, int, int);
	void set(unsigned char, string, char *, int, int, int);
	void set(unsigned char, string, unsigned char *, int);
	void set(unsigned char, string, double);
	void set(unsigned char, string, int);
	void set(unsigned char, string, unsigned int);
	void set(unsigned char, string, long long);
	void set(unsigned char, string, unsigned long long);
	void set(unsigned char, string, term, unsigned long long);

	unsigned long long length();

	static term format(string, ...);

	static inline term atom(string & s)							{ return term(ATOM, s);			};
	static inline term binary(string & s)						{ return term(BINARY, s);		};
	static inline term strng(string & s)						{ return term(STRING, s);		};
	static inline term integer(int i)							{ return term(i);				};
	static inline term integer(long l)							{ return term(l);				};
	static inline term integer(long long ll)					{ return term(ll);				};
	static inline term integer(unsigned int ui)					{ return term(ui);				};
	static inline term integer(unsigned long ul)				{ return term(ul);				};
	static inline term integer(unsigned long long ull)			{ return term(ull);				};
	static inline term dbl(double d)							{ return term(d);				};
	static inline term pid(string & _str, int n, int s, int c)	{ return term(_str, n, s, c);	};
	static inline term ref(string & _str, int n, int c)			{ return term(REF, _str, n, c);	};
	static inline term port(string & _str, int n, int c)		{ return term(PORT, _str, n, c);};

	static inline term tuple(void)								{ return term(TUPLE_START);		};
	static inline term list(void)								{ return term(LIST_START);		};

	inline void add(term t)										{ lt.push_back(t);				}

private:	
	static void format_tokenizer(vector<term::TOKEN> &, string);
	inline term(TYPE t)
	{
		switch(t) {
			case TUPLE_START:	type = "ERL_TUPLE";			type_code = ERL_TUPLE;		break;
			case LIST_START:	type = "ERL_LIST";			type_code = ERL_LIST;		break;
			case ATOM:			type = "ERL_ATOM";			type_code = ERL_ATOM;		break;
			case INTEGER:		type = "ERL_INTEGER";		type_code = ERL_INTEGER;	break;
			case LONG:			type = "ERL_INTEGER";		type_code = ERL_INTEGER;	break;
			case LONGLONG:		type = "ERL_LONGLONG";		type_code = ERL_LONGLONG;	break;
			case UINTEGER:		type = "ERL_U_INTEGER";		type_code = ERL_U_INTEGER;	break;
			case ULONG:			type = "ERL_U_INTEGER";		type_code = ERL_U_INTEGER;	break;
			case ULONGLONG:		type = "ERL_U_LONGLONG";	type_code = ERL_U_LONGLONG;	break;
			case DOUBLE:		type = "ERL_FLOAT";			type_code = ERL_FLOAT;		break;
			case STRING:		type = "ERL_LIST";			type_code = ERL_LIST;		break;
			case BINARY:		type = "ERL_BINARY";		type_code = ERL_BINARY;		break;
			case PID:			type = "ERL_PID";			type_code = ERL_PID;		break;
			case PORT:			type = "ERL_PORT";			type_code = ERL_PORT;		break;
			case REF:			type = "ERL_REF";			type_code = ERL_REF;		break;
		}
	};
};

#endif //_TERM_H_