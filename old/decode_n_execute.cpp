#include "decode_n_execute.h"

// 31  30  29  28 | 27  26  25  24 | 23  22  21  20 | 19  18  17  16 | 15  14  13  12 | 11  10  9  8 | 7  6  5  4 | 3  2  1  0


/**
 * Signextends a value
 * 
 * @signbit_num is MSB to extend
 *
 * val =   0 0 0 0 0 1 1 0 1
 * signextend(3,val) returns
 *         1 1 1 1 1 1 1 0 1
**/
int32_t signextend(uint8_t signbit_num, int32_t value) {  
  if(value>>signbit_num) { // negative number
    return (((~value)>>signbit_num)<<signbit_num)|value;
  }
  else return value;
}


/**
 * Calculates the 12-bit signed branch offset
 * Used by:
 *  JALR
*/
int32_t calc_branch_offset(uint32_t instruction) 
{
  int32_t branch_offset = signextend(11,((instruction >> 19) & 0x1000) | ((instruction << 4) & 0x0800) | ((instruction >> 20) & 0x07e0) | ((instruction >> 7) & 0x001e));
  return branch_offset;
}

/**
 * Calculates the 21-bit signed jal offset
 * 
*/
int32_t calc_jal_offset(uint32_t instruction) 
{
  int32_t jal_offset = signextend(20,((instruction >> 11) & 0x10000) | (instruction & 0xff000) | ((instruction >> 9) & 0x800) | ((instruction >> 20) & 0x7fe));
  return jal_offset;
}

/**
 * Calculates the 12-bit signed branch offset
 * Used by:
 *  SB
*/
int32_t calc_store_offset(uint32_t instruction)
{
  int32_t store_offset = signextend(11,((instruction >> 20) & 0xfe0) | ((instruction >> 7) & 0x1f));
  return store_offset;
}


