#ifdef __WIN32__
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#else
#include <stdlib.h>
#include <unistd.h>
#endif

#include "erl_comm.h"
#include "eterm.h"

int main(int argc, char* argv[])
{
#ifdef __WIN32__
    _setmode( _fileno( stdout ), _O_BINARY );
    _setmode( _fileno( stdin  ), _O_BINARY );
#endif

	eterm& et = eterm::getInstance();
	eterm& et1 = eterm::getInstance();

	byte buf[] = 
	//{131,100,0,1,97}; // atom a
	//{131,107,0,3,1,2,3}; // [1,2,3]
	//{131,104,3,97,1,97,2,97,3}; // {1,2,3}
	//{131,108,0,0,0,2,104,2,97,1,97,2,97,3,106}; //[{1,2},3]
	//{131,104,2,97,1,107,0,2,2,3}; //{1,[2,3]}
	//{131,103,100,0,13,110,111,110,111,100,101,64,110,111,104,111,115,116,0,0,0,32,0,0,0,0,0}; // self()
	//{131,114,0,3,100,0,13,110,111,110,111,100,101,64,110,111,104,111,115,116,0,0,0,0,44,0,0,0,0,0,0,0,0}; // ref
	//*
	{131,108,0,0,0,4,104,2,100,0,1,97,114,0,3,100,0,13,110,111,110,111,100,101,64, //[{a,make_ref()},{1,self()},<<1,2,3>>,{"test",4.796}]
	110,111,104,111,115,116,0,0,0,0,58,0,0,0,0,0,0,0,0,104,2,97,1,103,100,0,13,
	110,111,110,111,100,101,64,110,111,104,111,115,116,0,0,0,32,0,0,0,0,0,109,0,
	0,0,3,1,2,3,104,2,107,0,4,116,101,115,116,99,52,46,55,57,54,48,48,48,48,48,
	48,48,48,48,48,48,48,51,48,48,48,48,101,43,48,48,0,0,0,0,0,106};//*/

	term t = et.decode(buf);
	vector<unsigned char> b = et.encode(t);

	size_t s = b.size();
	unsigned char *out_buf = (unsigned char *)(&b[0]);

	while (read_cmd(buf) > 0) {
		printf("Just a breakpoint...");
	}

	out_buf = NULL;
	s = 0;
	return 0;
}