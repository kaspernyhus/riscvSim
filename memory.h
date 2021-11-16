#ifndef MEMORY_H_
#define MEMORY_H_

#include <iostream>
#include <fstream>
#include <vector>



using namespace std;


struct MemoryEntry {
  uint32_t address;
  int32_t data;
};


class Memory {
  private:
    vector<MemoryEntry> block;
  public:
    void load_instructions(vector<uint32_t> instructions);
    void load_instructions(const char* filename);
    void write(uint32_t addr, int32_t data);
    int32_t read(uint32_t addr, uint32_t bitmask=0xffffffff);
    void print();
    size_t size() {return block.size();};
};

#endif