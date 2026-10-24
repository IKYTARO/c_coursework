#include <assert.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define IMMEDIATE_MASK 0x7f
#define MNEMONIC_MASK 0x70
#define MAJOR_REGISTER_MASK 0x0c
#define MINOR_REGISTER_MASK 0x03

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

typedef enum { MOVI, ADD, SUB, MUL, DIV, IN, OUT, ILLEGAL } mnemonic_type;

typedef enum { A, B, C, D } register_type;

typedef struct {
  mnemonic_type mnemonic;
  unsigned val1;
  unsigned val2;
} instruction_type;

static unsigned extract_major_bit(unsigned upcode) {
  return (upcode >> 7) & 1u;
}

static mnemonic_type extract_mnemonic(unsigned upcode) {
  if (extract_major_bit(upcode) == 0) {
    return MOVI;
  }
  unsigned mnemonic_code = (upcode & MNEMONIC_MASK) >> 4;
  switch (mnemonic_code) {
    case 0:
      return ADD;
    case 1:
      return SUB;
    case 2:
      return MUL;
    case 3:
      return DIV;
    case 4: {
      unsigned in_out_bit = (upcode >> 2) & 1u;
      if (in_out_bit == 0) {
        return IN;
      }
      return OUT;
    }
    default:
      return ILLEGAL;
  }
}

static unsigned extract_immediate(unsigned upcode) {
  return upcode & IMMEDIATE_MASK;
}

static unsigned extract_major_register(unsigned upcode) {
  return (upcode & MAJOR_REGISTER_MASK) >> 2;
}

static unsigned extract_minor_register(unsigned upcode) {
  return upcode & MINOR_REGISTER_MASK;
}

static instruction_type make_instruction(unsigned upcode) {
  instruction_type instruction = {.mnemonic = ILLEGAL, .val1 = 0, .val2 = 0};
  instruction.mnemonic         = extract_mnemonic(upcode);
  switch (instruction.mnemonic) {
    case MOVI:
      instruction.val1 = extract_immediate(upcode);
      break;
    case ADD:
    case SUB:
    case MUL:
    case DIV:
      instruction.val1 = extract_major_register(upcode);
      instruction.val2 = extract_minor_register(upcode);
      break;
    case IN:
    case OUT:
      instruction.val1 = extract_minor_register(upcode);
      break;
    default:
      break;
  }
  return instruction;
}

static bool print_instruction(instruction_type instruction) {
  static const char *mnemonics[8] = {"MOVI", "ADD", "SUB", "MUL", "DIV", "IN", "OUT", "ILLEGAL"};
  static const char *registers[4] = {"A", "B", "C", "D"};
  switch (instruction.mnemonic) {
    case MOVI:
      printf("MOVI %u\n", instruction.val1);
      break;
    case ADD:
    case SUB:
    case MUL:
    case DIV:
      printf("%s %s, %s\n", mnemonics[instruction.mnemonic], registers[instruction.val1],
             registers[instruction.val2]);
      break;
    case IN:
    case OUT:
      printf("%s %s\n", mnemonics[instruction.mnemonic], registers[instruction.val1]);
      break;
    default:
      printf("%s", "ERROR");
      break;
  }
  return (instruction.mnemonic != ILLEGAL);
}

int main() {
  unsigned upcode;
  while (scanf("%x", &upcode) == 1) {
    instruction_type instruction = make_instruction(upcode);
    if (print_instruction(instruction) == false) {
      return EXIT_SUCCESS;
    }
  }
  return EXIT_SUCCESS;
}