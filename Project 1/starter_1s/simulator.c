/*
 * Project 1
 * EECS 370 LC-2K Instruction-level simulator
 *
 * Make sure to NOT modify printState or any of the associated functions
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//DO NOT CHANGE THE FOLLOWING DEFINITIONS 

// Machine Definitions
#define MEMORYSIZE 65536 /* maximum number of words in memory (maximum number of lines in a given file)*/
#define NUMREGS 8 /*total number of machine registers [0,7]*/

// File Definitions
#define MAXLINELENGTH 1000 /* MAXLINELENGTH is the max number of characters we read */

typedef struct 
stateStruct {
    int pc;
    int mem[MEMORYSIZE];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void instruction(stateType *);

void printState(stateType *);

static inline int convertNum(int32_t);

int 
main(int argc, char **argv)
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
        printf("error: can't open file %s , please ensure you are providing the correct path", argv[1]);
        perror("fopen");
        exit(2);
    }


    /* read the entire machine-code file into memory */
    for (state.numMemory=0; fgets(line, MAXLINELENGTH, filePtr) != NULL; state.numMemory++) {
		    if (state.numMemory >= MEMORYSIZE) {
			      fprintf(stderr, "exceeded memory size\n");
			      exit(2);
		    }
		    if (sscanf(line, "%x", state.mem+state.numMemory) != 1) {
			      fprintf(stderr, "error in reading address %d\n", state.numMemory);
			      exit(2);
		    }
		    printf("memory[%d]=0x%x\n", state.numMemory, state.mem[state.numMemory]);
    }

    // ---* MY CODE:

    // initialize PC and registers to 0
    state.pc = 0;
    for (int i = 0; i < NUMREGS; i++){
        state.reg[i] = 0;
    }

    // execute and simulate function
    instruction(&state);

    //Your code ends here! ---* 

    return(0);
}

/*
* DO NOT MODIFY ANY OF THE CODE BELOW. 
*/

void printState(stateType *statePtr) {
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i=0; i<statePtr->numMemory; i++) {
        printf("\t\tmem[ %d ] 0x%08X\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
	  for (i=0; i<NUMREGS; i++) {
	      printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
	  }
    printf("end state\n");
}

// convert a 16-bit number into a 32-bit Linux integer
static inline int convertNum(int num) 
{
    return num - ( (num & (1<<15)) ? 1<<16 : 0 );
}

/*
* Write any helper functions that you wish down here. 
*/

void instruction(stateType * structPtr) {
    int temp;          // Used to hold the instruction from memory
    int opcode;        // To store the opcode of the instruction
    int regA, regB;    // To hold register values for regA and regB
    int destReg;       // To hold the destination register for R-type instructions
    int offset;        // To hold the offset value for I-type instructions
    int numInst = 0;   // To keep track of the number of instructions executed
    bool halt = false; // To determine when to stop execution

    while (!halt) {
        // Print the current state of the machine
        printState(structPtr);

        // Fetch the instruction from memory at the current program counter (PC)
        temp = structPtr->mem[structPtr->pc];

        // Decode the instruction: extract the opcode, registers, and offset
        opcode = (temp >> 22) & 7;    // Opcode is in bits 24-22
        regA = (temp >> 19) & 7;      // regA is in bits 21-19
        regB = (temp >> 16) & 7;      // regB is in bits 18-16
        destReg = temp & 7;           // destReg is in bits 2-0
        offset = convertNum(temp & 65535);  // Offset field is bits 15-0

        // Execute the instruction based on the opcode
        switch (opcode) {
            // ADD
            case 0:
                structPtr->reg[destReg] = structPtr->reg[regA] + structPtr->reg[regB];
                structPtr->pc++;  // Move to the next instruction
                break;

            // NOR
            case 1:
                structPtr->reg[destReg] = ~(structPtr->reg[regA] | structPtr->reg[regB]);
                structPtr->pc++;  // Move to the next instruction
                break;

            // LW (Load Word)
            case 2:
                structPtr->reg[regB] = structPtr->mem[structPtr->reg[regA] + offset];
                structPtr->pc++;  // Move to the next instruction
                break;

            // SW (Store Word)
            case 3:
                structPtr->mem[structPtr->reg[regA] + offset] = structPtr->reg[regB];
                structPtr->pc++;  // Move to the next instruction
                break;

            // BEQ (Branch if Equal)
            case 4:
                if (structPtr->reg[regA] == structPtr->reg[regB]) {
                    structPtr->pc += 1 + offset;  // Branch if equal
                } else {
                    structPtr->pc++;  // Otherwise, move to the next instruction
                }
                break;

            // JALR (Jump and Link Register)
            case 5:
                structPtr->reg[regB] = structPtr->pc + 1;  // Save the return address in regB
                structPtr->pc = structPtr->reg[regA];      // Jump to the address in regA
                break;

            // HALT
            case 6:
                halt = true;  // Stop the machine
                structPtr->pc++;  // Move to the next instruction
                break;

            // NOOP (No Operation)
            case 7:
                structPtr->pc++;  // Just move to the next instruction
                break;
        }

        // Increment the instruction count
        numInst++;

        // If halt is triggered, break out of the loop
        if (halt) {
            break;
        }
    }

    // After the halt, print the final state of the machine and the total instructions executed
    printf("machine halted\n");
    printf("total of %d instructions executed\n", numInst);
    printf("final state of machine:\n");
    printState(structPtr);
} // instruction
