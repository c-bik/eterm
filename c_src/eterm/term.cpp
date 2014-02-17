#include "platform.h"
#include "term.h"

term term::format(string fmt, ...)
{
	term t;
	vector<TOKEN> tokens;
	format_tokenizer(tokens, fmt);

	for(size_t idx = 0; idx < tokens.size(); ++idx) {
		switch(tokens[idx].t) {
			case ATOM:
				break;
			case INTEGER:
				break;
			case LONG:
				break;
			case LONGLONG:
				break;
			case UINTEGER:
				break;
			case ULONG:
				break;
			case ULONGLONG:
				break;
			case DOUBLE:
				break;
			case STRING:
				break;
			case BINARY:
				break;
			case TERM:
				break;
			case PORT:
				break;
			case REF:
				break;
			case PID:
				break;
			case VALUE:
				break;
			case TUPLE_START: {
					size_t idx1 = idx;
					for(; idx1 < tokens.size(); ++idx1) {
						if(tokens[idx1].t == TUPLE_END)
							break;
					}
							  }
				break;
			case LIST_START: {
					size_t idx1 = idx;
					for(; idx1 < tokens.size(); ++idx1) {
						if(tokens[idx1].t == LIST_END)
							break;
					}

							  }
				break;
		}
	}

	return t;
}

void term::format_tokenizer(vector<term::TOKEN> & tokens, string fmt)
{
	size_t end_idx = 0;
	for(size_t idx = 0; idx < fmt.size(); ++idx) {
		switch (fmt[idx]) {
			case ',': case ' ': break; // skipping the ',' (commas) or ' ' (spaces)
			case '{': tokens.push_back(TOKEN(TUPLE_START)); break;
			case '[': tokens.push_back(TOKEN(LIST_START)); break;
			case '}': tokens.push_back(TOKEN(TUPLE_END)); break;
			case ']': tokens.push_back(TOKEN(LIST_END)); break;
			case '~': {
						size_t arr[] = {fmt.find(',',idx), fmt.find('}',idx), fmt.find(']',idx)};
						vector<size_t> ends(arr, arr+sizeof(arr)/sizeof(arr[0]));
						size_t next = *min_element(ends.begin(), ends.end()) - 1;
						string eoa = fmt.substr(idx+1,next-idx);
						if (0);
						else if (eoa == "a") tokens.push_back(TOKEN(ATOM));
						else if (eoa == "i") tokens.push_back(TOKEN(INTEGER));
						else if (eoa == "l") tokens.push_back(TOKEN(LONG));
						else if (eoa == "ll") tokens.push_back(TOKEN(LONGLONG));
						else if (eoa == "ui") tokens.push_back(TOKEN(UINTEGER));
						else if (eoa == "ul") tokens.push_back(TOKEN(ULONG));
						else if (eoa == "ull") tokens.push_back(TOKEN(ULONGLONG));
						else if (eoa == "f") tokens.push_back(TOKEN(DOUBLE));
						else if (eoa == "s") tokens.push_back(TOKEN(STRING));
						else if (eoa == "b") tokens.push_back(TOKEN(BINARY));
						else if (eoa == "w") tokens.push_back(TOKEN(TERM));
						idx = next;
					  }
				break;
			// Constants in format are not supported list (use ~w instead)
			case '\'': // atoms may contain \" or \'
				for (end_idx = idx+1; end_idx < fmt.length(); ++end_idx) {
					if (end_idx+1 < fmt.length() && fmt[end_idx] != '\\' && fmt[end_idx+1] == '\'') {
						end_idx++;
						break;
					}
				}
				if (end_idx > idx) { // We have an atom
					string sstr = fmt.substr(idx, end_idx-idx);
					idx = end_idx;
					tokens.push_back(TOKEN(ATOM,sstr));
				} else {
					tokens.push_back(TOKEN(UNKNOWN));
				}
				break;
			case '"': // strings may contain \"
				for (end_idx = idx+1; end_idx < fmt.length(); ++end_idx) {
					if (end_idx+1 < fmt.length() && fmt[end_idx] != '\\' && fmt[end_idx+1] == '"') {
						end_idx+=2;
						break;
					}
				}
				if (end_idx > idx) { // We have a string
					string sstr = fmt.substr(idx, end_idx-idx);
					idx = end_idx;
					tokens.push_back(TOKEN(STRING,sstr));
				} else {
					tokens.push_back(TOKEN(UNKNOWN));
				}
				break;
			default:
				// binary <<,...>>, (<<"...">>, may contain all escapable characters field markers not allowed
				if (idx+1 < fmt.length() && fmt[idx] == '<' && fmt[idx+1] == '<') { // start of binary
					for (end_idx = idx+2; end_idx < fmt.length(); ++end_idx) {
						if (end_idx+1 < fmt.length() && fmt[end_idx] == '>' && fmt[end_idx+1] == '>') {
							end_idx+=2;
							break;
						}
					}
					if (end_idx > idx) { // We have a binary
						string sstr = fmt.substr(idx, end_idx-idx);
						idx = end_idx;
						tokens.push_back(TOKEN(BINARY,sstr));
					} else {
						tokens.push_back(TOKEN(UNKNOWN));
					}
				}
				// regular atom (starting with a-z containing [A-Za-z0-9_@]*)
				else if(fmt[idx] >= 'a' && fmt[idx] <= 'z') { // unquoted atom
					tr1::regex re("([a-z0-9_@]*)");
					tr1::cmatch res;
					string sstr = fmt.substr(idx, fmt.length()-idx);
					regex_search(sstr.c_str(), res, re);
					idx += res[0].length();
					tokens.push_back(TOKEN(ATOM,res[0]));
				}
				// 	integer, double ([+\-\.]*[0-9]+[\.0-9]*)
				else if ((fmt[idx] >= '0' && fmt[idx] <= '9')
					|| fmt[idx] == '+'
					|| fmt[idx] == '-'
					|| fmt[idx] == '.') {
						size_t _idx = fmt.find(',',idx);
						string sstr = fmt.substr(idx, _idx - idx);
						if (sstr.find('.',0) !=  string::npos)
							tokens.push_back(TOKEN(DOUBLE,sstr));
						else
							tokens.push_back(TOKEN(INTEGER,sstr));
						idx = _idx;
				}
				else {
					tokens.push_back(TOKEN(VALUE,""));
				}
				break;
		}
	}
}

