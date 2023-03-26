#ifndef ADDRESS_H
#define ADDRESS_H

#include "GeneralFunctions.h"

#define MAX_NUM_INSTRUCTION 4

/*Gave numerical values to given addressing methods*/
typedef enum addressing {
    immediateAddress = 0,
    directAddress = 1,
    jumpWithParametersAddress = 2,
    directRegisterAddress = 3
} addressingMode;

/*
    This method checks if its valid instruction name

    parameters:
                @param instruction - tha name of instruction to check
    return:
                @return TRUE if it (.data/.string/.entry/.extern),otherwise False
*/
int handle_instruction_Name(char *instruction);


/*
    This method return the address for the given operand

    parameters:
                @param operand - the operand we are looking for its address
                @param checksLabelFile - The file saves all the labels that appear in the file
                @param flag - Indicates that an operand is a label and must be saved in a file
    return:
                @return the valid address for the operand if it legal, else return error
*/
addressingMode handel_address(char *operand, FILE *checksLabelFile, int flag);

#endif
