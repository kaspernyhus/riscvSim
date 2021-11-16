#ifndef REGISTERS_H_
#define REGISTERS_H_

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>


using namespace std;

/*
Register    ABI Name	Description	                        Saver
x0	        zero	    hardwired zero	                    -
x1	        ra	        return address	                    Caller
x2	        sp	        stack pointer	                    Callee
x3	        gp	        global pointer	                    -
x4	        tp	        thread pointer	                    -
x5-7        t0-2	    temporary registers	                Caller
x8	        s0 / fp	    saved register / frame pointer	    Callee
x9	        s1	        saved register	                    Callee
x10-11	    a0-1	    function arguments / return values	Caller
x12-17	    a2-7	    function arguments	                Caller
x18-27	    s2-11	    saved registers	                    Callee
x28-31	    t3-6	    temporary registers	                Caller
*/


class Registers {
  private:
    vector<int> registers;
  
  public:
    Registers();
    void write(uint8_t reg, uint32_t value);
    int32_t& read(uint8_t reg) {return registers[reg];}
    void print_all();
    void dump_to_files();
};


#endif