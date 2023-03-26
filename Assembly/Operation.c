#include "Operation.h"
#include "GeneralFunctions.h"

static const struct operationConsts {
    const char *command;
    int opcode;
    int numOfOperation;
    int startOp;
    int endOp;
/*An array representing all legal operations, their number,
amount of parameters, source address, destination address, for each method*/
} operation[] = {{"mov",  0,  2, ALL_OPERATION,     WITHOUT_FIRST_OPERATION},
                 {"cmp",  1,  2, ALL_OPERATION,     ALL_OPERATION},
                 {"add",  2,  2, ALL_OPERATION,     WITHOUT_FIRST_OPERATION},
                 {"sub",  3,  2, ALL_OPERATION,     WITHOUT_FIRST_OPERATION},
                 {"not",  4,  1, 0,                 WITHOUT_FIRST_OPERATION},
                 {"clr",  5,  1, 0,                 WITHOUT_FIRST_OPERATION},
                 {"lea",  6,  2, ONE_TWO_OPERATION, WITHOUT_FIRST_OPERATION},
                 {"inc",  7,  1, 0,                 WITHOUT_FIRST_OPERATION},
                 {"dec",  8,  1, 0,                 WITHOUT_FIRST_OPERATION},
                 {"jmp",  9,  1, 0,                 WITHOUT_FIRST_OPERATION},
                 {"bne",  10, 1, 0,                 WITHOUT_FIRST_OPERATION},
                 {"red",  11, 1, 0,                 WITHOUT_FIRST_OPERATION},
                 {"prn",  12, 1, 0,                 ALL_OPERATION},
                 {"jsr",  13, 1, 0,                 WITHOUT_FIRST_OPERATION},
                 {"rts",  14, 0, 0, 0},
                 {"stop", 15, 0, 0, 0}};

int validOpName(char *command) {
    int i;
    for (i = 0; i < MAX_OPERATIONS; i++) {
        if (!strcmp(command, operation[i].command))
            return TRUE;
    }
    return FALSE;
}

int whichOpcode(char *command) {
    int i;
    for (i = 0; i < MAX_OPERATIONS; i++) {
        if (!strcmp(command, operation[i].command))
            return operation[i].opcode;
    }
    return NON;
}

int numOfOperation(char *command) {
    int i;
    for (i = 0; i < MAX_OPERATIONS; i++) {
        if (strcmp(command, operation[i].command) == 0)
            return operation[i].numOfOperation;
    }
    return NON;
}
