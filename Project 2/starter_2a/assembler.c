/**
 * Project 2
 * Assembler code fragment for LC-2K
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ADD_OPCODE 0       // Add Instruction (000)
#define NOR_OPCODE 1 << 22  // NOR Instruction (001), bit 22
#define LW_OPCODE 1 << 23   // Load Word Instruction (010), bit 23
#define SW_OPCODE (1 << 23) + (1 << 22) // Store Word Instruction (011), bits 23 and 22
#define BEQ_OPCODE 1 << 24  // Branch if Equal Instruction (100), bit 24
#define JALR_OPCODE (1 << 24) + (1 << 22) // Jump and Link Register Instruction (101), bits 24 and 22
#define HALT_OPCODE (1 << 24) + (1 << 23) // Halt Instruction (110), bits 24 and 23
#define NOOP_OPCODE (1 << 24) + (1 << 23) + (1 << 22) // No Operation Instruction (111), bits 24, 23, and 22



//Every LC2K file will contain less than 1000 lines of assembly.
#define MAXLINELENGTH 1000

/**
 * Requires: readAndParse is non-static and unmodified from project 1a. 
 *   inFilePtr and outFilePtr must be opened. 
 *   inFilePtr must be rewound before calling this function.
 * Modifies: outFilePtr
 * Effects: Prints the correct machine code for the input file. After 
 *   reading and parsing through inFilePtr, the pointer is rewound.
 *   Most project 1a error checks are done. No undefined labels of any
 *   type are checked, and these are instead resolved to 0.
*/
/**
 * This function will be provided in an instructor object file once the
 * project 1a deadline + late days has passed.
*/
// extern void print_inst_machine_code(FILE *inFilePtr, FILE *outFilePtr);

int readAndParse(FILE *, char *, char *, char *, char *, char *);
static void checkForBlankLinesInCode(FILE *inFilePtr);
static inline int isNumber(char *);
static inline void printHexToFile(FILE *, int);
int validLabel(char *);
int GetOffset(char *, char *, FILE *, int, char[][7], int, int[]);
int GetRegBits(int, char);



