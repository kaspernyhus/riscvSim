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

Memory memory;
class CPU CPU(memory);


vector<uint32_t> test_prog {
  0xfd600513,
  0x00100893,
  0x00000073
};



int main(void)
{
  cout << "\n\n";
  cout << "* --------------------------------- *\n" << "             Welcome to:\n" << "           Kasper Nyhus's\n" << "   RISC-V instructionset simulator\n" << "* --------------------------------- *\n";

  // CPU.debug_on();

  // memory.load_instructions("tests/task3/loop.bin");
  memory.load_instructions(test_prog);
  // memory.print();
  
  while(1) {
    if(CPU.step()) break;
  }

  // /* Step through program */
  // CPU.debug_on();
  // const int STEPS = 9;
  // for(int i=0;i<STEPS;++i) {
  //   CPU.step();
  // }
  

  cout << "\n\n* --------------------------------- *\n" << "          Simulation ended!\n" << "      Instructions executed: " << dec << CPU.get_cycle_count() << "\n* --------------------------------- *" << endl;
  CPU.dump();
  return 0;
}
