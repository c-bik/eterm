#ifdef __WIN32__
#include <WinSock.h>
#else
#include <unistd.h>
#endif

#include "erl_comm.h"

#include <stdio.h>
#include <iostream>
using namespace std;

int i_read(byte * buf, int to_read)
{
	cin.read((char*)buf, to_read);
	return (int)cin.gcount();
}

int i_write(byte * buf, int to_write)
{
	size_t before = cout.tellp();
	if (cout.write((char*)buf, to_write)) {
		if(cout.fail())
			return (-1);
		else {
			size_t now = cout.tellp();
			return (int)(now - before);
		}
	}
}

int read_exact(byte *buf, long len)
{
	int i, got=0;

	do {
		if ((i = i_read(buf+got, len-got)) <= 0)
			return(i);
		got += i;
	} while (got<len);

	return(len);
}

int write_exact(byte *buf, int len)
{
	int i, wrote = 0;

	do {
		if ((i = i_write(buf+wrote, len-wrote)) <= 0)
			return (i);
		wrote += i;
	} while (wrote<len);

	return (len);
}

int read_cmd(byte *buf)
{
	int len = 0;

	while(len == 0) {
		if(cin.eof())
			return(-1);
		if (read_exact(buf, 4) == 4)
			len = 4;
	}

	len = (int)ntohl(*((u_long*)buf));
	return read_exact(buf, len);
}

int write_cmd(byte *buf, int len)
{
	byte li[4];
	u_long _len;

	_len = htonl((u_long)len);
	memcpy(li, &_len, 4);

	write_exact(li, 4);

	return write_exact(buf, len);
}