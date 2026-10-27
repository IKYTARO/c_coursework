#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "statements/problem_mk.h"

// clang-format off

/******************************************************************************
 * ЗАДАЧА: Problem AS2 — декодер
 *
 * ОПИСАНИЕ:
 *   Регистровый файл состоит из четырёх восьмиразрядных регистров:
 *
 *   - A : 00
 *   - B : 01
 *   - C : 10
 *   - D : 11
 *
 *   Вам задана следующая система команд и таблица мнемоник:
 *
 *   - MOVI Imm (move constant to D register)	  : 0IIIIIII
 *   - ADD R, R (add two registers modulo 256)	: 1000RRRR
 *   - SUB R, R (sub two registers modulo 256)	: 1001RRRR
 *   - MUL R, R (mul two registers modulo 256)	: 1010RRRR
 *   - DIV R, R (div two registers modulo 256)	: 1011RRRR
 *   - IN R     (read register R from keyboard)	: 110000RR
 *   - OUT R    (write register R to screen)    : 110001RR
 *
 *   Вам необходимо распечатать hex ввод как текст программы.
 *   В случае ошибки в одной из команд, все предыдущие должны быть распечатаны верно.
 *
 * ПРИМЕРЫ:
 *   Вход:  0x70 0xc7 0xc1 0x87
 *   Выход: MOVI 112
 *          OUT D
 *          IN B
 *          ADD B, D
 ******************************************************************************/

// clang-format on

static enum reg_t reg_from_code(unsigned code) {
  return (enum reg_t)code;
}

struct instr_t decode_command(unsigned char cmd) {
  struct instr_t instr = {.opcode = OPLAST, .opnd.imm = 0};

  // MOVI: 0IIIIIII
  if ((cmd >> 7) == 0) {
    instr.opcode   = MOVI;
    instr.opnd.imm = cmd & 0x7F;
    return instr;
  }

  // IN/OUT: 110000RR / 110001RR
  if (((cmd >> 6) & 1u) == 1) {
    unsigned hi = cmd >> 2;
    if (hi == 0x30) {
      instr.opcode = IN;
    } else if (hi == 0x31) {
      instr.opcode = OUT;
    } else {
      return instr;
    }
    instr.opnd.rop = reg_from_code(cmd & 0x3);
    return instr;
  }

  // ADD/SUB/MUL/DIV: 10MMRRRR
  switch (cmd >> 4) {
    case ADD:
    case SUB:
    case MUL:
    case DIV:
      instr.opcode = (enum opcode_t)(cmd >> 4);
      break;
    default:
      return instr;
  }
  instr.opnd.ops.rd = reg_from_code((cmd >> 2) & 0x3);
  instr.opnd.ops.rs = reg_from_code(cmd & 0x3);
  return instr;
}

static const char *const regnames[]   = {"A", "B", "C", "D"};
static const char *const arithnames[] = {"ADD", "SUB", "MUL", "DIV"};

bool print_instr(struct instr_t instr) {
  switch (instr.opcode) {
    case MOVI:
      printf("MOVI %d\n", instr.opnd.imm);
      break;
    case ADD:
    case SUB:
    case MUL:
    case DIV:
      printf("%s %s, %s\n", arithnames[instr.opcode - ADD], regnames[instr.opnd.ops.rd],
             regnames[instr.opnd.ops.rs]);
      break;
    case IN:
      printf("IN %s\n", regnames[instr.opnd.rop]);
      break;
    case OUT:
      printf("OUT %s\n", regnames[instr.opnd.rop]);
      break;
    default:
      return false;
  }
  return true;
}

int main(void) {
  unsigned command;
  while (scanf("%x", &command) == 1) {
    if (command > 0xFFu) {
      printf("ERROR\n");
      break;
    }
    struct instr_t instr = decode_command((unsigned char)command);
    if (!print_instr(instr)) {
      printf("ERROR\n");
      break;
    }
  }
  return EXIT_SUCCESS;
}