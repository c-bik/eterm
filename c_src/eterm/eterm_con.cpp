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
	return LOCK(q_lock);
}

void unlock()
{
	UNLOCK(q_lock);
}

int main(int argc, char* argv[])
{
	term::format("{-123456789,.1,0.2,-.3,+4,+.5,un1quoted_atom@, <<\" \\\"Weird\\\" binary\">>, 'A very bad bu\\'t valid atom', 'Another one', \"A long \\\"complicated\\\" list\", <<1,2,3>>, {~a},~i,~w,[~ui,~ull,{~i}],~w}");
    if (INIT_LOCK(q_lock)) {
        return 0;
    }

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
