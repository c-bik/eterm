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
		vector<term> * lt; // list or tuple
		int	i;
		unsigned int ui;
		double d;
		long l;
		long long ll;
		unsigned long ul;
		unsigned long long ull;
		struct {
			string * v;
			int n;
			int s;
			int c;
		} ppr; // pid, port, ref, string or binary
	} v;
	string type;
	unsigned char type_code;

	inline term() { v.lt = NULL; v.ppr.v = NULL; };
	inline ~term()
	{
		if(!(v.lt) && (type_code == ERL_TUPLE
					|| type_code == ERL_LIST))
			delete v.lt;
		else if(!(v.ppr.v) && (type_code == ERL_ATOM
							|| type_code == ERL_PID
							|| type_code == ERL_PORT
							|| type_code == ERL_REF
							|| type_code == ERL_BINARY
						))
			delete v.ppr.v;
	};

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
};

#endif //_TERM_H_
