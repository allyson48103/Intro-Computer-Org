/**
 * Project 1
 * Assembler code fragment for LC-2K
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//Every LC2K file will contain less than 1000 lines of assembly.
#define MAXLINELENGTH 1000

// *--- Struct to hold address for label
struct LabelAddress{
    int address;
    char label[7];
};

// *--- Function headers
int add(char *regA, char *regB, char *destReg);
int nor(char *regA, char *regB, char *destReg);
int lw (int type, struct LabelAddress list[MAXLINELENGTH], int address, int numLabels, char *regA, char *regB, char *offset);
int sw (int type, struct LabelAddress list[MAXLINELENGTH], int address, int numLabels, char *regA, char *regB, char *offset);
int beq(int type, struct LabelAddress list[MAXLINELENGTH], int address, int numLabels, char *regA, char *regB, char *offset);
int jalr(char *regA, char *regB);
int halt();
int noop();
int fill(int type, struct LabelAddress list[MAXLINELENGTH], int address, int numLabels, char *fillval);
int findAddress(int type, char* field0, struct LabelAddress list[], int address, int numLabels);
void findDuplicate(struct LabelAddress list[MAXLINELENGTH], int numLabels);


// Functions already immplemented for you
int readAndParse(FILE *, char *, char *, char *, char *, char *);
static void checkForBlankLinesInCode(FILE *inFilePtr);
static inline int isNumber(char *);
static inline void printHexToFile(FILE *, int);


int main(int argc, char **argv)
{
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
            arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

    if (argc != 3) {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
            argv[0]);
        exit(1);
    }

    inFileString = argv[1];
    outFileString = argv[2];

    inFilePtr = fopen(inFileString, "r");
    if (inFilePtr == NULL) {
        printf("error in opening %s\n", inFileString);
        exit(1);
    }

    // Check for blank lines in the middle of the code.
    checkForBlankLinesInCode(inFilePtr);

    outFilePtr = fopen(outFileString, "w");
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileString);
        exit(1);
    }

    // *--- First pass
    int address = 0;
    int numLabels = 0;
    struct LabelAddress list[MAXLINELENGTH];

    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){
        if (strlen(label)){
            list[numLabels].address = address;
            strcpy(list[numLabels].label, label);
            numLabels++;
        }
        address++;
    }

    findDuplicate(list, numLabels);

    // *--- Second pass
    rewind(inFilePtr);
    address = 0;

    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
        int val = 0;
        
        if (!strcmp(opcode, "add")) {
            val = add(arg0, arg1, arg2);
        } else if (!strcmp(opcode, "nor")) {
            val = nor(arg0, arg1, arg2);
        } else if (!strcmp(opcode, "lw")) {
            val = lw(2, list, address, numLabels, arg0, arg1, arg2);
        } else if (!strcmp(opcode, "sw")) {
            val = sw(3, list, address, numLabels, arg0, arg1, arg2);
        } else if (!strcmp(opcode, "beq")) {
            // Increment the address before calculating the offset for beq
            address++;
            val = beq(4, list, address - 1, numLabels, arg0, arg1, arg2);  // Fix offset calculation by passing address - 1
        } else if (!strcmp(opcode, "jalr")) {
            val = jalr(arg0, arg1);
        } else if (!strcmp(opcode, "halt")) {
            val = halt();
        } else if (!strcmp(opcode, "noop")) {
            val = noop();
        } else if (!strcmp(opcode, ".fill")) {
            val = fill(-1, list, address, numLabels, arg0);
        } else {
            printf("Invalid opcode entered \n");
            exit(1);
        }

        // Increment the address for non-beq instructions
        if (strcmp(opcode, "beq")) {
            address++;
        }
        
        printHexToFile(outFilePtr, val);
    } // while

    exit(0);

} // main()


// Returns non-zero if the line contains only whitespace.
static int lineIsBlank(char *line) {
    char whitespace[4] = {'\t', '\n', '\r', ' '};
    int nonempty_line = 0;
    for(int line_idx=0; line_idx < strlen(line); ++line_idx) {
        int line_char_is_whitespace = 0;
        for(int whitespace_idx = 0; whitespace_idx < 4; ++ whitespace_idx) {
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


// *--- Function Implementations:


int add(char *regA, char *regB, char *destReg) {
    int val = 0;

    if (!(isNumber(regA) && isNumber(regB) && isNumber(destReg))){
        printf("Error: register not number %s\n", regA);
        exit(1);
    }
    
    // Convert register strings to integers and perform error checking
    int regA_val = atoi(regA);
    int regB_val = atoi(regB);
    int destReg_val = atoi(destReg);

    // Error checking: registers must be integers between 0 and 7

    if (regA_val < 0 || regA_val > 7) {
        printf("Error: Invalid register A value %s\n", regA);
        exit(1);
    }
    if (regB_val < 0 || regB_val > 7) {
        printf("Error: Invalid register B value %s\n", regB);
        exit(1);
    }
    if (destReg_val < 0 || destReg_val > 7) {
        printf("Error: Invalid destination register value %s\n", destReg);
        exit(1);
    }

    val |= (regA_val << 19);
    val |= (regB_val << 16);
    val |= destReg_val;  

    return val; // Return the machine code value for the add instruction
} // add()


int nor(char *regA, char *regB, char *destReg) {
    int val = 0;

    // NOR opcode: 001 -> 1 << 22
    val = (1 << 22);

    if (!(isNumber(regA) && isNumber(regB) && isNumber(destReg))){
        printf("Error: register not number %s\n", regA);
        exit(1);
    }

    // Convert register values (regA, regB, destReg) to integers
    int regA_val = atoi(regA);
    int regB_val = atoi(regB);
    int destReg_val = atoi(destReg);

    // Error checking for valid register values [0, 7]
    if (regA_val < 0 || regA_val > 7 || regB_val < 0 || regB_val > 7 || destReg_val < 0 || destReg_val > 7) {
        printf("error: register values must be between 0 and 7\n");
        exit(1);
    }

    // Shift the register values into their respective positions in the instruction
    val |= (regA_val << 19);  // regA in bits 21-19
    val |= (regB_val << 16); 
    val |= destReg_val;

    return val;
} // nor()


int lw(int type, struct LabelAddress list[MAXLINELENGTH], int address, int numLabels, char *regA, char *regB, char *offset) {
    int regA_val;
    int regB_val;
    int offset_val;
    int val = 0;

    if (!(isNumber(regA) && isNumber(regB))){
        printf("Error: register not number %s\n", regA);
        exit(1);
    }
    
    regA_val = atoi(regA);    
    regB_val = atoi(regB);

    // Error Checking if registers in range [0,7]
    if (regA_val < 0 || regA_val > 7) { 
        printf("Invalid register %s\n", regA);
        exit(1);
    } 
    if (regB_val < 0 || regB_val > 7) { 
        printf("Invalid register %s\n", regB);
        exit(1);
    }

    // Opcode for 'lw' is 0b010, which is 2 << 22
    val = (2 << 22);

    val |= (regA_val << 19);
    val |= (regB_val << 16);
    
    if (isNumber(offset)) {
        offset_val = atoi(offset);
        if (offset_val < -32768 || offset_val > 32767) {
            printf("Offset field is too big\n");
            exit(1);
        }
        if (offset_val < 0){
            val |= (offset_val & 0xFFFF);
        }
        else {
            val |= offset_val;
        }
    } else {
        val |= findAddress(type, offset, list, address, numLabels);
    }
    
    return val;
}// lw()

int sw(int type, struct LabelAddress list[MAXLINELENGTH], int address, int numLabels, char *regA, char *regB, char *offset) {
    int regA_val;
    int regB_val;
    int offset_val;
    int val = 0;

    if (!(isNumber(regA) && isNumber(regB))){
        printf("Error: register not number %s\n", regA);
        exit(1);
    }
    
    regA_val = atoi(regA);
    regB_val = atoi(regB);


    // Error Checking if registers are in range [0,7]
    if (regA_val < 0 || regA_val > 7) { 
        printf("Invalid register %s\n", regA);
        exit(1);
    }
    if (regB_val < 0 || regB_val > 7) { 
        printf("Invalid register %s\n", regB);
        exit(1);
    }

    // Instruction opcode for 'sw' is 0b011, which is 3 << 22
    val = (3 << 22);

    val |= (regA_val << 19);
    val |= (regB_val << 16);
    
    if (isNumber(offset)) {
        offset_val = atoi(offset);
        if (offset_val < -32768 || offset_val > 32767) {
            printf("Field is too big \n");
            exit(1);
        }
        val |= (offset_val & 0xFFFF);
    } else {
        val |= findAddress(type, offset, list, address, numLabels);
    }
    
    return val;
} // sw()


int beq(int type, struct LabelAddress list[MAXLINELENGTH], int address, int numLabels, char *regA, char *regB, char *offset) {
    int val = 0;

    // beq opcode: 100 -> 4 << 22
    val = (4 << 22);

    if (!(isNumber(regA) && isNumber(regB))){
        printf("Error: register not number %s\n", regA);
        exit(1);
    }

    // Convert register values from strings to integers
    int regA_val = atoi(regA);
    int regB_val = atoi(regB);

    // Error checking for valid register values [0, 7]
    if (regA_val < 0 || regA_val > 7 || regB_val < 0 || regB_val > 7) {
        printf("Error: register values must be between 0 and 7\n");
        exit(1);
    }

    // Shift the register values into their respective positions in the instruction
    val |= (regA_val << 19);  // regA in bits 21-19
    val |= (regB_val << 16);  // regB in bits 18-16

    // Calculate the offset (immediate value or label)
    int offset_val;
    if (isNumber(offset)) {
        // If the offset is a number, convert it to an integer
        offset_val = atoi(offset);
        if (offset_val < -32768 || offset_val > 32767) {
            printf("Error: offset field out of range\n");
            exit(1);
        }
    } else {
        // If it's a label, find the address of the label and calculate the offset
        offset_val = findAddress(type, offset, list, address, numLabels);
    }

    // Add the offset to the instruction (offset fits in the lower 16 bits)
    val |= (offset_val & 0xFFFF);

    return val;
} // beq()

int jalr(char *regA, char *regB) {
    int regA_val;
    int regB_val;
    int val = 0;
    
    // Instruction opcode for 'jalr' is 0b101, which is 5 << 22
    val = (5 << 22);

    if (!(isNumber(regA) && isNumber(regB))){
        printf("Error: register not number %s\n", regA);
        exit(1);
    }

    
    regA_val = atoi(regA);
    regB_val = atoi(regB);


    // Error Checking if registers are in range [0,7]
    if (regA_val < 0 || regA_val > 7) { 
        printf("Invalid register %s\n", regA);
        exit(1);
    }
    if (regB_val < 0 || regB_val > 7) { 
        printf("Invalid register %s\n", regB);
        exit(1);
    }
    
    // Shift and add register values to the instruction value
    val |= (regA_val << 19);
    val |= (regB_val << 16);
    
    return val;
} // jalr()


int halt() {
    int val = 0;
    
    // Instruction opcode for 'halt' is 0b110, which is 6 << 22
    val = (6 << 22);
    
    return val;
} // halt()

int noop() {
    int val = 0;
    
    // Instruction opcode for 'noop' is 0b111, which is 7 << 22
    val = (7 << 22);
    
    return val;
} // noop()

int fill(int type, struct LabelAddress list[MAXLINELENGTH], int address, int numLabels, char *fillval){
    int val = 0;
    int fillvalue = 0;

    if (isNumber(fillval)){
        fillvalue = atoi(fillval);
        /*
        if (fillvalue < -32768 || fillvalue > 32767) {
            printf("Field is too big \n");
            exit(1);
        }*/
    } else {
        fillvalue = findAddress(type, fillval, list, address, numLabels);
    }

    val += fillvalue;

    return val;

} // fill()

