# Risc-v ISA simulator
---
Simulates a risc-v instruction set architecture

How it works:
A .bin file containing instructions is loaded into the simulated memory.
To load a .bin file change the path in `memory.load_instructions("tests/task3/loop.bin");`in main.cpp and recompile.

Compiler: g++

To build:
- run `make`
- run `./riscvSim`to execute

