#include "Address.h"
#include "Registers.h"

static const struct methodName {
    const char *name;
/*Names of directive sentences*/
} methodNames[] = {
        {".data"},
        {".string"},
        {".entry"},
        {".extern"},
};

int handle_instruction_Name(char *instruction) {
    int i;
    for (i = 0; i < MAX_NUM_INSTRUCTION; i++) {
        if (strcmp(instruction, methodNames[i].name) == 0)
            return TRUE;
    }
    return FALSE;
}

addressingMode handel_address(char *operand, FILE *checksLabelFile, int flag) {
    int i = 0;
    if (operand != NULL && operand[0] != '\0') {
        if (validNumber(operand, FALSE))
            return immediateAddress;
        if (validRegName(operand))/*if its name of register from registerTable, its directRegisterAddress */
            return directRegisterAddress;
        if (handel_label_name(operand)) {/*when address is a valid label name,it's directAddressed*/
            if (flag)/*We will save all the labels in the file, and at the end of the first pass we will check that they have all been defined correctly*/
                fprintf(checksLabelFile, "%s\n", operand);
            return directAddress;
        }/*if address starts with # and a number right after that, it's immediately addressed*/
        if (operand[i] == '#') {
            for (i = 0; i < strlen(operand) - 1; ++i)
                operand[i] = operand[i + 1];
            operand[strlen(operand) - 1] = '\0';
            if (validNumber(operand, FALSE))
                return immediateAddress;
        }
    }
    return NON; /*if nothing, just return NON - "no exist" */
}



