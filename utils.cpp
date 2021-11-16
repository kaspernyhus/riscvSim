#include "utils.h"

/**
 * Signextends a 32bit value
 * 
**/
int32_t signextend(uint8_t signbit_num, int32_t value) 
{  
  return (value << (32 - signbit_num)) >> (32 - signbit_num);
}


/**
 * Calculates the 12-bit signed branch offset
 * Used by:
 *  JALR
*/
int32_t calc_branch_offset(uint32_t instruction) 
{
  int32_t branch_offset = signextend(12,((instruction >> 19) & 0x1000) | ((instruction << 4) & 0x0800) | ((instruction >> 20) & 0x07e0) | ((instruction >> 7) & 0x001e));
  return branch_offset;
}


/**
 * Calculates the 20-bit signed jal offset
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
  int32_t store_offset = signextend(12,((instruction >> 20) & 0xfe0) | ((instruction >> 7) & 0x1f));
  return store_offset;
}