int findAddress(int type, char* field0, struct LabelAddress list[], int address, int numLabels) {
    int foundAddress = -1;  
    
    // Search for the label in the list 
    for (int i = 0; i < numLabels; i++) {
        if (strcmp(list[i].label, field0) == 0) {
            foundAddress = list[i].address;
            break;
        }
    }

    // If label was not found, print an error and exit
    if (foundAddress == -1) {
        printf("Error: Undefined label, unable to find address '%s'\n", field0);
        exit(1);
    }

    // Handle address transformation based on type 
    if (type == 2 || type == 3) {
        // For type 2 or 3 (lw or sw), just use the lower 16 bits of the address
        foundAddress &= 0xFFFF;
    } else if (type == 4) {
        // For type 4 (beq), calculate the relative offset
        int offset = foundAddress - (address + 1);  // Calculate PC-relative offset
        if (offset < -32768 || offset > 32767) {
            printf("Error: Calculated relative offset is out of range\n");
            exit(1);
        }
        foundAddress = offset & 0xFFFF; // Mask to 16 bits
    }

    return foundAddress; // for type -1 (fill) skip here
} // findAddress()

void findDuplicate(struct LabelAddress list[MAXLINELENGTH], int numLabels){
    for (int i = 0; i < numLabels; i++){
        for (int j = i + 1; j < numLabels; j++){
            if (strcmp(list[i].label, list[j].label) == 0){
                printf("Found duplicate label \n");
                exit(1);
            }
        }
    }
} // findDuplicate()










/*
* NOTE: The code defined below is not to be modifed as it is implimented correctly.
*/

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
    char *arg1, char *arg2)
{
    char line[MAXLINELENGTH];
    char *ptr = line;

    /* delete prior values */
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    /* read the line from the assembly-language file */
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
	/* reached end of file */
        return(0);
    }

    /* check for line too long */
    if (strlen(line) == MAXLINELENGTH-1) {
	printf("error: line too long\n");
	exit(1);
    }

    // Ignore blank lines at the end of the file.
    if(lineIsBlank(line)) {
        return 0;
    }

    /* is there a label? */
    ptr = line;
    if (sscanf(ptr, "%[^\t\n ]", label)) {
	/* successfully read label; advance pointer over the label */
        ptr += strlen(label);
    }

    /*
     * Parse the rest of the line.  Would be nice to have real regular
     * expressions, but scanf will suffice.
     */
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


