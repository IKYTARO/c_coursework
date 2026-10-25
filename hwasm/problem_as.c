#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef enum { MOVI, ADD, SUB, MUL, DIV, IN, OUT, ILLEGAL } mnemonic_type;

typedef struct {
  mnemonic_type mnemonic;
  unsigned imm;
  char r1, r2;
} instruction_type;

static bool is_valid_register(char r) {
  return r >= 'A' && r <= 'D';
}

static unsigned register_code(char r) {
  switch (r) {
    case 'A':
      return 0;
    case 'B':
      return 1;
    case 'C':
      return 2;
    case 'D':
      return 3;
    default:
      return 0;
  }
}

static bool parse_line(const char *line, instruction_type *ins) {
  unsigned imm;
  char r1, r2;

  if (sscanf(line, "MOVI %u", &imm) == 1) {
    if (imm > 0x7f) {
      return false;
    }
    *ins = (instruction_type){.mnemonic = MOVI, .imm = imm};
    return true;
  }
  if (sscanf(line, "ADD %c, %c", &r1, &r2) == 2) {
    if (!is_valid_register(r1) || !is_valid_register(r2)) {
      return false;
    }
    *ins = (instruction_type){.mnemonic = ADD, .r1 = r1, .r2 = r2};
    return true;
  }
  if (sscanf(line, "SUB %c, %c", &r1, &r2) == 2) {
    if (!is_valid_register(r1) || !is_valid_register(r2)) {
      return false;
    }
    *ins = (instruction_type){.mnemonic = SUB, .r1 = r1, .r2 = r2};
    return true;
  }
  if (sscanf(line, "MUL %c, %c", &r1, &r2) == 2) {
    if (!is_valid_register(r1) || !is_valid_register(r2)) {
      return false;
    }
    *ins = (instruction_type){.mnemonic = MUL, .r1 = r1, .r2 = r2};
    return true;
  }
  if (sscanf(line, "DIV %c, %c", &r1, &r2) == 2) {
    if (!is_valid_register(r1) || !is_valid_register(r2)) {
      return false;
    }
    *ins = (instruction_type){.mnemonic = DIV, .r1 = r1, .r2 = r2};
    return true;
  }
  if (sscanf(line, "IN %c", &r1) == 1) {
    if (!is_valid_register(r1)) {
      return false;
    }
    *ins = (instruction_type){.mnemonic = IN, .r1 = r1};
    return true;
  }
  if (sscanf(line, "OUT %c", &r1) == 1) {
    if (!is_valid_register(r1)) {
      return false;
    }
    *ins = (instruction_type){.mnemonic = OUT, .r1 = r1};
    return true;
  }
  return false;
}

static unsigned encode(const instruction_type *ins) {
  switch (ins->mnemonic) {
    case MOVI:
      return ins->imm;
    case ADD:
      return 0x80 | (register_code(ins->r1) << 2) | register_code(ins->r2);
    case SUB:
      return 0x90 | (register_code(ins->r1) << 2) | register_code(ins->r2);
    case MUL:
      return 0xa0 | (register_code(ins->r1) << 2) | register_code(ins->r2);
    case DIV:
      return 0xb0 | (register_code(ins->r1) << 2) | register_code(ins->r2);
    case IN:
      return 0xc0 | register_code(ins->r1);
    case OUT:
      return 0xc4 | register_code(ins->r1);
    default:
      return 0;
  }
}

int main() {
  char line[LINE_MAX_SIZE];
  while (fgets(line, sizeof line, stdin)) {
    if (line[0] == '\n') {
      continue;
    }
    instruction_type ins;
    if (!parse_line(line, &ins)) {
      printf("ERROR\n");
      break;
    }
    printf("0x%x ", encode(&ins));
  }
  return EXIT_SUCCESS;
}