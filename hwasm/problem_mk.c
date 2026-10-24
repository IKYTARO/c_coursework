#include <assert.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define IMMEDIATE_MASK 0x7f
#define MNEMONIC_MASK 0x70
#define MAJOR_REGISTER_MASK 0x0c
#define MINOR_REGISTER_MASK 0x03
#define REGISTER_COUNT 4
#define MODULO_MASK 0xff

// clang-format off

/******************************************************************************
 * ЗАДАЧА: Problem MK — эмулятор
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
 *   Вам приходит закодированная программа в stdin, последней командой в программе
 *   всегда будет не валидная для данного в задаче набора инструкций команда 0xf0.
 *
 *   Далее, также на stdin, поступят аргументы для данной программы.
 *
 *   Вам необходимо симулировать работу программы и вывести всё что она выводит
 *   на stdout.
 *
 * ПРИМЕРЫ:
 *   Вход:  0x70 0xc7 0xc1 0x87 0x27 0xc5 0x8d 0xc1 0x87 0x6f 0xc5 0xc7 0xf0
 *          104 64
 *   Выход: 112 216 63 111
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

static void execute_instruction(instruction_type instruction, unsigned *state) {
  switch (instruction.mnemonic) {
    case MOVI:
      state[D] = instruction.val1;
      break;
    case ADD:
      state[instruction.val1] = (state[instruction.val1] + state[instruction.val2]) & MODULO_MASK;
      break;
    case SUB:
      state[instruction.val1] = (state[instruction.val1] - state[instruction.val2]) & MODULO_MASK;
      break;
    case MUL:
      state[instruction.val1] = (state[instruction.val1] * state[instruction.val2]) & MODULO_MASK;
      break;
    case DIV:
      state[instruction.val1] = (state[instruction.val1] / state[instruction.val2]) & MODULO_MASK;
      break;
    case IN: {
      unsigned val;
      scanf("%u", &val);
      state[instruction.val1] = val;
    } break;
    case OUT:
      printf("%u\n", state[instruction.val1]);
      break;
    default:
      break;
  }
}

static void test_make_instruction() {
  static char *mnemonics[8]   = {"MOVI", "ADD", "SUB", "MUL", "DIV", "IN", "OUT", "ILLEGAL"};
  unsigned upcodes[]          = {0x7f, 0x88, 0x99, 0xaa, 0xbb, 0xc0, 0xc4, 0xff};
  instruction_type expected[] = {{MOVI, 127, 0}, {ADD, C, A}, {SUB, C, B}, {MUL, C, C},
                                 {DIV, C, D},    {IN, A, 0},  {OUT, A, 0}, {ILLEGAL, 0, 0}};
  for (unsigned i = 0; i < sizeof(upcodes) / sizeof(unsigned); i++) {
    bool failed             = false;
    instruction_type actual = make_instruction(upcodes[i]);
    if (actual.mnemonic != expected[i].mnemonic) {
      failed = true;
      printf("--- Test #%d FAILED:\n", i);
      printf("    --- mnemonic extraction error: actual = %s, expected = %s\n",
             mnemonics[actual.mnemonic], mnemonics[expected[i].mnemonic]);
    }
    if (actual.val1 != expected[i].val1) {
      if (!failed) {
        failed = true;
        printf("--- Test #%d FAILED:\n", i);
      }
      printf("    --- major value extraction error: actual = %u, expected = %u\n", actual.val1,
             expected[i].val1);
    }
    if (actual.val2 != expected[i].val2) {
      if (!failed) {
        failed = true;
        printf("--- Test #%d FAILED:\n", i);
      }
      printf("    --- minor value extraction error: actual = %u, expected = %u\n", actual.val2,
             expected[i].val2);
    }
  }
}

int main() {
  unsigned *state = (unsigned *)calloc(REGISTER_COUNT, sizeof(unsigned));
  unsigned upcode;
  while (scanf("%x", &upcode) == 1) {
    instruction_type instruction = make_instruction(upcode);
    execute_instruction(instruction, state);
  }
  free(state);
  return EXIT_SUCCESS;
}
