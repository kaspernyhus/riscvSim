#include "instruction_fetch.h"



Instructions::Instructions()
{
  current_pc = 0;
  next_pc = 0;
}


uint32_t Instructions::next() 
{
  current_pc = next_pc;
  next_pc += 4;
  return current_pc;
}


void Instructions::set_pc(uint32_t new_pc) 
{
  next_pc = new_pc;
};


void Instructions::set_pc_relative(int32_t new_pc) 
{
  current_pc += new_pc;
  next_pc = current_pc;
  if(next_pc<0) exit(0);
};

