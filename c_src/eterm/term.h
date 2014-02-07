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
	union {
		vector<term> * v;
		string * s;
		int	i;
		unsigned int ui;
		double d;
		long l;
		long long ll;
		unsigned long ul;
		unsigned long long ull;
	} v;
	string type;
	unsigned char type_code;

	inline term() { v.v = NULL; v.s = NULL; };
	inline ~term()
	{
		if(!(v.v) && (type_code == ERL_TUPLE
				   || type_code == ERL_LIST))
			delete v.v;
		else if(!(v.s) && (type_code == ERL_ATOM
			            || type_code == ERL_PID
			            || type_code == ERL_PORT
			            || type_code == ERL_REF
			            || type_code == ERL_BINARY
						))
			delete v.s;
	};

	void set(unsigned char, string, char *);
	void set(unsigned char, string, unsigned char *, int);
	void set(unsigned char, string, double);
	void set(unsigned char, string, int);
	void set(unsigned char, string, unsigned int);
	void set(unsigned char, string, long long);
	void set(unsigned char, string, unsigned long long);
	void set(unsigned char, string, term, unsigned long long);
};

#endif //_TERM_H_
