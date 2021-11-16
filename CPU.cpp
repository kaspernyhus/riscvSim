#include "CPU.h"

/**
 * CPU constructor
 * @param reference to external memory where instructions and live
**/
CPU::CPU(Memory& mem): memory(mem)
{
  current_pc = 0;
  next_pc = 0;
  debug = false;
  count_cycles = 0;
}


/**
 * Increments PC by 4
**/
uint32_t CPU::_next() 
{
  current_pc = next_pc;
  next_pc += 4;
  return current_pc;
}


/**
 * Sets PC to value
 * @param new_pc unsigned int
 * @exception exit() if new_pc < 0
**/
void CPU::_set_pc(uint32_t new_pc) 
{ 
  if(next_pc<0) {
    cout << "pc < 0" << endl;
    exit(0);
  }
  next_pc = new_pc;
};


/**
 * Sets PC to value relative to current PC
 * @param pc_offset signed 32bit int
 * @exception exit() updated PC < 0
**/
void CPU::_set_pc_relative(int32_t pc_offset) 
{
  current_pc += pc_offset;
  
  if(current_pc<0) {
    cout << "pc < 0" << endl;
    exit(0);
  }

  next_pc = current_pc;
};


/**
 * Steps through program
 * 
 * @return termination flag: true=exit, false=continue
**/
bool CPU::step()
{
  count_cycles++; // count how many cycles executed
  
  if(memory.size() == 0) {
    cout << "No instructions in memory" << endl;
    return 1;
  }
  _next();
  // fetch next instruction
  uint32_t instruction = memory.read(current_pc);
  // decode, execute & opdate registers/memory
  bool exit = _decode_n_execute(instruction);
  if(debug) {
    registers.print_all();
  }
  if(exit) {
    return 1;
  }
  else return 0;
}

bool CPU::step(uint16_t steps)
{
  for(int i=0;i<steps;++i) {
    count_cycles++; // count how many cycles executed
    
    if(memory.size() == 0) {
      cout << "No instructions in memory" << endl;
      return 1;
    }
    _next();
    // fetch next instruction
    uint32_t instruction = memory.read(current_pc);
    // decode, execute & opdate registers/memory
    bool exit = _decode_n_execute(instruction);
    if(debug) {
      registers.print_all();
    }
    if(exit) {
      return 1;
    }
  }
  return 1;
}

void CPU::debug_on() 
{
  debug=1;
}

void CPU::debug_off() 
{
  debug=0;
}

void CPU::dump()
{
  print_registers();
  registers.dump_to_files();
}

