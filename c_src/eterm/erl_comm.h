#pragma once
#ifndef _ERL_COMM_H_
#define _ERL_COMM_H_

typedef unsigned char byte;

extern int read_cmd(byte *);
extern int write_cmd(byte *, int);

#endif // _ERL_COMM_H_