#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "statements/problem_mk.h"

#define LINE_MAX_SIZE 128

// clang-format off

/******************************************************************************
 * ЗАДАЧА: Problem AS — кодировщик
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
 *   Вам необходимо закодировать в hex пришедшие вам на stdin мнемоники.
 *   В случае ошибки в одной из них, все предыдущие должны быть закодированы верно.
 *
 * ПРИМЕРЫ:
 *   Вход:  MOVI 112
 *          OUT D
 *          IN B
 *          ADD B, D
 *   Выход: 0x70 0xc7 0xc1 0x87
 ******************************************************************************/

// clang-format on

static bool is_valid_register(char r) {
  return r >= 'A' && r <= 'D';
}

static enum reg_t reg_from_char(char r) {
  return (enum reg_t)(r - 'A');
}

static bool parse_line(const char *line, struct instr_t *instr) {
  unsigned imm;
  char r1, r2;

  if (sscanf(line, "MOVI %u", &imm) == 1) {
    if (imm > 0x7F)
      return false;
    *instr = (struct instr_t){.opcode = MOVI, .opnd.imm = (int)imm};
    return true;
  }
  if (sscanf(line, "ADD %c, %c", &r1, &r2) == 2) {
    if (!is_valid_register(r1) || !is_valid_register(r2))
      return false;
    *instr = (struct instr_t){.opcode   = ADD,
                              .opnd.ops = {.rd = reg_from_char(r1), .rs = reg_from_char(r2)}};
    return true;
  }
  if (sscanf(line, "SUB %c, %c", &r1, &r2) == 2) {
    if (!is_valid_register(r1) || !is_valid_register(r2))
      return false;
    *instr = (struct instr_t){.opcode   = SUB,
                              .opnd.ops = {.rd = reg_from_char(r1), .rs = reg_from_char(r2)}};
    return true;
  }
  if (sscanf(line, "MUL %c, %c", &r1, &r2) == 2) {
    if (!is_valid_register(r1) || !is_valid_register(r2))
      return false;
    *instr = (struct instr_t){.opcode   = MUL,
                              .opnd.ops = {.rd = reg_from_char(r1), .rs = reg_from_char(r2)}};
    return true;
  }
  if (sscanf(line, "DIV %c, %c", &r1, &r2) == 2) {
    if (!is_valid_register(r1) || !is_valid_register(r2))
      return false;
    *instr = (struct instr_t){.opcode   = DIV,
                              .opnd.ops = {.rd = reg_from_char(r1), .rs = reg_from_char(r2)}};
    return true;
  }
  if (sscanf(line, "IN %c", &r1) == 1) {
    if (!is_valid_register(r1))
      return false;
    *instr = (struct instr_t){.opcode = IN, .opnd.rop = reg_from_char(r1)};
    return true;
  }
  if (sscanf(line, "OUT %c", &r1) == 1) {
    if (!is_valid_register(r1))
      return false;
    *instr = (struct instr_t){.opcode = OUT, .opnd.rop = reg_from_char(r1)};
    return true;
  }
  return false;
}

static unsigned encode(const struct instr_t *instr) {
  switch (instr->opcode) {
    case MOVI:
      return (unsigned)instr->opnd.imm & 0x7F;
    case ADD:
    case SUB:
    case MUL:
    case DIV: {
      unsigned mm = (unsigned)instr->opcode;
      unsigned rd = (unsigned)instr->opnd.ops.rd;
      unsigned rs = (unsigned)instr->opnd.ops.rs;
      return (mm << 4) | (rd << 2) | rs;
    }
    case IN:
      return 0xC0u | (unsigned)instr->opnd.rop;
    case OUT:
      return 0xC4u | (unsigned)instr->opnd.rop;
    default:
      return 0;
  }
}

int main() {
  char line[LINE_MAX_SIZE];
  while (fgets(line, sizeof line, stdin)) {
    if (line[0] == '\n')
      continue;
    struct instr_t instr;
    if (!parse_line(line, &instr)) {
      printf("ERROR\n");
      break;
    }
    printf("0x%x ", encode(&instr));
  }
  return EXIT_SUCCESS;
}