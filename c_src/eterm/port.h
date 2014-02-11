#pragma once
#ifndef _ERL_COMM_H_
#define _ERL_COMM_H_

#include "platform.h"

#include <iostream>
#include <vector>

using namespace std;

class port
{
private:
	int stdi;
	int stdo;
	mutex_type port_io_lock;
	inline bool lock();
	inline void unlock();
	int read_exact(vector<unsigned char> &, unsigned long);
	int write_exact(vector<unsigned char> &);

	port(void);
	port(port const&);          // Not implemented
    void operator=(port const&); // Not implemented

public:
	static port & getInstance()
	{
		static port instance;
		return instance;
	}
	int read_cmd(vector<unsigned char>&);
	int write_cmd(vector<unsigned char>&);

	inline ~port(void) {};
};

#endif // _ERL_COMM_H_