#include "Registers.h"
#include "GeneralFunctions.h"

static const struct registerConsts {
    const char *name;
    int value;
/*An array representing all valid registers*/
} registers[] = {{"r0", 0},
                 {"r1", 1},
                 {"r2", 2},
                 {"r3", 3},
                 {"r4", 4},
                 {"r5", 5},
                 {"r6", 6},
                 {"r7", 7}
};

int validRegName(char *validName) {
    int i;
    for (i = 0; i <= MAX_NUM_REGISTERS; i++) {
        if (strcmp(validName, registers[i].name) == 0)
            return TRUE;
    }
    return FALSE;
}

int whichRegisterValue(char *validName) {
    int i;
    for (i = 0; i <= MAX_NUM_REGISTERS; i++) {
        if (strcmp(validName, registers[i].name) == 0)
            return registers[i].value;
    }
    return FALSE;
}
