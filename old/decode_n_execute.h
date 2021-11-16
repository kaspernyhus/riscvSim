#ifndef DECODE_N_EXECUTE_H_
#define DECODE_N_EXECUTE_H_

#include <iostream>
#include <bitset>
#include "registers.h"
#include "instruction_fetch.h"
#include "memory.h"


const int DEBUG = 0;

using namespace std;

bool decode_execute_update(uint32_t pc, Registers &registers, Memory &memory, Instructions &instructions);


#endif