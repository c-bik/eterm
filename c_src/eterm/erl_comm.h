#pragma once
#ifndef _ERL_COMM_H_
#define _ERL_COMM_H_

#include <iostream>
#include <vector>

using namespace std;

#include "eterm.h"
extern int read_cmd(vector<byte>&);
extern int write_cmd(vector<byte>&);

#endif // _ERL_COMM_H_
