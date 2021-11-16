#ifndef UTILS_H_
#define UTILS_H_

#include <iostream>

using namespace std;

int32_t signextend(uint8_t signbit_num, int32_t value);
int32_t calc_branch_offset(uint32_t instruction);
int32_t calc_jal_offset(uint32_t instruction);
int32_t calc_store_offset(uint32_t instruction);



#endif