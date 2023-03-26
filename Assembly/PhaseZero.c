#include "PhaseZero.h"
#include "GeneralList.h"

void WriteMcr(FILE *inputFile, int end, FILE *outFile) {
    while (ftell(inputFile) != end)
        fputc(fgetc(inputFile), outFile);
}

FILE *openAndCheckFile(char *fileName) {
    FILE *inputFile = open_file(fileName, "as", "r");
    if (!inputFile) {
        printf("Error: unable to open file %s\n", fileName);
        return NULL;
    }
    if (feof(inputFile)) {
        printf("Error: The file is empty %s\n", fileName);
        fclose(inputFile);
        return NULL;
    }
    return inputFile;
}

int SearchAndAddMcr(char *fileName) {
    long prevLocation;
    FILE *inputFile, *outputFile;
    GeneralList mcrList = NULL, mcrTemp = NULL;
    char lineCheck[MAX_LINE_LENGTH] = {0}, lineCopy[MAX_LINE_LENGTH] = {0};
    char *curWord;
    if ((inputFile = openAndCheckFile(fileName)) == NULL)
        return FALSE;
    if ((outputFile = open_file(fileName, "am", "w")) == NULL) {
        fclose(inputFile);
        printf("\"Error: unable to open file %s.am\n", fileName);
        return FALSE;
    }
    printf("Execution of PreAssembler on the file: %s.as\n", fileName);
    while (!feof(inputFile)) { /*over all file, line by line*/
        memset(lineCheck, 0, MAX_LINE_LENGTH);
        memset(lineCopy, 0, MAX_LINE_LENGTH);
        fgets(lineCheck, MAX_LINE_LENGTH, inputFile);
        strcpy(lineCopy, lineCheck);
        curWord = strtok(lineCheck, " \t\n\v\f\r"); /* get the first word in the line. */
        if (curWord != NULL && strcmp(curWord, "mcr") == 0) { /*mcr start detection*/
            curWord = strtok(NULL, " \t\n\v\f\r"); /* get the next word in the line. */
            if (handel_label_name(curWord)) {
                mcrTemp = createLinked(curWord, 0);
                setStartId(mcrTemp, ftell(inputFile));/*Save macro start position*/
                while (!curWord || strcmp(curWord, "endmcr") != 0) {
                    setEndId(mcrTemp, ftell(inputFile));/*Save macro end position*/
                    fgets(lineCheck, MAX_LINE_LENGTH, inputFile);
                    curWord = strtok(lineCheck, " \t\n\v\f\r");
                }/*We will go through the text until we find an ending word for the macro (given that it always exists)*/
                InsertLinked(&mcrList, mcrTemp);/*Insert a macro into the list of macros*/
            } else {
                printf("Error:invalid macro name: '%s'\n Sorry PreAssembler on file: %s.as did not succeed!\n", curWord,
                       fileName);
                return FALSE;
            }
        } else {
            mcrTemp = (SearchLink(mcrList, curWord));/*Checking for potential macro words*/
            if (mcrTemp != NULL) { /* if it's a mcr */
                prevLocation = ftell(inputFile);
                fseek(inputFile, getStartId(mcrTemp), SEEK_SET);
                /* we write the macro's code instead of the macro call */
                WriteMcr(inputFile, getEndId(mcrTemp), outputFile);
                setUsed(mcrTemp, TRUE);
                fseek(inputFile, prevLocation, SEEK_SET);
            } else {
                fprintf(outputFile, "%s", lineCopy);/*Copy the row without change*/
            }
        }

    }
    if (mcrList != NULL)
        allUsed(mcrList, 0);/*this method checks for unused macros*/
    fclose(inputFile);
    fclose(outputFile);
    freelist(mcrList);
    printf("we have successfully completed the process PreAssembler on file: %s.as\n", fileName);
    return TRUE;
}


