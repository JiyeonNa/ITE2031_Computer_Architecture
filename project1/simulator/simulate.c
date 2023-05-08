/* LC-2K Instruction-level simulator */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000 
typedef struct stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);
int convertNum(int num);

void parse(stateType *statePtr, int *opcode, int *arg0, int *arg1, int *arg2);
void isValidReg(int arg);

int main(int argc, char *argv[])
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;

    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }

    /* read in the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL;
            state.numMemory++) {

        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }

    /* initialize register and program counter to 0 */
    state.pc=0;
    for (int i = 0; i < NUMREGS; i++) {
        state.reg[i] = 0;
    }

    /* simulate the program until the program executes a halt */
    int opcode;
    int arg0, arg1, arg2;
    int offsetField;
    int instructionCount = 0;
    int checkHalt = 0;

    while(!checkHalt) {
        printState(&state);
        parse(&state, &opcode, &arg0, &arg1, &arg2);
        state.pc++;

        /* R-type instructions */
        if (opcode == 0 || opcode == 1) {
            isValidReg(arg0);
            isValidReg(arg1);
            isValidReg(arg2);

            switch(opcode) {
                case 0:
                    state.reg[arg2] = state.reg[arg0] + state.reg[arg1];
                    break;
                case 1:
                    state.reg[arg2] = ~(state.reg[arg0] | state.reg[arg1]);
                    break;
            }

            instructionCount++;
        }

        /* I-type instructions */
        else if (opcode == 2 || opcode == 3 || opcode == 4) {
            isValidReg(arg0);
            isValidReg(arg1);
            offsetField = convertNum(arg2);

            /* check if the offsetField fits in 16 bits */
            if (offsetField < -32768 || offsetField > 32767) {
                printf("error: offsetField out of range\n");
                exit(1);
            }

            switch(opcode) {
                case 2:
                    state.reg[arg1] = state.mem[state.reg[arg0] + offsetField];
                    break;
                case 3:
                    state.mem[state.reg[arg0] + offsetField] = state.reg[arg1];
                    break;
                case 4:
                    if (state.reg[arg0] == state.reg[arg1]) {
                        state.pc += offsetField;
                    }
                    break;
            }

            instructionCount++;
        }

        /* J-type instructions */
        else if (opcode == 5) {
            isValidReg(arg0);
            isValidReg(arg1);

            state.reg[arg1] = state.pc + 1;
            state.pc = state.reg[arg0] - 1;

            instructionCount++;
        }

        /* O-type instructions */
        else if (opcode == 6 || opcode == 7) {
            if (opcode == 6) {
                checkHalt = 1;
            }

            instructionCount++;
        }

        else{
            printf("error: invalid opcode\n");
        }
    }
    printf("machine halted\n");
    printf("total of %d instructions executed\n", instructionCount);
    printf("final state of machine:\n");
    printState(&state);

    fclose(filePtr);

    return(0);
}

void printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i = 0; i < statePtr->numMemory; i++) {
        printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
    for (i = 0; i < NUMREGS; i++) {
        printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("end state\n");
}

int convertNum(int num)
{
	/* convert a 16-bit number into a 32-bit Linux integer */
	if (num & (1 << 15)) {
		num -= (1 << 16);
	}
	return (num);
}

void parse(stateType *statePtr, int *opcode, int *arg0, int *arg1, int *arg2)
{
    *opcode = statePtr->mem[statePtr->pc] >> 22 & 0x7;

    /* I-type, J-type, O-type */
    if (*opcode == 2 || *opcode == 3 || *opcode == 4 || *opcode == 5 || *opcode == 6 || *opcode == 7) {
        *arg0 = (statePtr->mem[statePtr->pc] >> 19) & 0x7;
        *arg1 = (statePtr->mem[statePtr->pc] >> 16) & 0x7;
        *arg2 = statePtr->mem[statePtr->pc] & 0xFFFF;
        return;
    }

    /* R-type */
    *arg0 = (statePtr->mem[statePtr->pc] >> 19) & 0x7;
    *arg1 = (statePtr->mem[statePtr->pc] >> 16) & 0x7;
    *arg2 = statePtr->mem[statePtr->pc] & 0x7;

}

void isValidReg(int arg)
{
    if(arg < 0 || arg > 7){
        printf("error: invalid register number\n");
        exit(1);
    }
}
