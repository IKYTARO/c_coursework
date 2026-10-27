#include "statements/problem_mk.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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
  if (cmd & 0x40) {
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

static const char *mnemonic_name(struct instr_t instr) {
  switch (instr.opcode) {
    case MOVI:
      return "MOVI";
    case ADD:
      return "ADD";
    case SUB:
      return "SUB";
    case MUL:
      return "MUL";
    case DIV:
      return "DIV";
    case IN:
      return "IN";
    case OUT:
      return "OUT";
    default:
      return "ILLEGAL";
  }
}

static bool execute_instruction(struct instr_t const *instr, struct cpu_t *cpu, FILE *in,
                                FILE *out) {
  switch (instr->opcode) {
    case MOVI:
      cpu->regs[D] = instr->opnd.imm;
      return true;
    case ADD:
      cpu->regs[instr->opnd.ops.rd] += cpu->regs[instr->opnd.ops.rs];
      return true;
    case SUB:
      cpu->regs[instr->opnd.ops.rd] -= cpu->regs[instr->opnd.ops.rs];
      return true;
    case MUL:
      cpu->regs[instr->opnd.ops.rd] *= cpu->regs[instr->opnd.ops.rs];
      return true;
    case DIV:
      if (cpu->regs[instr->opnd.ops.rs] == 0) {
        return false;
      }
      cpu->regs[instr->opnd.ops.rd] /= cpu->regs[instr->opnd.ops.rs];
      return true;
    case IN: {
      unsigned input;
      if (fscanf(in, "%u", &input) != 1) {
        return false;
      }
      cpu->regs[instr->opnd.rop] = (uint8_t)input;
      return true;
    }
    case OUT:
      fprintf(out, "%u\n", cpu->regs[instr->opnd.rop]);
      return true;
    default:
      return false;
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <program.enc>\n", argv[0]);
    return EXIT_FAILURE;
  }

  FILE *prog = fopen(argv[1], "r");
  if (!prog) {
    fprintf(stderr, "Error opening file: %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  struct cpu_t cpu = {.regs = {0}};
  unsigned upcode;
  while (fscanf(prog, "%x", &upcode) == 1) {
    if (upcode > 0xFF) {
      fprintf(stderr, "Error read byte: 0x%x\n", upcode);
      break;
    }
    struct instr_t instr = decode_command((unsigned char)upcode);
    if (!execute_instruction(&instr, &cpu, stdin, stdout)) {
      fprintf(stderr, "Error execute instruction: %s (0x%x)\n", mnemonic_name(instr), upcode);
      break;
    }
  }
  fclose(prog);
  return EXIT_SUCCESS;
}
