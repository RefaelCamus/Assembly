#ifndef OPERATION_H
#define OPERATION_H

#define MAX_OPERATIONS 16
#define ONE_TWO_OPERATION 2
#define WITHOUT_FIRST_OPERATION 3
#define ALL_OPERATION 4

/*
    This method check if the given name is valid operation

    parameters
                @param command - tha name of the operation

    return
                @return TRUE weather it found that operation,otherwise FALSE
*/
int validOpName(char *command);

/*
    This method receives command name and return Opcode of her.

    parameters
                @param command - tha name of the operation

    return
                @return the Opcode
*/
int whichOpcode(char *command);

/*
    This method receives command name and return number of her.

    parameters
                @param command - tha name of the operation

    return
                @return int of the operand Number
*/
int numOfOperation(char *command);


#endif