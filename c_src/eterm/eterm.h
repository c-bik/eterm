#pragma once
#ifndef _ETERM_H_
#define _ETERM_H_

#include <iostream>
#include <string>
#include <vector>

#include "erl_interface.h"
#include "ei.h"
#include "term.h"

#ifdef __WIN32__
	typedef HANDLE mutex_type;
#else
	typedef  pthread_mutex_t mutex_type;
#endif

class eterm
{
private:
	static mutex_type transcoder_lock;
	static inline bool lock();
	static inline void unlock();

	eterm(void);
	eterm(eterm const&);          // Not implemented
    void operator=(eterm const&); // Not implemented

public:
	static eterm & getInstance()
	{
		static eterm instance;
		return instance;
	}
	static inline void get_stats(unsigned long & allocated, unsigned long & freed) { erl_eterm_statistics(&allocated,&freed); };
	term decode(vector<byte> &);
	vector<byte> encode(term &);
	inline ~eterm(void) {};
};

class simple : public term
{
public:
	string val;

	simple(string _val) : term() {val = _val;}
	~simple(void) {};
};

#endif //_ETERM_H_