/**
 * Decodes, executes and updates registers and memory
 * 
 * @param instruction unsigned 32-bit instruction
 * 
**/
bool CPU::_decode_n_execute(uint32_t instruction)
{ 
  /* Decode instruction */
  uint8_t opcode = instruction & 0x7f;
  uint8_t funct3 = (instruction >> 12) & 0x07;
  uint8_t funct7 = (instruction >> 25) & 0x7f;
  uint8_t rd = (instruction >> 7) & 0x1f;
  uint8_t rs1 = (instruction >> 15) & 0x1f;
  uint8_t rs2 = (instruction >> 20) & 0x1f;
  int32_t imm11_0 = signextend(11,(instruction >> 20));
  int32_t imm31_12 = instruction & 0xfffff000;
  int32_t store_offset = calc_store_offset(instruction);
  int32_t branch_offset = calc_branch_offset(instruction);
  int32_t jal_offset = calc_jal_offset(instruction);
  int32_t a0 = registers.read(10);
  bitset<5> a0_bin(a0);


  /* Execute */
  if(debug) {
    cout << "------------ " << "PC: " << hex << current_pc << " --------------" << endl;
    cout << "Next PC: " << next_pc << endl;
    bitset<32> instr_bin(instruction);
    cout << "Instr:      " << "0x" << setfill ('0') << setw(8) << hex << instruction << endl;
    bitset<7> opcode_bin(opcode);
    cout << "Opcode:" << "\t       " << opcode_bin << endl;
    bitset<3> funct3_bin(funct3);
    cout << "funct3:" << "\t           " << funct3_bin << endl;
    bitset<7> funct7_bin(funct7);
    cout << "funct7:" << "\t       " << funct7_bin << endl;
    bitset<5> rd_bin(rd);
    cout << "rd:"  << "\t         " << rd_bin << "\t" << dec << (int)rd << endl;
    bitset<5> rs1_bin(rs1);
    cout << "rs1:" << "\t         " << rs1_bin << "\t" << (int)rs1 << endl;
    bitset<5> rs2_bin(rs2);
    cout << "rs2:" << "\t         " << rs2_bin << "\t" << (int)rs2 << endl;
    bitset<12> imm11_bin(imm11_0);
    cout << "imm11_0:  " << imm11_bin << "\t" << (int)imm11_0 << endl;
    cout << "imm31_12: " << "\t" << imm31_12 << endl;
    bitset<12> b_offset_bin(branch_offset);
    cout << "branch_offset:  " << b_offset_bin << "\t" << branch_offset << endl;
    bitset<20> j_offset_bin(jal_offset);
    cout << "jal_offset:  " << j_offset_bin << "\t" << jal_offset << endl;
    bitset<12> s_offset_bin(store_offset);
    cout << "store_offset:  " << s_offset_bin << "\t" << store_offset << endl;
  }


  /* Decode, execute and update registers */

  switch (opcode) {
    case 0b0110011:       
      switch (funct3) {
        case 0b000:
          switch (funct7) {
            case 0b0000000:
              if(debug) cout << "add" << endl;
              registers.write(rd,registers.read(rs1)+registers.read(rs2));
              break;
            case 0b0100000:
              if(debug) cout << "sub" << endl;
              registers.write(rd,registers.read(rs1)-registers.read(rs2));
              break;
            default:
              cout << "Funct7 error" << endl;
              break;
          }
          break;
        case 0b001:
          if(debug) cout << "sll" << endl;
          registers.write(rd,registers.read(rs1)<<registers.read(rs2));
          break;
        case 0b010:
          if(debug) cout << "slt" << endl;
          if(registers.read(rs1)<registers.read(rs2)) registers.write(rd,1);
          else registers.write(rd,0);
          break;
        case 0b011:
          if(debug) cout << "sltu" << endl;
          if((uint32_t)registers.read(rs1)<(uint32_t)registers.read(rs2)) registers.write(rd,1);
          else registers.write(rd,0);
          break;

        case 0b100:
          if(debug) cout << "xor" << endl;
          registers.write(rd,registers.read(rs1)^registers.read(rs2));
          break;
        case 0b101:
          switch (funct7) {
            case 0b0000000:
              if(debug) cout << "srl" << endl;
              registers.write(rd,(uint32_t)registers.read(rs1)>>registers.read(rs2));
              break;
            case 0b0100000:
              if(debug) cout << "sra" << endl;
              registers.write(rd,registers.read(rs1)>>registers.read(rs2));
              break;
            default:
              cout << "Funct7 error" << endl;
              break;
          }
          break;
        case 0b110:
          if(debug) cout << "or" << endl;
          registers.write(rd,registers.read(rs1)|registers.read(rs2));
          break;
        case 0b111:
          if(debug) cout << "and" << endl;
          registers.write(rd,registers.read(rs1)&registers.read(rs2));
          break;
        default:
          cout << "Funct3 error" << endl;
          break;
      }
      break;
    
    case 0b0000011:
      switch (funct3)
      {
      case 0b000:
        if(debug) cout << "lb" << endl;
        registers.write(rd,signextend(7,memory.read(registers.read(rs1)+imm11_0,0x000000ff)));
        break;
      case 0b001:
        if(debug) cout << "lh" << endl;
        registers.write(rd,signextend(15,memory.read(registers.read(rs1)+imm11_0,0x0000ffff)));
        break;
      case 0b010:
        if(debug) cout << "lw" << endl;
        registers.write(rd,memory.read(registers.read(rs1)+imm11_0));
        break;
      case 0b011:
        if(debug) cout << "ld" << endl;
        cout << "not part of RV32I" << endl;
        break;
      case 0b100:
        if(debug) cout << "lbu" << endl;
        registers.write(rd,(uint32_t)memory.read(registers.read(rs1)+imm11_0,0x000000ff));
        break;
      case 0b101:
        if(debug) cout << "lhu" << endl;
        registers.write(rd,(uint32_t)memory.read(registers.read(rs1)+imm11_0,0x0000ffff));
        break;
      case 0b110:
        if(debug) cout << "lwu" << endl;
        cout << "not part of RV32I" << endl;
        break;
      default:
        cout << "Funct3 error" << endl;
        break;
      }
      break;

    case 0b0010011:
      switch (funct3) {
        case 0b000:
            if(debug) cout << "addi" << endl;
            registers.write(rd,registers.read(rs1)+imm11_0);
            break;
        case 0b001:
            if(debug) cout << "slli" << endl;
            registers.write(rd,registers.read(rs1)<<imm11_0);
            break;
        case 0b010:
            if(debug) cout << "slti" << endl;
            if(registers.read(rs1)<imm11_0) registers.write(rd,1);
            else registers.write(rd,0);
            break;
        case 0b011:
            if(debug) cout << "sltiu" << endl;
            if((uint32_t)registers.read(rs1)<imm11_0) registers.write(rd,1);
            else registers.write(rd,0);
            break;
        case 0b100:
            if(debug) cout << "xori" << endl;
            registers.write(rd,registers.read(rs1)^imm11_0);
            break;
        case 0b101:
            switch (funct7) {
                case 0b0000000:
                    if(debug) cout << "srli" << endl;
                    registers.write(rd,(uint32_t)registers.read(rs1)>>imm11_0);
                    break;
                case 0b0100000:
                    if(debug) cout << "srai" << endl;
                    registers.write(rd,registers.read(rs1)>>imm11_0);
                    break;
                default:
                    cout << "Funct7 error" << endl;
                    break;
            }
            break;
        case 0b110:
            if(debug) cout << "ori" << endl;
            registers.write(rd,registers.read(rs1)|imm11_0);
            break;
        case 0b111:
            if(debug) cout << "andi" << endl;
            registers.write(rd,registers.read(rs1)&imm11_0);
            break;
        default:
            cout << "Funct3 error" << endl;
            break;
      }
      break;
    
    case 0b1100111:
      if(debug) cout << "jalr" << endl;
      registers.write(rd, next_pc);
      _set_pc(registers.read(rs1)+imm11_0);
      break;

    case 0b0100011:
      switch (funct3) {
      case 0b000:
        if(debug) cout << "sb" << endl;
        memory.write(registers.read(rs1)+store_offset,registers.read(rs2));
        break;
      case 0b001:
        if(debug) cout << "sh" << endl;
        memory.write(registers.read(rs1)+store_offset,registers.read(rs2));
        break;
      case 0b010:
        if(debug) cout << "sw" << endl;
        memory.write(registers.read(rs1)+store_offset,registers.read(rs2));
        break;
      case 0b111:
        if(debug) cout << "sd" << endl;
        memory.write(registers.read(rs1)+store_offset,registers.read(rs2));
        break;
      default:
        cout << "Funct3 error" << endl;
        break;
      }
      break;
      
    case 0b1100011:
      switch (funct3) {
      case 0b000:
        if(debug) cout << "beq" << endl;
        if(registers.read(rs1) == registers.read(rs2)) _set_pc_relative(branch_offset);
        break;
      case 0b001:
        if(debug) cout << "bne" << endl;
        if(registers.read(rs1) != registers.read(rs2)) _set_pc_relative(branch_offset);
        break;
      case 0b100:
        if(debug) cout << "blt" << endl;
        if(registers.read(rs1) < registers.read(rs2)) _set_pc_relative(branch_offset);
        break;
      case 0b101:
        if(debug) cout << "bge" << endl;
        if(registers.read(rs1) >= registers.read(rs2)) _set_pc_relative(branch_offset);
        break;
      case 0b110:
        if(debug) cout << "bltu" << endl;
        if((uint32_t)registers.read(rs1) < (uint32_t)registers.read(rs2)) _set_pc_relative(branch_offset);
        break;
      case 0b111:
        if(debug) cout << "bgeu" << endl;
        if((uint32_t)registers.read(rs1) >= (uint32_t)registers.read(rs2)) _set_pc_relative(branch_offset);
        break;
      
      default:
        cout << "Funct3 error" << endl;
        break;
      }
      break;

    case 0b0110111:
      if(debug) cout << "lui" << endl;
      registers.write(rd,imm31_12);
      break;
    
    case 0b0010111:
      if(debug) cout << "auipc" << endl;
      registers.write(rd,current_pc+imm31_12);
      break;

    case 0b1101111:
      if(debug) cout << "jal" << endl;
      registers.write(rd, next_pc);
      _set_pc_relative(jal_offset);
      break;
  

    /*
    a7	  a0	                  Name	              Description
    1	    (integer to print)	  print_int	          Prints the value located in a0 as a signed integer
    2	    (float to print)	    print_float	        Prints the value located in a0 as a floating point number
    4	    (pointer to string)	  print_string	      Prints the null-terminated string located at address in a0
    10	  -	                    exit	              Halts the simulator
    11	  (char to print)	      print_char	        Prints the value located in a0 as an ASCII character
    34	  (integer to print)	  print_hex	          Prints the value located in a0 as a hex number
    35	  (integer to print)	  print_bin	          Prints the value located in a0 as a binary number
    36	  (integer to print)	  print_unsigned	    Prints the value located in a0 as an unsigned integer
    93	  (status code)	exit	  Halts the simulator and exits with status code in a0
    */
    case 0b1110011:
      if(debug) cout << "\nExit by Ecall" << endl;
      cout << "\nConsole output:" << endl;
      cout << ">> ";
      switch (registers.read(17))
      {
        case 1:
          cout << a0 << endl;
          break;
        case 2:
          cout << (float)a0 << endl;
          break;
        case 4:
          cout << to_string(a0) << endl;
          break;
        case 10:
          break;
        case 11:
          cout << (char)a0 << endl;
          break;
        case 34:
          cout << "0x" << setfill ('0') << setw(4) << hex << a0 << endl;
          break;
        case 35:
          cout << "0b" << setfill ('0') << setw(32) << a0_bin << endl;
          break;
        case 36:
          cout << (uint32_t)a0 << endl;
          break;
        case 93:
          break;
        
        default:
          cout << "Unknown ecall" << endl;
          break;
      }
      return 1;
      break;

    case 0b00000000:
      cout << "\nERROR:\nEmpty instruction - terminating program" << endl;
      return 1;
      break;

    default:
      cout << "Opcode error" << endl;
      break;
  }
  return 0;
}