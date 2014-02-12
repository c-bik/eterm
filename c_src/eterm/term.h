#pragma once
#ifndef _TERM_H_
#define _TERM_H_

#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "erl_interface.h"
#include "ei.h"

class term {
public:
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

	void set(unsigned char, string, char *);
	void set(unsigned char, string typestr, char *, int, int);
	void set(unsigned char, string typestr, char *, int, int, int);
	void set(unsigned char, string, unsigned char *, int);
	void set(unsigned char, string, double);
	void set(unsigned char, string, int);
	void set(unsigned char, string, unsigned int);
	void set(unsigned char, string, long long);
	void set(unsigned char, string, unsigned long long);
	void set(unsigned char, string, term, unsigned long long);

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
};

#endif //_TERM_H_