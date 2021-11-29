/*
---------------------------------
RISC-V instructionset simulator

Kasper Nyhus Kaae
01/11/21
---------------------------------
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
  0xdeadc1b7,
  0xeef18193,
  0xc0dec237,
  0xabe20213,
  0x10000113,
  0x00312023,
  0x00411123,
  0x00000073
};



int main(int argc, char *argv[])
{ 
  cout << "\n\n";
  cout << "* --------------------------------- *\n" << "             Welcome to:\n" << "           Kasper Nyhus's\n" << "   RISC-V instructionset simulator\n" << "* --------------------------------- *\n";
  bool debug = false;

  for(int i=0; i<argc; ++i) {
    if(string(argv[i]) == "-d") {
      debug = true;
      CPU.debug_on();
    }
  }

  memory.load_instructions(argv[1]);
  // memory.load_instructions(test_prog);
  
  while(1) {
    if(CPU.step()) break;
  }
  
  if(debug) { 
    memory.print_bytes();
    memory.print();
  }
  

  cout << "\n\n* --------------------------------- *\n" << "          Simulation ended!\n" << "      Instructions executed: " << dec << CPU.get_cycle_count() << "\n* --------------------------------- *" << endl;
  CPU.dump();
  return 0;
}
