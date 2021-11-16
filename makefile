riscvSim: main.o registers.o memory.o CPU.o utils.o
	g++ -std=c++11 main.o registers.o memory.o CPU.o utils.o -o riscvSim

main.o: main.cpp
	g++ -c -std=c++11 main.cpp

registers.o: registers.cpp
	g++ -c -std=c++11 registers.cpp

memory.o: memory.cpp
	g++ -c -std=c++11 memory.cpp

CPU.o: CPU.cpp
	g++ -c -std=c++11 CPU.cpp

utils.o: utils.cpp
	g++ -c -std=c++11 utils.cpp


make clear:
	rm *.o riscvSim