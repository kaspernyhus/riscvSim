/*

RISC-V instructionset simulator

Kasper Nyhus Kaae
01/11/21

*/

#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include <bitset>
#include "memory.h"
#include "CPU.h"


using namespace std;

/* Class object instantiation */
Memory memory;
class CPU CPU(memory);


/* Test program that can be loaded into memory */
vector<uint32_t> test_prog {
  0x00010137,
  0xdeadc237,
  0xeef20213,
  0xc0dec2b7,
  0xabe28293,
  0x00412023,
  0xfe412623,
  0x00012303,
  0xfec12383,
  0xfe512823,
  0xfe512623,
  0xfec12403,
  0x00100893,
  0x00000073
};



int main(int argc, char *argv[])
{ 
  cout << "argc: " << argc << " argc[0]: " << argv[0] << " argv[1]: " << argv[1] << endl;
  
  cout << "\n\n";
  cout << "* --------------------------------- *\n" << "             Welcome to:\n" << "           Kasper Nyhus's\n" << "   RISC-V instructionset simulator\n" << "* --------------------------------- *\n";

  // CPU.debug_on();

  // memory.load_instructions(argv[1]);
  // memory.load_instructions(test_prog);
  // memory.print();

  
  while(1) {
    if(CPU.step()) break;
  }
  
  // memory.print_bytes();
  // memory.print();


  cout << "\n\n* --------------------------------- *\n" << "          Simulation ended!\n" << "      Instructions executed: " << dec << CPU.get_cycle_count() << "\n* --------------------------------- *" << endl;
  CPU.dump();
  return 0;
}
