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
  0xdeadc237,
  0xeef20213,
  0x010002b7,
  0x0042a023,
  0x0042a223,
  0x00428423,
  0x00429623,
  0x0042aa23,
  0x004288a3,
  0x0002a303,
  0x00100893,
  0x00000073
};



int main(void)
{
  cout << "\n\n";
  cout << "* --------------------------------- *\n" << "             Welcome to:\n" << "           Kasper Nyhus's\n" << "   RISC-V instructionset simulator\n" << "* --------------------------------- *\n";

  // CPU.debug_on();

  memory.load_instructions("tests/task3/loop.bin");
  // memory.load_instructions(test_prog);
  // memory.print();

  
  while(1) {
    if(CPU.step()) break;
  }
  
  memory.print();

  cout << "\n\n* --------------------------------- *\n" << "          Simulation ended!\n" << "      Instructions executed: " << dec << CPU.get_cycle_count() << "\n* --------------------------------- *" << endl;
  CPU.dump();
  return 0;
}
