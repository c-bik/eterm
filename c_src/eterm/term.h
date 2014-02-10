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
};

#endif //_TERM_H_
