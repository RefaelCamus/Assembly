#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdio.h>
#include <string.h>

#define NON 9999/*no exist*/
#define EXIST 1
#define MAX_LINE_LENGTH 80
#define MAX_FILE_NAME 255
#define MAX_LABEL_LENGTH 30
#define TRUE 1
#define FALSE 0
#define MAX_NUM_14_BIT 16383
#define MIN_NUM_14_BIT -16383
#define MAX_NUM_12_BIT 4095
#define MIN_NUM_12_BIT -4095

/*
    This method adds the right extension to the file it receives

    parameters:
                @param fileN - the file's name
                @param extension - the extension of the file
                @param fullFileName - the string to write to
    return:
                @return non
*/
void fullNameToFile(char *fileN, char *extension, char *fullFileName);

/*
    This method opens the file according to the specific mode

    parameters:
                @param file_name - the file's name
                @param fileExtension - the file's extension
                @param mode - which mode open the file with
    return:
                @return open file
*/
FILE *open_file(char *file_name, char *fileExtension, char *mode);

/*
    This method deletes the current file

    parameters:
                @param fileN- the file's name
                @param file_with_extension - the file's extension
    return:
                @return non
*/
void removeFile(char *fileN, char *file_with_extension);

/*
    This method deletes the whiteSpaces at the end of a given string

    parameters:
                @param line - the string that we remove whiteSpaces from it
    return:
                @return none
*/
void LeftSpaces(char line[MAX_LINE_LENGTH]);

/*
    This method deletes the whiteSpaces at the beginning of a given string

    parameters:
                @param line - the string that we remove whiteSpaces from it
    return:
                @return none
*/
void RightSpaces(char line[MAX_LINE_LENGTH]);

/*
    This method copy a string to an array

    parameters:
                @param restOfLine - received the string
                @param line - string to be copy
    return:
                @return TRUE if it valid string, otherwise FALSE
*/
int copy(char restOfLine[MAX_LINE_LENGTH], char *line);

/*
    This method declare if we found an error and prints it on the screen

    parameters:
                @param errorMsg - the error message to be printed
                @param countLine - the number of line the error was occurred at
    return:
                @return none
*/
void findError(char *errorMsg, int countLine);

/*
    This method declare if we found a warning and prints it on the screen

    parameters:
                @param warningMsg - the warning message to be printed
                @param countLine - the number of line the error was occurred at
    return:
                @return none
*/
void findWarning(char *warningMsg, int numberOfLine);

/*
    This method checks if it's register/instructionName/command name

    parameters:
                @param word - the string to be checked
    return:
                @return TRUE if it savedWord, otherwise FALSE
*/
int savedWord(char *word);

/*
    This method checks if it's a valid number in the array

    parameters:
                @param number -the string to be check
    return:
                @return weather it's a valid number return TRUE,otherwise FALSE
*/
int validNumber(char number[MAX_LINE_LENGTH], int flag);

/*
    This method checks if a given label name is a valid

    parameters:
                @param name - the label to be check
    return:
                @return TRUE if it valid, otherwise FALSE
*/
int handel_label_name(char *name);

/*
    This method copy the string to array without white characters,
    and after any comma adds space

    parameters:
                @param line - the string we receive
                @param restOfLine - the array we save the string after adding space
    return:
                @return TRUE if the string not NULL, otherwise FALSE
*/

int addSpace(char *line, char restOfLine[MAX_LINE_LENGTH * 2]);

#endif