int main(int argc, char **argv) {
    char *inFileStr, *outFileStr;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
            arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

    if (argc != 3) {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
            argv[0]);
        exit(1);
    }

    inFileStr = argv[1];
    outFileStr = argv[2];

    inFilePtr = fopen(inFileStr, "r");
    if (inFilePtr == NULL) {
        printf("error in opening %s\n", inFileStr);
        exit(1);
    }
    // Check for blank lines in the middle of the code.
    checkForBlankLinesInCode(inFilePtr);
    
    outFilePtr = fopen(outFileStr, "w");
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileStr);
        exit(1);
    }

   // ------ Global variables and relocation tracking
    struct symbolTable {
        char label[7];
        char category[2];
        int position;
    };

    struct symbolTable symbols[50];

    struct relocationEntry {
        int position;
        char instruction[7];
        char label[7];
    };

    struct relocationEntry relocationList[50];
    int relocationCount = 0;

    char labelList[50][7];  
    int labelPosition[50];
    int labelCount = 0, textCount = 0, dataCount = 0, symbolCount = 0;

    int globalIndex, labelIndex, relocationIndex, currentInstruction, dataStart = -1;
    for (currentInstruction = 0, labelIndex = 0, globalIndex = 0; 
        readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2); 
        currentInstruction++) {

        if (!strcmp(opcode, ".fill") && dataStart < 0) {
            dataStart = currentInstruction;
        }

        if (validLabel(label)) {  // If a label is found
            for (int x = 0; x < labelCount; x++) {
                if (!strcmp(labelList[x], label)) {  // Check for duplicates
                    printf("Error: duplicate label detected\n");
                    exit(1);
                }
            }

            labelPosition[labelIndex] = currentInstruction;
            strcpy(labelList[labelIndex++], label);
            labelCount++;

            if (isupper(label[0])) {  // If it's a global label
                strcpy(symbols[globalIndex].label, label);  // Add to global symbols

                symbolCount++;

                if (!strcmp(opcode, ".fill")) {
                    strcpy(symbols[globalIndex].category, "D");  // Mark as data
                    symbols[globalIndex++].position = currentInstruction - dataStart;
                } else {
                    strcpy(symbols[globalIndex].category, "T");  // Mark as text
                    symbols[globalIndex++].position = currentInstruction;
                }
            }
        }

        // Track number of data/text instructions
        if (!strcmp(opcode, ".fill")) {
            dataCount++;
        } else if (!strcmp(opcode, "lw") || !strcmp(opcode, "sw")
            || !strcmp(opcode, "add") || !strcmp(opcode, "nor") 
            || !strcmp(opcode, "beq") || !strcmp(opcode, "jalr") 
            || !strcmp(opcode, "halt") || !strcmp(opcode, "noop")) {
            ++textCount;
        } else {
            printf("Error: unrecognized opcode\n");
            exit(1);
        }
    } // for loop

    rewind(inFilePtr);  // Rewind file pointer

    for (currentInstruction = 0, relocationIndex = 0; 
        readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2); 
        ++currentInstruction) {

        if (!strcmp(opcode, "lw") || !strcmp(opcode, "sw")) {
            if (!isNumber(arg2)) {  

                relocationList[relocationIndex].position = currentInstruction;  // Track relocation
                strcpy(relocationList[relocationIndex].instruction, opcode);
                strcpy(relocationList[relocationIndex++].label, arg2);

                relocationCount++;

                if (isupper(arg2[0])) {
                    int found = 0;
                    for (int i = 0; i < labelCount; i++) {
                        if (!strcmp(labelList[i], arg2)) {  // Check if global was defined...
                            found = 1;
                        }
                    }

                    for (int i = 0; i < symbolCount; i++) {
                        if (!strcmp(symbols[i].label, arg2)) {  // Check if global was already found...
                            found = 1;
                        }
                    }

                    if (!found) {
                        symbolCount++;
                        strcpy(symbols[globalIndex].label, arg2);  // Add undefined global
                        strcpy(symbols[globalIndex].category, "U");  // Undefined global
                        symbols[globalIndex++].position = 0;
                    }
                }
            }
        } else if (!strcmp(opcode, ".fill")) {
            if (!isNumber(arg0)) {  // If symbolic address used in .fill...
                relocationList[relocationIndex].position = currentInstruction - dataStart;
                strcpy(relocationList[relocationIndex].instruction, opcode);
                strcpy(relocationList[relocationIndex++].label, arg0);
                ++relocationCount;

                if (isupper(arg0[0])) {
                    int found = 0;
                    for (int i = 0; i < labelCount; i++) {
                        if (!strcmp(labelList[i], arg0)) {
                            found = 1;
                        }
                    }

                    for (int i = 0; i < symbolCount; i++) {
                        if (!strcmp(symbols[i].label, arg0)) {
                            found = 1;
                        }
                    }

                    if (!found) {
                        ++symbolCount;
                        strcpy(symbols[globalIndex].label, arg0);
                        strcpy(symbols[globalIndex].category, "U");  // Undefined global
                        symbols[globalIndex++].position = 0;
                    }
                }
            }
        } else if (!strcmp(opcode, "beq")) {
            if (!isNumber(arg2) && isupper(arg2[0])) {  
                int found = 0;
                for (int i = 0; i < labelCount; i++) {
                    if (!strcmp(labelList[i], arg2)) {
                        found = 1;
                    }
                }

                if (!found) {
                    printf("Error: 'beq' using an undefined global label\n");
                    exit(1);
                }
            }
        } 
    } // for loop

    // Write the header: Object file
    fprintf(outFilePtr, "%d %d %d %d\n", textCount, dataCount, symbolCount, relocationCount);

    rewind(inFilePtr);

    // Process TEXT & DATA sections
    int PC, machineCode;
    for (machineCode = 0, PC = 0; readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2); 
        ++PC, machineCode = 0) {

        if (strcmp(opcode, "add") == 0) {
            machineCode += GetRegBits(atoi(arg0), 'A') + GetRegBits(atoi(arg1), 'B') + GetRegBits(atoi(arg2), 'C');
        }
        else if (strcmp(opcode, "nor") == 0) {
            machineCode = NOR_OPCODE;  // NOR bit shift
            machineCode += GetRegBits(atoi(arg0), 'A') + GetRegBits(atoi(arg1), 'B') + GetRegBits(atoi(arg2), 'C');
        }
        else if (strcmp(opcode, "lw") == 0) {
            machineCode = LW_OPCODE;  // Load Word bit shift
            machineCode += GetRegBits(atoi(arg0), 'A') + GetRegBits(atoi(arg1), 'B');
            machineCode += GetOffset(opcode, arg2, inFilePtr, PC, labelList, labelCount, labelPosition);
        }
        else if (strcmp(opcode, "sw") == 0) {
            machineCode = SW_OPCODE;  // Store Word bit shift
            machineCode += GetRegBits(atoi(arg0), 'A') + GetRegBits(atoi(arg1), 'B');
            machineCode += GetOffset(opcode, arg2, inFilePtr, PC, labelList, labelCount, labelPosition);
        }
        else if (strcmp(opcode, "beq") == 0) {
            machineCode = BEQ_OPCODE;  // Branch if Equal bit shift
            machineCode += GetRegBits(atoi(arg0), 'A') + GetRegBits(atoi(arg1), 'B');
            machineCode += GetOffset(opcode, arg2, inFilePtr, PC + 1, labelList, labelCount, labelPosition);
        }
        else if (strcmp(opcode, "jalr") == 0) {
            machineCode = JALR_OPCODE;  // Jump and Link Register bit shift
            machineCode += GetRegBits(atoi(arg0), 'A') + GetRegBits(atoi(arg1), 'B');
        }
        else if (strcmp(opcode, "halt") == 0) {
            machineCode = HALT_OPCODE;  // Halt bit shift
        }
        else if (strcmp(opcode, "noop") == 0) {
            machineCode = NOOP_OPCODE;  // No Operation bit shift
        }

        else if (strcmp(opcode, ".fill") == 0) {   // Fill Instruction
            machineCode = GetOffset(opcode, arg0, inFilePtr, 0, labelList, labelCount, labelPosition);
        }

        printHexToFile(outFilePtr, machineCode);
    }


    // Process SYMBOLS
    for (int i = 0; i < symbolCount; i++) {
        fprintf(outFilePtr, "%s %s %d\n", symbols[i].label, symbols[i].category, symbols[i].position);
    }

    // Process RELOCATIONS
    for (int i = 0; i < relocationCount; i++) {
        fprintf(outFilePtr, "%d %s %s\n", relocationList[i].position, relocationList[i].instruction, relocationList[i].label);
    }

    return(0);

} // main


