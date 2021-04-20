#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define INSTRUCTION_COUNT 47
#define IL 3 // Instruction length

typedef enum OpCode
{
    nop,    // nop
    hlf,    // half
    tpl,    // triple
    inc,    // increment
    jmp,    // jump
    jie,    // jump if even
    jio     // jump if one
} OpCode;

typedef enum Register
{
    a,
    b
} Register;

typedef struct Instruction
{
    OpCode op;
    Register reg;
    int off;
} Instruction;

typedef struct Computer
{
    unsigned int *registers;
    Instruction *instructions;
    unsigned int programCounter;
} Computer;

int part1(FILE *in);
int part2(FILE *in);
void run(Computer *computer);
Computer getComputer(FILE *in, unsigned int *registerBuff, Instruction *instructionsBuff);

int main()
{
    FILE *in = fopen("in23", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    Instruction instructionsBuff[INSTRUCTION_COUNT] = {{0}};
    Register registerBuff[b+1] = {0};
    Computer computer = getComputer(in, registerBuff, instructionsBuff);
    run(&computer);
    return computer.registers[b];
}

int part2(FILE *in)
{
    Instruction instructionsBuff[INSTRUCTION_COUNT] = {{0}};
    Register registerBuff[b+1] = {0};
    Computer computer = getComputer(in, registerBuff, instructionsBuff);
    computer.registers[a] = 1;
    run(&computer);
    return computer.registers[b];
}

void run(Computer *computer)
{
    Instruction currentInstruction;
    while (1)
    {
        currentInstruction = computer->instructions[computer->programCounter];
        switch (currentInstruction.op)
        {
            case nop:
                return;
            case hlf:
                computer->registers[currentInstruction.reg] /= 2;
                computer->programCounter++;
                break;
            case tpl:
                computer->registers[currentInstruction.reg] *= 3;
                computer->programCounter++;
                break;
            case inc:
                computer->registers[currentInstruction.reg]++;
                computer->programCounter++;
                break;
            case jmp:
                computer->programCounter += currentInstruction.off;
                break;
            case jie:
                if (computer->registers[currentInstruction.reg] & 1)
                    // Odd
                    computer->programCounter++;
                else
                    computer->programCounter += currentInstruction.off;
                break;
            case jio:
                if (computer->registers[currentInstruction.reg] == 1)
                    computer->programCounter += currentInstruction.off;
                else
                    computer->programCounter++;
                break;
            default:
                return;
        }
    }
}

Computer getComputer(FILE *in, unsigned int *registerBuff, Instruction *instructionsBuff)
{
    Computer res = (Computer) {.registers = registerBuff, .instructions = instructionsBuff, .programCounter = 0};
    size_t n = 0;
    char *line = NULL;
    for (int i = 0; i < INSTRUCTION_COUNT; i++)
    {
        if (getline(&line, &n, in) == 0)
        {
            fprintf(stderr, "Less lines than expected\n");
            break;
        }

        if (strstr(line, "hlf") != NULL)
        {
            res.instructions[i].op = hlf;
            res.instructions[i].reg = line[IL+1] - 'a';
        }
        else if (strstr(line, "tpl") != NULL)
        {
            res.instructions[i].op = tpl;
            res.instructions[i].reg = line[IL+1] - 'a';
        }
        else if (strstr(line, "inc") != NULL)
        {
            res.instructions[i].op = inc;
            res.instructions[i].reg = line[IL+1] - 'a';
        }
        else if (strstr(line, "jmp") != NULL)
        {
            res.instructions[i].op = jmp;
            res.instructions[i].off = atoi(line+IL+1);
        }
        else if (strstr(line, "jie") != NULL)
        {
            res.instructions[i].op = jie;
            res.instructions[i].reg = line[IL+1] - 'a';
            res.instructions[i].off = atoi(line+IL+strlen(" x, "));
        }
        else if (strstr(line, "jio") != NULL)
        {
            res.instructions[i].op = jio;
            res.instructions[i].reg = line[IL+1] - 'a';
            res.instructions[i].off = atoi(line+IL+strlen(" x, "));
        }
        else if (*line == '\n')
            res.instructions[i].op = nop;
    }
    free(line);
    return res;
}

