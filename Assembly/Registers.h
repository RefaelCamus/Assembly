#ifndef REGISTERTABLE_H
#define REGISTERTABLE_H

#define MAX_NUM_REGISTERS 7

/*
    This method checks whether the name of the register valid.
    parameters:
                @param validName - tha name of the registers
    return:
                @return TRUE - weather it found a register with that name, Other FALSE
*/
int validRegName(char *validName);

/*
    This method receives name of register, if the name valid - return the register Number

    parameters:
                @param validName - tha name of the register
    return:
                @return int of the registers
*/
int whichRegisterValue(char *validName);

#endif