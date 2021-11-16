#include "memory.h"


/**
 * Copy instructions from vector into memory at 0x00000000
 * 
 * @param instructions vector of type 32 unsigned bit
**/
void Memory::load_instructions(vector<uint32_t> instructions)
{
  for(uint32_t i=0;i<instructions.size();++i) {
    MemoryEntry new_entry = {i*4, (int32_t)instructions.at(i)};
    block.push_back(new_entry);
  }
}


/**
 * Copy instructions from binary file into memory at 0x00000000
 * 
 * @param filename path to file
**/
void Memory::load_instructions(const char* filename)
{
  vector<int32_t> temp_instr;
  cout << "Reading from file:\n" << filename << endl;
  // open the file:
  std::streampos fileSize;
  std::ifstream file(filename, std::ios::binary);
  
  if(!file) {
    cout << "Failed to open file!" << endl;
    exit(0);
  }

  // get its size:
  file.seekg(0, std::ios::end);
  fileSize = file.tellg();
  file.seekg(0, std::ios::beg);

  // read the data:
  temp_instr.resize(fileSize);
  file.read((char*) &temp_instr[0], fileSize);

  for(uint32_t i=0;i<temp_instr.size();++i) {
    MemoryEntry new_entry = {i*4, (int32_t)temp_instr.at(i)};
    block.push_back(new_entry);
  }
}


/**
 * Print address: memory data for data stored in memory
 * 
**/
void Memory::print()
{
  cout << "----------- Memory -----------" << endl;
  for(MemoryEntry m : block) {
    cout << hex << m.address << "\t: " << "0x" << std::setfill ('0') << std::setw(8) << hex << m.data << endl;
  }
  cout << "------------------------------" << endl;
}


/**
 * Write to an address in memory
 * 
 * @param address to write to, data to save
**/
void Memory::write(uint32_t addr, int32_t data)
{
  // see if address already in memory
  bool address_found {false};
  for(auto &e: block) {
    if (e.address == addr) {
      e.data = data;
      address_found = true;
    }
  }
  if(!address_found) {
    MemoryEntry new_entry = {addr, data};
    block.push_back(new_entry);
  }
}


/**
 * Write to an address in memory
 * 
 * @param addr address to write to
 * @param bitmask mask out unwanted data eg lb: 0x000000ff
**/
int32_t Memory::read(uint32_t addr, uint32_t bitmask)
{
  for(int i=0; i<block.size(); ++i) {
    if(block[i].address == addr) return block[i].data&bitmask;
  }
  return 0;
}