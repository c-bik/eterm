#ifdef __WIN32__
#include <WinSock.h>
typedef u_long ul;
#else
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
typedef uint32_t ul;
#endif

#include "erl_comm.h"

#include <stdio.h>

int i_read(byte * buf, int to_read)
{
	cin.read((char*)buf, to_read);
	return (int)cin.gcount();
}

int i_write(byte * buf, int to_write)
{
	size_t before = cout.tellp();
	if (cout.write((char*)buf, to_write)) {
		cout.flush();
		if(!cout.fail()) {
			size_t now = cout.tellp();
			return (int)(now - before);
		}
	}
	return (-1);
}

int read_exact(vector<byte> & buf, long len)
{
	int i, got=0;

	if ((long)buf.size() < len)
		buf.resize(len);
	do {
		if ((i = i_read(&buf[0]+got, len-got)) <= 0)
			return(i);
		got += i;
	} while (got<len);

	return(len);
}

int write_exact(vector<byte> & buf)
{
	int i, wrote = 0;

	int len = (int)buf.size();
	do {
		if ((i = i_write(&buf[0]+wrote, len-wrote)) <= 0)
			return (i);
		wrote += i;
	} while (wrote<len);

	return (len);
}

int read_cmd(vector<byte> & buf)
{
	int len = 0;

	while(len == 0) {
		if(cin.eof())
			return(-1);
		if (read_exact(buf, 4) == 4)
			len = 4;
	}

	len = (int)ntohl(*((ul*)&buf[0]));
	return read_exact(buf, len);
}

int write_cmd(vector<byte> & buf)
{
	vector<byte> li(4);
	ul _len;

	int len = (int)buf.size();
	_len = htonl((ul)len);
	memcpy(&li[0], &_len, 4);

	write_exact(li);

	return write_exact(buf);
}
