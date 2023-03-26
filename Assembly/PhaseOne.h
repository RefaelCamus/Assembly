#ifndef PHASEONE_H
#define PHASEONE_H

#include "GeneralFunctions.h"
#include "GeneralList.h"

#define FLAGS 5

/* numbers to identify the flags cell */
enum flagsNames {
    ICcounter, DCcounter, EX, EN, ERROR
};

/*
    This method closes open file and delete files

    parameters:
                @param inputFile - the original received file
                @param checksEntry - file to be written all entry label in it
                @param checksLabelFile - file to be written all labels in it

    return:
                @return non
*/
void close_files(FILE *inputFile, FILE *checksLabelFile, FILE *checksEntry);

/*
    This method received a file with all labels with .entry declaration that appear
    in the original file, and checks that the label is defined in the symbol list

    parameters:
                @param checksEntry - file to be written all entry labels in it
                @param symbolList - the symbol list
    return:
                @return TRUE if the label exist in the symbol list, otherwise FALSE
*/
int checksLabelEntry(FILE *checksEntry, GeneralList *symbolList);

/*
    This method received a file with all labels  that appear in the original file,
    and checks that the label is defined in the symbol list

    parameters:
                @param checksLabelFile -  file to be written all label on it
                @param symbolList - the symbol list
    return:
               @return TRUE if the label exist in the symbol list, otherwise FALSE
*/
int checksLabelNotFound(FILE *checksLabelFile, GeneralList *symbolList);

/*
    This method handles with .entry instructions

    parameters:
                @param symbolList -the symbol list
                @param tag - the current line
                @param DC -  a pointer to the DC
                @param checksEntry - file to be written all entry label on it
                @param countLine - the number of the current line processed in the input file
                @param symbolTemp - the created symbol of that instruction

    return:
                 @return FALSE if an error occurred,otherwise TRUE

*/
int
handel_entry(GeneralList *symbolList, char *tag, int *DC, FILE *checksEntry, int countLine, GeneralList *symbolTemp);

/*
    This method handles with .extern instructions

    parameters:
                @param symbolList -the symbol list
                @param tag - the current line
                @param DC -  a pointer to the DC
                @param countLine - the number of the current line processed in the input file
                @param symbolTemp - the created symbol of that instruction
    return:
                 @return FALSE if an error occurred,otherwise TRUE

*/
int handel_extern(GeneralList *symbolList, char *tag, int *DC, int countLine, GeneralList *symbolTemp);

/*
    This method handles with .string instructions

    parameters:
                @param symbolList -the symbol list
                @param line - the current line
                @param DC -  a pointer to the DC
                @param countLine - the number of the current line in the input file
                @param symbolTemp - the created symbol of that instruction

    return:
                @return FALSE if an error occurred,otherwise TRUE

*/
int handel_string(GeneralList *symbolList, char *line, int *DC, int countLine, GeneralList *symbolTemp);

/*
    This method handles with .data instructions

    parameters:
                @param symbolList -the symbol list
                @param line - the current line
                @param DC -  a pointer to the data counter
                @param countLine - the number of the current line in the input file
                @param symbolTemp - the created symbol of that instruction

    return:
                 @return FALSE if an error occurred,otherwise TRUE

*/
int handel_data(GeneralList *symbolList, char *line, int *DC, int countLine, GeneralList *symbolTemp);

/*
    This method handles a sentence with one Operand , by checking its validation, and updates the IC
    parameters:
                @param command - the command of the command sentence
                @param IC - a pointer to the IC
                @param checksLabelFile - file to be written all label on it
                @param countLine - the number current line processed in the input file
                @param restOfLine - string that include 1 operand

    return:
                @return weather it found any errors in the line

*/
int ValidOneOperand(char *command, int *IC, FILE *checksLabelFile, int countLine, char restOfLine[]);

/*
    This method handles a command sentence with two Operands, by checking its validation, and updates the IC
    parameters:
                @param command - the command of the command sentence
                @param IC - a pointer to the IC
                @param checksLabelFile - file to be written all label on it
                @param countLine - the number current line processed in the input file
                @param restOfLine - string that include 2 operands

    return:
                @return weather it found any errors in the line

*/
int ValidTwoOperand(char *command, int *IC, FILE *checksLabelFile, int countLine, char restOfLine[]);

/*
    This method handles a command sentence, by checking its validation, and calling the correspondence functions
    parameters:
                @param command - the command of the command sentence
                @param IC - a pointer to the IC
                @param checksLabelFile - file to be written all label on it
                @param countLine - the number current line in the input file

    return:
                @return weather it found any errors in the line

*/
int handel_command_sentence(char *command, int *IC, FILE *checksLabelFile, int countLine);

/*
    This method handles checks if an instruction sentence is valid, it checks for errors
    parameters:
                @param symbolList -  a pointer to the symbol list
                @param curWord -  the instruction we got in the line
                @param DC - a pointer to the DC
                @param checksEntry - file to be written all entry label on it
                @param countLine - the number current line in the input file
                @param symbolTemp - the new symbol
    return:
                @return FALSE if any errors were found, TRUE otherwise

*/
int handel_instructions(GeneralList *symbolList, char *curWord, int *DC, FILE *checksEntry, int countLine,
                        GeneralList *symbolTemp, int *markValues);

/*
    This method handles a label definition, and creates a new node in the symbol list
    parameters:
                @param symbolList -  a pointer to the symbol list
                @param labelName -  the instruction we have in the line
                @param IC - a pointer to the IC
                @param countLine - the number current line in the input file
                @param symbolTemp - the new symbol
    return:
                @return FALSE if any errors were found, TRUE otherwise

*/
int handel_label(GeneralList *symbolList, char *labelName, int IC, int countLine, GeneralList *symbolTemp);

/*
    This method is the main function in this file and do all valid checks on the received file
    parameters:
                @param fileName -  the name of the file to be checked
                @param markValues - flags
    return:
                @return symbolTable the created symbol list, NULL if error occurred

*/
GeneralList SearchAndAddSymbol(char *fileName, int *markValues);

#endif