/**
 * Function to decode, execute and update registers and memory
 * 
*/
bool decode_execute_update(uint32_t pc, Registers &registers, Memory &memory, Instructions &instructions)
{
  /* Get instruction from memory */
  uint32_t instruction = memory.read(pc,0xffffffff);

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


  if(DEBUG) {
    cout << "------------ " << "pc: " << hex << instructions.get_current_pc() << " --------------" << endl;
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
              if(DEBUG) cout << "add" << endl;
              registers.write(rd,registers.read(rs1)+registers.read(rs2));
              break;
            case 0b0100000:
              if(DEBUG) cout << "sub" << endl;
              registers.write(rd,registers.read(rs1)-registers.read(rs2));
              break;
            default:
              cout << "Funct7 error" << endl;
              break;
          }
          break;
        case 0b001:
          if(DEBUG) cout << "sll" << endl;
          registers.write(rd,registers.read(rs1)<<registers.read(rs2));
          break;
        case 0b010:
          if(DEBUG) cout << "slt" << endl;
          if(registers.read(rs1)<registers.read(rs2)) registers.write(rd,1);
          else registers.write(rd,0);
          break;
        case 0b011:
          if(DEBUG) cout << "sltu" << endl;
          if((uint32_t)registers.read(rs1)<(uint32_t)registers.read(rs2)) registers.write(rd,1);
          else registers.write(rd,0);
          break;

        case 0b100:
          if(DEBUG) cout << "xor" << endl;
          registers.write(rd,registers.read(rs1)^registers.read(rs2));
          break;
        case 0b101:
          switch (funct7) {
            case 0b0000000:
              if(DEBUG) cout << "srl" << endl;
              registers.write(rd,(uint32_t)registers.read(rs1)>>registers.read(rs2));
              break;
            case 0b0100000:
              if(DEBUG) cout << "sra" << endl;
              registers.write(rd,registers.read(rs1)>>registers.read(rs2));
              break;
            default:
              cout << "Funct7 error" << endl;
              break;
          }
          break;
        case 0b110:
          if(DEBUG) cout << "or" << endl;
          registers.write(rd,registers.read(rs1)|registers.read(rs2));
          break;
        case 0b111:
          if(DEBUG) cout << "and" << endl;
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
        cout << "lb" << endl;
        registers.write(rd,signextend(7,memory.read(registers.read(rs1)+imm11_0,0x000000ff)));
        break;
      case 0b001:
        cout << "lh" << endl;
        registers.write(rd,signextend(15,memory.read(registers.read(rs1)+imm11_0,0x0000ffff)));
        break;
      case 0b010:
        cout << "lw" << endl;
        registers.write(rd,memory.read(registers.read(rs1)+imm11_0,0xffffffff));
        break;
      case 0b011:
        cout << "ld" << endl;
        cout << "not part of RV32I" << endl;
        break;
      case 0b100:
        cout << "lbu" << endl;
        registers.write(rd,(uint32_t)memory.read(registers.read(rs1)+imm11_0,0x000000ff));
        break;
      case 0b101:
        cout << "lhu" << endl;
        registers.write(rd,(uint32_t)memory.read(registers.read(rs1)+imm11_0,0x0000ffff));
        break;
      case 0b110:
        cout << "lwu" << endl;
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
            if(DEBUG) cout << "addi" << endl;
            registers.write(rd,registers.read(rs1)+imm11_0);
            break;
        case 0b001:
            if(DEBUG) cout << "slli" << endl;
            registers.write(rd,registers.read(rs1)<<imm11_0);
            break;
        case 0b010:
            if(DEBUG) cout << "slti" << endl;
            if(registers.read(rs1)<imm11_0) registers.write(rd,1);
            else registers.write(rd,0);
            break;
        case 0b011:
            if(DEBUG) cout << "sltiu" << endl;
            if((uint32_t)registers.read(rs1)<imm11_0) registers.write(rd,1);
            else registers.write(rd,0);
            break;
        case 0b100:
            if(DEBUG) cout << "xori" << endl;
            registers.write(rd,registers.read(rs1)^imm11_0);
            break;
        case 0b101:
            switch (funct7) {
                case 0b0000000:
                    if(DEBUG) cout << "srli" << endl;
                    registers.write(rd,(uint32_t)registers.read(rs1)>>imm11_0);
                    break;
                case 0b0100000:
                    if(DEBUG) cout << "srai" << endl;
                    registers.write(rd,registers.read(rs1)>>imm11_0);
                    break;
                default:
                    cout << "Funct7 error" << endl;
                    break;
            }
            break;
        case 0b110:
            if(DEBUG) cout << "ori" << endl;
            registers.write(rd,registers.read(rs1)|imm11_0);
            break;
        case 0b111:
            if(DEBUG) cout << "andi" << endl;
            registers.write(rd,registers.read(rs1)&imm11_0);
            break;
        default:
            cout << "Funct3 error" << endl;
            break;
      }
      break;
    
    case 0b1100111:
      if(DEBUG) cout << "jalr" << endl;
      registers.write(rd, instructions.get_next_pc());
      instructions.set_pc(registers.read(rs1)+imm11_0);
      break;

    case 0b0100011:
      switch (funct3) {
      case 0b000:
        cout << "sb" << endl;
        memory.write(registers.read(rs1)+store_offset,registers.read(rs2));
        break;
      case 0b001:
        cout << "sh" << endl;
        memory.write(registers.read(rs1)+store_offset,registers.read(rs2));
        break;
      case 0b010:
        cout << "sw" << endl;
        memory.write(registers.read(rs1)+store_offset,registers.read(rs2));
        break;
      case 0b111:
        cout << "sd" << endl;
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
        if(DEBUG) cout << "beq" << endl;
        if(registers.read(rs1) == registers.read(rs2)) instructions.set_pc_relative(branch_offset);
        break;
      case 0b001:
        if(DEBUG) cout << "bne" << endl;
        if(registers.read(rs1) != registers.read(rs2)) instructions.set_pc_relative(branch_offset);
        break;
      case 0b100:
        if(DEBUG) cout << "blt" << endl;
        if(registers.read(rs1) < registers.read(rs2)) instructions.set_pc_relative(branch_offset);
        break;
      case 0b101:
        if(DEBUG) cout << "bge" << endl;
        if(registers.read(rs1) >= registers.read(rs2)) instructions.set_pc_relative(branch_offset);
        break;
      case 0b110:
        if(DEBUG) cout << "bltu" << endl;
        if((uint32_t)registers.read(rs1) < (uint32_t)registers.read(rs2)) instructions.set_pc_relative(branch_offset);
        break;
      case 0b111:
        if(DEBUG) cout << "bgeu" << endl;
        if((uint32_t)registers.read(rs1) >= (uint32_t)registers.read(rs2)) instructions.set_pc_relative(branch_offset);
        break;
      
      default:
        cout << "Funct3 error" << endl;
        break;
      }
      break;

    case 0b0110111:
      if(DEBUG) cout << "lui" << endl;
      registers.write(rd,imm31_12);
      break;

    case 0b1101111:
      if(DEBUG) cout << "jal" << endl;
      registers.write(rd, instructions.get_next_pc());
      instructions.set_pc_relative(jal_offset);
      break;
    
    case 0b1110011:
      cout << "Exit by Ecall" << endl;
      return 1;
      break;

    default:
      cout << "Opcode error" << endl;
      break;
  }
  return 0;
}




