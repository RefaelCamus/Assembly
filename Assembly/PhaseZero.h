#ifndef ASSEMBLER_ZERO_H
#define ASSEMBLER_ZERO_H

#include "GeneralFunctions.h"

/*
    This method is added to the output file the macro content

    parameters:
                @param inputFile - The original file without the macro layout
                @param end - location of the end of writing the macro
                @param outFile - the output am file to write into
    return:
                @return  non

*/
void WriteMcr(FILE *inputFile, int end, FILE *outFile);

/*
        This method open file and checks the validation of the received file

        parameters:
                    @param fileName - The received file name
        return:
                    @return  inputFile if it's valid, other null
*/
FILE *openAndCheckFile(char *fileName);

/*

    This method compresses the name of the macro and all the data related to the macro from the file,
    then a preAssembler step is performed which makes use of the macro

    parameters:
                @param fileName - the input file we get and make all the process

    return:
                @return FALSE if an error occurred , TRUE otherwise
*/
int SearchAndAddMcr(char *fileName);

#endif
