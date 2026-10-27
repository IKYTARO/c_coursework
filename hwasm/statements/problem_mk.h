#ifndef PROBLEM_MK_H
#define PROBLEM_MK_H

#include <stdint.h>

enum reg_t { A = 0, B, C, D, RLAST };

enum opcode_t { MOVI = 0, ADD = 8, SUB = 9, MUL = 10, DIV = 11, IN, OUT, OPLAST };

union operand_t {
  struct {
    enum reg_t rd, rs;
  } ops;
  enum reg_t rop;
  int imm;
};

struct instr_t {
  enum opcode_t opcode;
  union operand_t opnd;
};

struct cpu_t {
  uint8_t regs[RLAST];
};

#endif  // PROBLEM_MK_H