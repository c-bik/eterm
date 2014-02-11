#include "term.h"

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
