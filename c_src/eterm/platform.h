#pragma once
#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#ifdef __WIN32__
	#include <io.h>
	#include <fcntl.h>
	#include <stdio.h>
	#include <WinSock2.h>
	#include <Windows.h>
	typedef u_long ul4;
	typedef HANDLE mutex_type;
#else
	#include <stdlib.h>
	#include <unistd.h>
	#include <string.h>
	#include <unistd.h>
	#include <arpa/inet.h>
	typedef  pthread_mutex_t mutex_type;
	typedef uint32_t ul4;
#endif

#endif //_PLATFORM_H_