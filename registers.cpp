
#include "registers.h"


string register_names[32] = {
    "x0        : ",
    "x1  (ra)  : ",
    "x2  (sp)  : ",
    "x3  (gp)  : ",
    "x4  (tp)  : ",
    "x5  (t0)  : ",
    "x6  (t1)  : ",
    "x7  (t2)  : ",
    "x8  (s0)  : ",
    "x9  (s1)  : ",
    "x10 (a0)  : ",
    "x11 (a1)  : ",
    "x12 (a2)  : ",
    "x13 (a3)  : ",
    "x14 (a4)  : ",
    "x15 (a5)  : ",
    "x16 (a6)  : ",
    "x17 (a7)  : ",
    "x18 (s2)  : ",
    "x19 (s3)  : ",
    "x20 (s4)  : ",
    "x21 (s5)  : ",
    "x22 (s6)  : ",
    "x23 (s7)  : ",
    "x24 (s8)  : ",
    "x25 (s9)  : ",
    "x26 (s10) : ",
    "x27 (s11) : ",
    "x28 (t3)  : ",
    "x29 (t4)  : ",
    "x30 (t5)  : ",
    "x31 (t6)  : "
  };


Registers::Registers() : registers(32,0) 
{
  
}

/**
 * Write data to a register
 * 
 * @param reg register number to srite to
 * @param value data value 32-bit unsigned 
**/
void Registers::write(uint8_t reg, uint32_t value)
{ 
  if(reg != 0) registers[reg] = value;
}


/**
 * Print out register content to console
 * 
**/
void Registers::print_all()
{
  cout << "\n------------------------------\n" << "Register content\n" << "------------------------------\n";
  for(int i=0; i<32; ++i) {
    cout << register_names[i] << "0x" << std::setfill ('0') << std::setw(8) << hex << registers[i] << "\n";
  }
  cout << "------------------------------\n";
}



/**
 * Dumps register content to file
 * 
**/
void Registers::dump_to_files()
{
  ofstream ResFile("result.res");
  int new_line = 0;
  for(int i=0; i<32; ++i) {
    ResFile << std::setfill ('0') << std::setw(2) << hex << (registers[i]&0x000000ff) << std::setfill ('0') << std::setw(2) << hex << ((registers[i]&0x0000ff00)>>8) << " ";
    ResFile << std::setfill ('0') << std::setw(2) << hex << ((registers[i]&0x00ff0000)>>16) << std::setfill ('0') << std::setw(2) << hex << ((registers[i] >> 24)&0xff) << " ";
    new_line++;
    if(new_line>=4) {
      new_line = 0;
      ResFile << "\n";
    }
  }
  ResFile.close();

  ofstream ResFile2("result.txt");
  new_line = 0;
  for(int i=0; i<32; ++i) {
    ResFile2 << std::setfill ('0') << std::setw(2) << hex << (registers[i]&0x000000ff) << std::setfill ('0') << std::setw(2) << hex << ((registers[i]&0x0000ff00)>>8) << " ";
    ResFile2 << std::setfill ('0') << std::setw(2) << hex << ((registers[i]&0x00ff0000)>>16) << std::setfill ('0') << std::setw(2) << hex << ((registers[i] >> 24)&0xff) << " ";
    new_line++;
    if(new_line>=4) {
      new_line = 0;
      ResFile2 << "\n";
    }
  }
  ResFile2.close();
}
