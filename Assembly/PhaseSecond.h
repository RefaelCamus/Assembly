#ifndef SECONDPASSH
#define SECONDPASSH

#include "PhaseOne.h"
#include "Address.h"

/*
    This method convert a given parameter to base 14

    parameters:
                @param parameter -the parameter to be converted
                @param flag - checks whether the parameter is number or string
                @param dataFile -  a data file to write into
                @param DC - a pointer to the DC
    return:
                @return non

*/
void toBase2(char *parameter, int flag, FILE *dataFile, int *DC);

/*
    This method convert a given parameter to base 12

    parameters:
                @param number -the parameter to be converted
                @param ARE -bits indicate the type of encoding, is it absolute, external, requires relocation
                @param structionsFile -  a structions file to write into
                @param IC - a pointer to the DC
    return:
                @return non

*/
void OperandToBase2(int number, int ARE, FILE *structionsFile, int *IC);

/*
    This method encode the opcode of the parameter

    parameters:
                @param opcode -the opcode of the parameter
                @param structionsFile -  a structions file to write into
    return:
                @return non

*/
void writeOpcode(int opcode, FILE *structionsFile);

/*
    This method encode According to cases

    parameters:
                @param encode -the cases of encode
                @param structionsFile -  a structions file to write into
    return:
                @return non

*/
void writeTwoBit(int encode, FILE *structionsFile);

/*
    This method convert a given parameter to base 2

    parameters:
                @param parameter1 - the first operand of the string that be encoded
                @param parameter2 - the second  operand of the string that be encoded
                @param ARE -bits indicate the type of encoding, is it absolute, external, requires relocation
                @param destination - address of destination operand
                @param source - address of source operand
                @param opcode - the operations of the parameter
                @param structionsFile -  a structions file to write into
                @param IC - a pointer to the IC
    return:
                @return non

*/
void
writeFirstWord(int parameter1, int parameter2, int opcode, int source, int destination, int ARE, FILE *structionsFile,
               int *IC);

/*
    This method encode data instructions

    parameters:
                @param lineCheck - the string that be encoded
                @param DC - a pointer to the DC
                @param dataFile -  a data file to write into
    return:
                @return non

*/
void encoding_data(char *lineCheck, int *DC, FILE *dataFile);

/*
    This method encode string instructions

    parameters:
                @param lineCheck - the string that be encoded
                @param IC - a pointer to the IC
                @param dataFile -  a data file to write into
    return:
                @return non

*/
void encoding_string(char *lineCheck, int *DC, FILE *dataFile);

/*
    This method sends to the corresponding encoding method

    parameters:
                @param symbolList - the symbol lList
                @param instruction - the instructions that should be encoded
                @param DC -  pointer to the DC
                @param dataFile -  a data file to write into
    return:
                @return non
*/
void handle_instructions(GeneralList *symbolList, char *instruction, int *DC, FILE *dataFile);

/*
   This method is used to encode a one operand command

    parameters:
                @param symbolList - the symbol lList
                @param command - the command that should be encoded
                @param IC -  pointer to the IC
                @param structionsFile - file that we write our structions into
                @param extFile - file that we write our externals into
    return:
                @return non
*/
void encoding_one_operands(GeneralList *symbolList, char *command, int *IC, FILE *structionsFile, FILE *extFile);

/*
  This method is used to encode a two operand command

    parameters:
                @param symbolList - the symbol lList
                @param command - the command that should be encoded
                @param IC -  pointer to the IC
                @param structionsFile - file that we write our structions into
                @param extFile - file that we write our externals into
    return:
                @return non
*/
void encoding_two_operands(GeneralList *symbolList, char *command, int *IC, FILE *structionsFile, FILE *extFile);

/*
  This method is handel with 2 address

    parameters:
                @param symbolList - the symbol lList
                @param command - the command that should be encoded
                @param IC -  pointer to the IC
                @param source - address of source operand
                @param destination - address of destination operand
                @param firstOperand - the first operand of the string
                @param secondOperand - the second operand of the string
                @param structionsFile - a structions file to write into
                @param extFile -  an extern file to write into

    return:
                @return non
*/
void
handleTwoAddress(GeneralList *symbolList, char *command, int *IC, addressingMode source, addressingMode destination,
                 char *firstOperand, char *secondOperand, FILE *structionsFile, FILE *extFile);

