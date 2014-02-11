#include "platform.h"
#include "port.h"
#include "eterm.h"

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

DWORD WINAPI term_processor(LPVOID);

#include <queue>
using namespace std;
static queue<vector<unsigned char> > mq;
static 	mutex_type q_lock;

bool lock()
{
#ifdef __WIN32__
	return (WAIT_OBJECT_0 == WaitForSingleObject((q_lock),INFINITE));
#else
	return (0 == pthread_mutex_lock(&q_lock));
#endif
}

void unlock()
{
#ifdef __WIN32__
    ReleaseMutex(q_lock);
#else
    pthread_mutex_unlock(&q_lock);
#endif
}

int main(int argc, char* argv[])
{
#ifdef __WIN32__
    q_lock = CreateMutex(NULL, FALSE, NULL);
    if (NULL == q_lock) {
        return 0;
    }
#else
    if(pthread_mutex_init(&q_lock, NULL) != 0) {
        return 0;
    }
#endif

	eterm& et = eterm::getInstance();
	port& prt = port::getInstance();

	vector<unsigned char> read_buf(4);
	while (prt.read_cmd(read_buf) > 0) {
		//term t = et.decode(read_buf);
		if(lock()) {
			mq.push(read_buf);
			unlock();
		}
		CreateThread(NULL,0,term_processor,NULL,0,NULL);
		//vector<unsigned char> write_buf = et.encode(t);
		//prt.write_cmd(write_buf);
	}

	return 0;
}

DWORD WINAPI term_processor(LPVOID lpParam)
{ 
	eterm& et = eterm::getInstance();
	port& prt = port::getInstance();

	vector<unsigned char> read_buf;
	if(lock()) {
		read_buf = mq.front();
		mq.pop();
		unlock();
	}
	term t = et.decode(read_buf);
	vector<unsigned char> write_buf = et.encode(t);
	prt.write_cmd(write_buf);
	return 0;
} 