int GetRegBits(int regNum, char regType) {
    int bitPosition;

    // Determine bit position based on register type
    switch (regType) {
        case 'A':
            bitPosition = 21;  // Register A occupies bits 21-19
            break;
        case 'B':
            bitPosition = 18;  // Register B occupies bits 18-16
            break;
        case 'C':
            bitPosition = 2;   // Register C occupies bits 2-0
            break;
        default:
            bitPosition = 0;   // Default case, shouldn't normally occur
            break;
    }

    int result = 0;

    // Process each bit of regNum to calculate the result
    if (regNum >= 4) {
        result += (1 << bitPosition);  // Set the most significant bit
        regNum -= 4;
    }
    if (regNum >= 2) {
        result += (1 << (bitPosition - 1));  // Set the next bit
        regNum -= 2;
    }
    if (regNum == 1) {
        result += (1 << (bitPosition - 2));  // Set the least significant bit
    }

    return result;
} // GetRegBits()


int GetOffset(char* opcode, char* arg, FILE* inFile, int PC, char labels[][7], int numLabels, int labelAddy[]) {
    int value;

    // Check if argument is a number and fits within 16 bits
    if (isNumber(arg)) {
        value = atoi(arg);
        if (value < -32768 || value > 32767) {
            printf("Error: offsetField does not fit in 16 bits\n");
            exit(1);
        }
        // Handle positive and negative offsets, return the appropriate value
        if (value >= 0 || !strcmp(opcode, ".fill")) {
            return value;
        } else {
            return 65535 + value + 1;
        }
    }

    // If argument is not a number, search for the label
    for (int i = 0; i < numLabels; i++) {
        if (!strcmp(arg, labels[i])) {
            if (!strcmp(opcode, "lw") || !strcmp(opcode, "sw")) {
                return labelAddy[i];  // Return label address for lw/sw
            } else {
                int offset = labelAddy[i] - PC;  // Calculate offset relative to PC
                return (offset < 0) ? (65535 + offset + 1) : offset;
            }
        }
    }

    // If symbolic label not defined and it's a valid symbolic address for lw, sw, or .fill
    if (isupper(arg[0]) && (!strcmp(opcode, "lw") || !strcmp(opcode, "sw") || !strcmp(opcode, ".fill"))) {
        return 0;
    }

    // Handle undefined symbolic address
    printf("Error: use of undefined symbolic address\n");
    exit(1);
} // GetOffset()