/*
     This method encode an operand with an immediate address

    parameters:
                @param operand - the operand to encode
                @param IC -  pointer to the IC
                @param structionsFile - a structions file to write into
    return:
                @return non
*/
void handle_immediate(char *operand, int *IC, FILE *structionsFile);

/*
     This method encode an operand with direct address

    parameters:
                @param symbolList - the symbol lList
                @param operand - the operand to encode
                @param IC -  pointer to the IC
                @param structionsFile - a structions file to write into
                @param extFile - a extern file to write into
    return:
                @return non
*/

void handle_direct(GeneralList *symbolList, char *operand, int *IC, FILE *structionsFile, FILE *extFile);

/*
     This method corresponding function and handel the address

    parameters:
                @param operandMode - addressing mode of the operand
                @param symbolList - the symbol lList
                @param operand - the operand that should be encoded
                @param IC -  pointer to the IC
                @param structionsFile - a structions file to write into
                @param extFile -  an extern file to write into
                @param firstOrSecond - weather the operand is a destination operand or source operand

    return:
                @return non
*/
void handleAddress(addressingMode operandMode, GeneralList *symbolList, char *operand, int *IC,
                   FILE *structionsFile, FILE *extFile, int firstOrSecond);

/*
    This method is the general function that handles command sentences
    by calling the corresponding function to how many operands the command takes

    parameters:
                @param symbolList - the symbol lList
                @param command - the command that should be encoded
                @param IC -  pointer to the IC
                @param structionsFile - a structions file to write into
                @param extFile -  an extern file to write into

    return:
                @return non
*/
void encoding_sentence(GeneralList *symbolList, char *command, int *IC, FILE *structionsFile, FILE *extFile);

/*
    This method terminates the second phase
    it closes files, frees memory, and delete files

    parameters:
                @param fileName -the name of the file
                @param symbolList - the symbol lList
                @param inputFile the am file we read and encode
                @param entFile the entry file we created to store our entities
                @param extFile the extern file we created to store out externals calls
                @param structionsFile the structions file we created to store our commands encoding
                @param dataFile the .data file we created to store out data encoding

    return:
                @return non
*/
void end_second_phase(char *fileName, GeneralList *symbolList, FILE *inputFile, FILE *entFile, FILE *extFile,
                      FILE *structionsFile, FILE *dataFile);

/*
    This method handles the final output files, by combining them into the .ob file

    parameters:
                @param fileName - the name of the file
                @param structionsFile - the structions file we created to store our commands encoding
                @param dataFile - the .data file we created to store out data encoding

    return:
                @return non
*/
void final_files(char *fileName, FILE *structionsFile, FILE *dataFile);

/*
   This method opens all the files needed for the second phase

    parameters:
                @param fileName - the name of the file
                @param inputFile -  a pointer to the input file
                @param structionsFile -  a pointer to the structions file
                @param dataFile -  a pointer to the data file
                @param entFile - a pointer to the entry file
                @param extFile - a pointer to the extern file
                @param markValues -  flag
    return:
                @return non
*/
void openFiles(char *fileName, FILE **inputFile, FILE **structionsFile, FILE **dataFile, FILE **extFile,
               int *markValues);

/*
   This method  handle entry instructions

    parameters:
                @param fileName - the name of the file
                @param symbolList -  the symbol list
                @param entFile - file that we write our entry
                @param markValues - flag
    return:
                @return non
*/
void handle_entry(char *fileName, GeneralList *symbolList, FILE **entFile, int *markValues);

/*
   This method encoding the assembler at the last phase

    parameters:
                @param fileName - the name of the file
                @param symbolList -  the symbol list
                @param markValues - flag


    return:
                @return non
*/
int encoding_second_pass(char *fileName, GeneralList *symbolList, int *markValues);

#endif






