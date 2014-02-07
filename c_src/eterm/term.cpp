#include "term.h"

void term::set(unsigned char t, string typestr, char * str)
{
	type_code = t;
	type = typestr;
	v.s = new string(str);
}

void term::set(unsigned char t, string typestr, unsigned char * str, int strl)
{
	type_code = t;
	type = typestr;
	v.s = new string((char*)str, strl);
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
	if (!(v.v))
		v.v = new vector<term>;
	v.v->push_back(trm);
}