// ERROR CHECKING: check if valid label
int validLabel(char* label) {
    if (label[0] == '\0') {  // Check if the label is an empty string
        return 0;
    }
    return 1; 
} // validLabel()







/*
* NOTE: The code defined below is not to be modifed as it is implemented correctly.
*/

// Returns non-zero if the line contains only whitespace.
int lineIsBlank(char *line) {
    char whitespace[4] = {'\t', '\n', '\r', ' '};
    int nonempty_line = 0;
    for(int line_idx=0; line_idx < strlen(line); ++line_idx) {
        int line_char_is_whitespace = 0;
        for(int whitespace_idx = 0; whitespace_idx < 4; ++whitespace_idx) {
            if(line[line_idx] == whitespace[whitespace_idx]) {
                line_char_is_whitespace = 1;
                break;
            }
        }
        if(!line_char_is_whitespace) {
            nonempty_line = 1;
            break;
        }
    }
    return !nonempty_line;
}

// Exits 2 if file contains an empty line anywhere other than at the end of the file.
// Note calling this function rewinds inFilePtr.
static void checkForBlankLinesInCode(FILE *inFilePtr) {
    char line[MAXLINELENGTH];
    int blank_line_encountered = 0;
    int address_of_blank_line = 0;
    rewind(inFilePtr);

    for(int address = 0; fgets(line, MAXLINELENGTH, inFilePtr) != NULL; ++address) {
        // Check for line too long
        if (strlen(line) >= MAXLINELENGTH-1) {
            printf("error: line too long\n");
            exit(1);
        }

        // Check for blank line.
        if(lineIsBlank(line)) {
            if(!blank_line_encountered) {
                blank_line_encountered = 1;
                address_of_blank_line = address;
            }
        } else {
            if(blank_line_encountered) {
                printf("Invalid Assembly: Empty line at address %d\n", address_of_blank_line);
                exit(2);
            }
        }
    }
    rewind(inFilePtr);
}

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 */
int readAndParse(FILE *inFilePtr, char *label,
	char *opcode, char *arg0, char *arg1, char *arg2) {

    char line[MAXLINELENGTH];
    char *ptr = line;

    // delete prior values
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    // read the line from the assembly-language file
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
		// reached end of file
        return(0);
    }

    // check for line too long
    if (strlen(line) >= MAXLINELENGTH-1) {
		printf("error: line too long\n");
		exit(1);
    }

    // Ignore blank lines at the end of the file.
    if(lineIsBlank(line)) {
        return 0;
    }

    // is there a label?
    ptr = line;
    if (sscanf(ptr, "%[^\t\n ]", label)) {
		// successfully read label; advance pointer over the label */
        ptr += strlen(label);
    }

    // Parse the rest of the line.  Would be nice to have real regular expressions, but scanf will suffice.
    sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
        opcode, arg0, arg1, arg2);
    return(1);
}

static inline int
isNumber(char *string)
{
    int num;
    char c;
    return((sscanf(string, "%d%c",&num, &c)) == 1);
}

// Prints a machine code word in the proper hex format to the file
static inline void 
printHexToFile(FILE *outFilePtr, int word) {
    fprintf(outFilePtr, "0x%08X\n", word);
}
