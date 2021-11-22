#ifndef MEMORY_H_
#define MEMORY_H_

#include <iostream>
#include <fstream>
#include <vector>



using namespace std;


struct MemoryEntry {
  uint32_t address;
  uint8_t data;
};


class Memory {
  private:
    vector<MemoryEntry> block;
  public:
    void load_instructions(vector<uint32_t> instructions);
    void load_instructions(const char* filename);
    void write(uint32_t addr, int32_t data, int bytes=4);
    int32_t read(uint32_t addr, int bytes=4);
    void print();
    void print_bytes();
    size_t size() {return block.size();};
};

#endif