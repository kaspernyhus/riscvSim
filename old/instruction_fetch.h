#ifndef INSTRUCTION_FETCH_H_
#define INSTRUCTION_FETCH_H_

#include <fstream>
#include <iostream>
#include <vector>

using namespace std;


class Instructions {
  private:
    uint32_t current_pc;
    uint32_t next_pc;
  
  public:
    Instructions();
    uint32_t get_current_pc() {return current_pc;};
    uint32_t get_next_pc() {return next_pc;};
    uint32_t next();
    void set_pc(uint32_t new_pc);
    void set_pc_relative(int32_t new_pc);
};






#endif