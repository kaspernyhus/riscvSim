#include "memory.h"


/**
 * Copy instructions from vector into memory at 0x00000000
 * 
 * @param instructions vector of type 32 unsigned bit
**/
void Memory::load_instructions(vector<uint32_t> instructions)
{
  for(uint32_t i=0; i<instructions.size(); ++i) {
    for(uint32_t j=0; j<4; ++j) {
      MemoryEntry new_entry = {(i*4)+j, (uint8_t)((instructions.at(i)>>(j*8))&0xff)};
      block.push_back(new_entry);
    }
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
  
  for(uint32_t i=0; i<temp_instr.size(); ++i) {
    for(uint32_t j=0; j<4; ++j) {
      MemoryEntry new_entry = {(i*4)+j, (uint8_t)((temp_instr.at(i)>>(j*8))&0xff)};
      block.push_back(new_entry);
    }
  }
  
  // for(uint32_t i=0;i<temp_instr.size();++i) {
  //   for(int j=0; j<4; ++j) {
  //     MemoryEntry new_entry = {i+j, (int8_t)(temp_instr.at(i)>>j)&0xff};
  //     block.push_back(new_entry);
  //   }
  // }
}


/**
 * Print address: memory data for data stored in memory
 * 
**/
void Memory::print()
{
  cout << "----------- Memory -----------" << endl;
  for(int i=0; i<block.size(); i+=4) {

    cout << hex << block.at(i).address << "\t: " << "0x" << std::setfill ('0') << std::setw(2)  \
      << hex << static_cast<int>((uint8_t)block.at(i+3).data) << std::setfill ('0') << std::setw(2) \
      << hex << static_cast<int>((uint8_t)block.at(i+2).data) << std::setfill ('0') << std::setw(2) \
      << hex << static_cast<int>((uint8_t)block.at(i+1).data) << std::setfill ('0') << std::setw(2) \
      << hex << static_cast<int>((uint8_t)block.at(i).data) << endl;
  }
  cout << "------------------------------" << endl;
}

void Memory::print_bytes()
{
  cout << "----------- Memory -----------" << endl;
  for(int i=0; i<block.size(); ++i) {
    cout << hex << block.at(i).address << "\t: " << "0x" << std::setfill ('0') << std::setw(2) << hex << static_cast<int>((uint8_t)block.at(i).data) << endl;
  }
  cout << "------------------------------" << endl;
}

/**
 * Write to an address in memory
 * 
 * @param address to write to, data to save
**/
void Memory::write(uint32_t addr, int32_t data, int bytes)
{
  // see if address already in memory
  bool address_found {false};
  for(int i=0; i<block.size(); ++i) {
    if (block.at(i).address == addr) {
      for(int l=0; l<bytes; ++l) {
        block.at(i+l).data = (uint8_t)((data>>(l*8))&0xff);
      }
      address_found = true;
    }
  }
  if(!address_found) {
    for(int j=0; j<bytes; ++j) {
      MemoryEntry new_entry = {addr+j, (uint8_t)((data>>(j*8))&0xff)};
      block.push_back(new_entry);
    }
    // fill unaligned bytes with zeroes
    for(int k=bytes; k<4; ++k) {
      for(auto e: block) {
        if(e.address == addr+k) address_found = true;
      }
      if(!address_found) {
        MemoryEntry new_entry = {addr+k, 0x00};
        block.push_back(new_entry);
      }
    }
  }
}


/**
 * Read from an address in memory
 * 
 * @param addr start address to read from
 * @param bytes number of consecutive bytes to read from start address
**/
int32_t Memory::read(uint32_t addr, int bytes)
{
  int32_t ret_temp = 0;
  for(int i=0; i<block.size(); ++i) {
    if(block[i].address == addr) {        // start address found
      for(int j=0; j<bytes; ++j) {        // read x bytes from address
        try {
          ret_temp |= (block[i+j].data<<(j*8));
        } catch(std::out_of_range) {
          ret_temp |= (0x00<<(j*8));
        }
      }
      return ret_temp;
    }
  }
  
  // start address NOT found
  for(int k=1; k<bytes; ++k) {
    for(int l=0; l<block.size(); ++l) {
      if(block[l].address == addr+k) {
        ret_temp |= (block[l].data<<(k*8));
      }
    }
  }
  return ret_temp;
    
  // cout << "Invalid address access" << endl;
  // // exit(0);
  // return 0;
}