unsigned long long term::length()
{
	switch (type_code) {
		case ERL_ATOM:			return str.size();
		case ERL_FLOAT:			return sizeof(v.d);
		case ERL_PID:			return str.size()+sizeof(v.ppr);
		case ERL_PORT:			return str.size()+sizeof(v.ppr);
		case ERL_REF:			return str.size()+sizeof(v.ppr);
		case ERL_BINARY:		return str.size();
		case ERL_INTEGER:		return sizeof(v.i);
		case ERL_U_INTEGER:		return sizeof(v.ui);
		case ERL_U_LONGLONG:	return sizeof(v.ull);
		case ERL_LONGLONG:		return sizeof(v.ll);
		case ERL_LIST:			return lt.size();
		case ERL_TUPLE:			return lt.size();
		default:				return 0;
	}
}

void term::set(unsigned char t, string typestr, char * s)
{
	type_code = t;
	type = typestr;
	str = s;
}

void term::set(unsigned char t, string typestr, char *ns, int n, int c)
{
	type_code = t;
	type = typestr;
	str = ns;
	v.ppr.n = n;
	v.ppr.c = c;
}

void term::set(unsigned char t, string typestr, char *ns, int n, int s, int c)
{
	type_code = t;
	type = typestr;
	str = ns;
	v.ppr.n = n;
	v.ppr.s = s;
	v.ppr.c = c;
}

void term::set(unsigned char t, string typestr, unsigned char * s, int strl)
{
	type_code = t;
	type = typestr;
	str.assign((char*)s, strl);
}

void term::set(unsigned char t, string typestr, double dbl)
{
	type_code = t;
	type = typestr;
	v.d = dbl;
}

void term::set(unsigned char t, string typestr, int i)
{
	type_code = t;
	type = typestr;
	v.i = i;
}

void term::set(unsigned char t, string typestr, unsigned int ui)
{
	type_code = t;
	type = typestr;
	v.ui = ui;
}

void term::set(unsigned char t, string typestr, long long ll)
{
	type_code = t;
	type = typestr;
	v.ll = ll;
}

void term::set(unsigned char t, string typestr, unsigned long long ull)
{
	type_code = t;
	type = typestr;
	v.ull = ull;
}

void term::set(unsigned char t, string typestr, term trm, unsigned long long idx)
{
	type_code = t;
	type = typestr;
	lt.push_back(trm);
}
