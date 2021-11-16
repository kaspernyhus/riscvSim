#ifndef CPU_H_
#define CPU_H_

#include <iostream>
#include <vector>
#include "memory.h"
#include "registers.h"
#include "utils.h"


using namespace std;

class CPU {
  private:
    Memory &memory;
    Registers registers;
    uint32_t current_pc;
    uint32_t next_pc;
    void _set_pc(uint32_t new_pc);
    void _set_pc_relative(int32_t pc_offset);
    uint32_t _next();
    bool _decode_n_execute(uint32_t instruction);
    bool debug;
    int count_cycles;

  public:
    CPU(Memory& mem);
    bool step();
    bool step(uint16_t steps);
    void debug_on();
    void debug_off();
    int get_cycle_count() {return count_cycles;}
    uint32_t get_instruction() {return memory.read(current_pc,0xffffffff);}
    // uint32_t get_current_pc() {return current_pc;}
    // uint32_t get_next_pc() {return next_pc;}
    void write_to_mem(uint32_t addr, int32_t value) {memory.write(addr,value);}
    void print_registers() {registers.print_all();}
    void dump();
};


#endif