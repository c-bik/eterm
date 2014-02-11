#include "port.h"

#include <stdio.h>

port::port(void)
{
#ifdef __WIN32__
	stdi = _fileno(stdin);
	stdo = _fileno(stdout);
    _setmode(stdi, _O_BINARY );
    _setmode(stdo, _O_BINARY);
    port_io_lock = CreateMutex(NULL, FALSE, NULL);
    if (NULL == port_io_lock) {
        return;
    }
#else
	stdi = 0;
	stdo = 1;
    if(pthread_mutex_init(&port_io_lock, NULL) != 0) {
        return;
    }
#endif
}

bool port::lock()
{
#ifdef __WIN32__
	return (WAIT_OBJECT_0 == WaitForSingleObject((port_io_lock),INFINITE));
#else
	return (0 == pthread_mutex_lock(&port_io_lock));
#endif
}

void port::unlock()
{
#ifdef __WIN32__
    ReleaseMutex(port_io_lock);
#else
    pthread_mutex_unlock(&port_io_lock);
#endif
}

int port::read_exact(vector<unsigned char> & buf, unsigned long len)
{
	int i;
	unsigned long got=0;

	if (buf.size() < len)
		buf.resize(len);
	do {
		if ((i = read(stdi, &buf[0]+got, len-got)) <= 0)
			return(i);
		got += i;
	} while (got<len);
	return(len);
}

int port::write_exact(vector<unsigned char> & buf)
{
	int i, wrote = 0;

	int len = (int)buf.size();
	do {
		if ((i = write(stdo, &buf[0]+wrote, len-wrote)) <= 0)
			return (i);
		wrote += i;
	} while (wrote<len);

	return (len);
}

int port::read_cmd(vector<unsigned char> & buf)
{
	int len = 0;
	buf.clear();
	if (lock()) {
		if(read_exact(buf, 4) < 0) {
			unlock();
			return (-1);
		}
		len = read_exact(buf, ntohl(*((ul4*)&buf[0])));
		unlock();
	}
	return len;
}

int port::write_cmd(vector<unsigned char> & buf)
{
	vector<unsigned char> li(sizeof(ul4));
	
	ul4 len = htonl((ul4)buf.size());
	memcpy(&li[0], &len, sizeof(ul4));

	if(lock()) {
		write_exact(li);
		len = write_exact(buf);
		unlock();
	}
	return len;
}
