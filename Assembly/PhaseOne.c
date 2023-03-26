#include "PhaseOne.h"
#include "Address.h"
#include "Operation.h"


void close_files(FILE *inputFile, FILE *checksLabelFile, FILE *checksEntry) {
    fclose(inputFile);
    fclose(checksLabelFile);
    fclose(checksEntry);
    removeFile("EntryFile", "am");
    removeFile("LabelFile", "am");
}

int checksLabelEntry(FILE *checksEntry, GeneralList *symbolList) {
    char lineCheck[MAX_LINE_LENGTH] = {0};
    GeneralList symbolTemp = NULL;
    char *currentLabel;
    fseek(checksEntry, 0, SEEK_SET);
    while (!feof(checksEntry)) {
        currentLabel = NULL;
        memset(lineCheck, 0, MAX_LINE_LENGTH);
        /* iterating through each line(in every line exist 1 label) of the checksEntry file */
        fgets(lineCheck, MAX_LINE_LENGTH, checksEntry);
        currentLabel = strtok(lineCheck, " \t\n\v\f\r");
        if (!currentLabel)/* There are empty line */
            return TRUE;
        symbolTemp = SearchLink(*symbolList, currentLabel);/*checks if the label is defined in the symbol list */
        if ((symbolTemp) != NULL &&
            (getType(symbolTemp) != EXTERNAL)) {/*make sure that the label is not  defined as EXTERNAL type*/
            setType(symbolTemp, ENTRY);/* if it's a legal label, type defined as ENTRY */
            continue;
        }
        if (!(symbolTemp) || getType(symbolTemp) == EXTERNAL) {
            if (!(symbolTemp))
                printf("no declaration on this label\n");
            else
                printf("a label can be entry or external, but prohibited both\n");
            return FALSE;
        }
    }
    return TRUE;
}

int checksLabelNotFound(FILE *checksLabelFile, GeneralList *symbolList) {
    int flag = TRUE;
    char lineCheck[MAX_LINE_LENGTH] = {0};
    GeneralList symbolTemp = NULL;
    char *currentLabel = NULL;
    fseek(checksLabelFile, 0, SEEK_SET);
    while (!feof(checksLabelFile)) {
        memset(lineCheck, 0, MAX_LINE_LENGTH);
        /* iterating through each line(in every line exist 1 label) of the checksLabelFile file */
        fgets(lineCheck, MAX_LINE_LENGTH, checksLabelFile);
        currentLabel = strtok(lineCheck, " \t\n\v\f\r");
        if (!currentLabel) {
            printf("\n");
            return flag;
        }
        symbolTemp = SearchLink(*symbolList, currentLabel);/*checks if the label is defined in the symbol list */
        if (symbolTemp != NULL) {
            setUsed(symbolTemp, TRUE);
            continue;
        } else {
            if (flag)
                printf("The labels is not declared in the code: '%s'", currentLabel);
            else
                printf(",'%s'", currentLabel);
            flag = FALSE;
        }
    }
    return TRUE;
}

int
handel_entry(GeneralList *symbolList, char *tag, int *DC, FILE *checksEntry, int countLine, GeneralList *symbolTemp) {
    char restOfLine[MAX_LINE_LENGTH];
    if (*symbolTemp != NULL) { /* if had a declaration of a label before .entry , then we delete the symbol */
        findWarning("found an entry instruction with a symbol declared to it. new symbol got deleted.", countLine);
        moveHead(symbolList);
    }
    if (tag != NULL) {
        tag[strlen(tag) - 1] = '\0'; /* we are cutting the /n from it,*/
        memset(restOfLine, 0, MAX_LINE_LENGTH);
        copy(restOfLine, tag);
        LeftSpaces(restOfLine);
        tag[strlen(tag) - 1] = '\0'; /* we are cutting the colon(:) from it,*/
        if (restOfLine[0] != '\0' &&
            handel_label_name(restOfLine)) {/* if it .entry declaration and all valid, write it in entry file*/
            fprintf(checksEntry, "%s\n", restOfLine);
            return TRUE;
        }
        if (restOfLine[0] != '\0')
            printf("Error occurred at line %d: Invalid label name: %s\n", countLine, tag);
        else
            findError("Found an entry instruction without any label!", countLine);
        return FALSE;
    }
    findError("Found an entry instruction without any label!", countLine);
    return FALSE;
}

int handel_extern(GeneralList *symbolList, char *tag, int *DC, int countLine, GeneralList *symbolTemp) {
    if (*symbolTemp != NULL) { /* if had a declaration of a label before .extern , then we delete the symbol */
        findWarning("found an extern instruction with a symbol declared to it. new symbol got deleted.", countLine);
        moveHead(symbolList);
    }
    if (tag != NULL && handel_label(symbolList, tag, *DC, countLine, symbolTemp)) {
        setType(*symbolTemp, EXTERNAL);/*add the type of label, if it all valid and .extern declaration */
        setAddress(*symbolTemp, 0);
        return TRUE;
    }
    if (!tag)
        findError("Found an extern instruction without any label!", countLine);
    return FALSE;
}

int handel_string(GeneralList *symbolList, char *line, int *DC, int countLine, GeneralList *symbolTemp) {
    int i = 0;
    int Length = 0;
    int validStr = 0;
    char restOfLine[MAX_LINE_LENGTH] = {0};/* store the received line  */
    if (*symbolTemp != NULL) {
        setAddress(*symbolTemp, *DC);
        setType(*symbolTemp, DATA);
    }
    if (copy(restOfLine, line)) {
        /*remove white characters from the left and right of the string */
        RightSpaces(restOfLine);
        LeftSpaces(restOfLine);
        if (restOfLine[i] != '\0') {
            if (restOfLine[i] == '\"') { /*String must start with " */
                for (i = 1;
                     i < strlen(restOfLine) && restOfLine[i] != '\n' && restOfLine[i] != '\0' && !validStr; i++) {
                    Length++;
                    if (restOfLine[i] == '\"')
                        validStr = TRUE;/* valid string, ending with "*/
                }
                if (validStr && Length != 1) {
                    *DC = *DC + Length; /*increase DC according to the amount of chars*/
                    if (i == strlen(restOfLine))
                        return TRUE;
                    else {
                        findError("Found invalid text after string", countLine);
                        return FALSE;
                    }
                } else {
                    if (!validStr)
                        findError("valid string must end with \"", countLine);
                    else
                        findError("Missing chars in .string instruction", countLine);
                    return FALSE;
                }
            }
            findError("valid string must start with \"", countLine);
            return FALSE;
        }
        findError("only white spaces its not allow in string instruction", countLine);
        return FALSE;
    }
    findError("Missing string in .string instruction!", countLine);
    return FALSE;
}

int handel_data(GeneralList *symbolList, char *line, int *DC, int countLine, GeneralList *symbolTemp) {
    int foundNumbers = 0;
    char *nextNumber = NULL;
    char restOfLine[MAX_LINE_LENGTH * 2] = {0};
    char tempNumber[MAX_LINE_LENGTH] = {0};
    if (*symbolTemp != NULL) {
        setAddress(*symbolTemp, *DC);
        setType(*symbolTemp, DATA);
    }
    if (addSpace(line, restOfLine)) { /*adding space after comma*/
        if ((restOfLine[0] != '\0') && (restOfLine[0] != ',') && (restOfLine[strlen(restOfLine) - 2] != ',')) {
            nextNumber = strtok(restOfLine, ",");
            while (nextNumber != NULL) {
                memset(tempNumber, 0, MAX_LINE_LENGTH);
                foundNumbers = TRUE;
                copy(tempNumber, nextNumber);
                LeftSpaces(tempNumber); /*remove white characters from the left of the string */
                if (tempNumber[0] != '\0' &&
                    validNumber(tempNumber, TRUE)) {/*flag used to mark that the number cloud be 14 bit */
                    (*DC)++;/*increase DC according to the amount of numbers*/
                    nextNumber = strtok(NULL, ",");
                    continue;
                } else {
                    if (tempNumber[0] == '\0')
                        findError("between two commas must be number", countLine);
                    else
                        printf("Error occurred at line %d: number '%s' invalid in .data instruction! \n", countLine,
                               tempNumber);
                    return FALSE;
                }
            }
            if (!foundNumbers)
                findError("found a .data instruction without any numbers", countLine);
            return foundNumbers;
        }
    }
    if (!line || restOfLine[0] == '\0')
        findError("Missing numbers in .data instruction", countLine);
    else if (restOfLine[0] == ',')
        findError("Invalid ',' at the beginning of .data instruction", countLine);
    else
        findError("Invalid ',' at the end of .data instruction", countLine);
    return FALSE;
}

int ValidOneOperand(char *command, int *IC, FILE *checksLabelFile, int countLine, char restOfLine[]) {
    char firstOperand[MAX_LINE_LENGTH] = {0};
    char secondOperand[MAX_LINE_LENGTH] = {0};
    addressingMode source;
    addressingMode destination;
    char *tag;
    /* we handle special commands (jmp/bne\jsr) */
    if ((strcmp(command, "jmp") == 0) || (strcmp(command, "bne") == 0) || (strcmp(command, "jsr") == 0)) {
        /*  if ')' exist in the end we will relate to it like jumpWithParametersAddress cass*/
        if (restOfLine[strlen(restOfLine) - 1] == ')') {
            tag = strtok(restOfLine, "(");
            fprintf(checksLabelFile, "%s\n", tag);
            copy(firstOperand, strtok(NULL, ","));
            copy(secondOperand, strtok(NULL, ")"));
            /*flag used to mark witch labels should be saved for validation in the checksLabelFile file */
            source = handel_address(firstOperand, checksLabelFile, TRUE);
            destination = handel_address(secondOperand, checksLabelFile, TRUE);
            if (source != NON && destination != NON && handel_label_name(tag)) {
                if (source == directRegisterAddress && destination == directRegisterAddress)
                    (*IC) = (*IC) + 2;/*if substring bne LOOP(r4,r5)*/
                else
                    (*IC) = (*IC) + 3;/*if substring jmp L1(#-1,r6)*/
                return TRUE;
            }
            if (source == NON)
                findError("Invalid definition of firstOperands name!", countLine);
            else if (destination == NON)
                findError("Invalid definition of secondOperand name!", countLine);
            else
                findError("Invalid label name!", countLine);
            return FALSE;
        } else {/* we handle special commands (jmp/bne\jsr) but with one operand*/
            tag = strtok(restOfLine, "");
            if (handel_label_name(tag)) {
                (*IC)++;/* increase IC to point the next cell */
                fprintf(checksLabelFile, "%s\n", tag);
                return TRUE;
            }
            findError("Invalid Operand that should be label !", countLine);
            return FALSE;
        }
    } else {/* not the special commands but with one operand  */
        /*flag used to mark witch labels should be saved for validation in the checksLabelFile file */
        destination = handel_address(restOfLine, checksLabelFile, TRUE);
        if (!(destination == immediateAddress && (strcmp(command, "prn") != 0)) &&
            (destination != NON)) {/*check all the forbidden conjunctions and do negative */
            (*IC)++;/* increase IC to point the next cell */
            return TRUE;
        }
        if (destination == NON)
            findError("Invalid definition of a Operand!", countLine);
        else
            findError("Invalid destination address!", countLine);
        return FALSE;
    }
}

int ValidTwoOperand(char *command, int *IC, FILE *checksLabelFile, int countLine, char restOfLine[]) {
    char firstOperand[MAX_LINE_LENGTH] = {0};
    char secondOperand[MAX_LINE_LENGTH] = {0};
    addressingMode source;
    addressingMode destination;
    copy(firstOperand, strtok(restOfLine, ","));
    copy(secondOperand, strtok(NULL, ""));
    LeftSpaces(secondOperand);
    RightSpaces(firstOperand);
    /*flag used to mark witch labels should be saved for validation in the checksLabelFile file */
    source = handel_address(firstOperand, checksLabelFile, TRUE);
    destination = handel_address(secondOperand, checksLabelFile, TRUE);
    if (source != NON && destination != NON) {
        /*check all the forbidden conjunctions and do negative */
        if (!((source == immediateAddress || source == directRegisterAddress) && (strcmp(command, "lea") == 0)) &&
            (!(destination == immediateAddress && (strcmp(command, "cmp") != 0)))) {
            if (source == directRegisterAddress && destination == directRegisterAddress)
                (*IC)++;/* increase IC counter for 2 register parameter like r4,r5 */
            else
                (*IC) = (*IC) + 2; /* increase IC counter for 2 parameter accept 2 register*/
            return TRUE;
        } else {
            if ((source == immediateAddress || source == directRegisterAddress) && (strcmp(command, "lea") == 0))
                findError("Invalid source address!", countLine);
            else
                findError("Invalid destination address!", countLine);
            return FALSE;
        }
    }
    if (source == NON)
        findError("Invalid definition of firstOperands name!", countLine);
    else
        findError("Invalid definition of secondOperand name!", countLine);
    return FALSE;
}


int handel_command_sentence(char *command, int *IC, FILE *checksLabelFile, int countLine) {
    int opNumber;
    int flag;
    char restOfLine[MAX_LINE_LENGTH] = {0};
    opNumber = numOfOperation(command);
    (*IC)++;/* increase IC counter */
    if (opNumber == 0) {/* if the command doesn't require any operands */
        if (!copy(restOfLine, strtok(NULL, " \t\n\v\f\r")))
            return TRUE;
        findError("for this command no Operand allowed or any characters", countLine);
        return FALSE;
    }
    flag = copy(restOfLine, strtok(NULL, ""));
    if (flag && restOfLine[0] != ',') {
        LeftSpaces(restOfLine);
        RightSpaces(restOfLine);
        if (opNumber == 1) /* if the command require one operand */
            return ValidOneOperand(command, IC, checksLabelFile, countLine, restOfLine);
        else /* if the command require two operands */
            return ValidTwoOperand(command, IC, checksLabelFile, countLine, restOfLine);
    }
    if (!flag) /* if its empty - Missing operand*/
        findError("Missing operand", countLine);
    else  /* if it starts with a comma, it we have illegal comma */
        findError("illegal commas found right after the command name", countLine);
    return FALSE;
}

int handel_instructions(GeneralList *symbolList, char *curWord, int *DC, FILE *checksEntry, int countLine,
                        GeneralList *symbolTemp, int *markValues) {
    if (!strcmp(".data", curWord))
        return handel_data(symbolList, strtok(NULL, ""), DC, countLine, symbolTemp);
    if (!strcmp(".string", curWord))
        return handel_string(symbolList, strtok(NULL, ""), DC, countLine, symbolTemp);
    if (!strcmp(".entry", curWord)) {
        markValues[EN] = EXIST; /* marks that we have entry label  */
        return handel_entry(symbolList, strtok(NULL, ""), DC, checksEntry, countLine, symbolTemp);
    }
    if (!strcmp(".extern", curWord)) {
        markValues[EX] = EXIST;/* marks that we have extern label  */
        return handel_extern(symbolList, strtok(NULL, ""), DC, countLine, symbolTemp);
    }
    findError("found an illegal instruction", countLine);
    return FALSE;
}

int handel_label(GeneralList *symbolList, char *labelName, int IC, int countLine, GeneralList *symbolTemp) {
    char restOfLine[MAX_LINE_LENGTH];
    memset(restOfLine, 0, MAX_LINE_LENGTH);
    labelName[strlen(labelName) - 1] = '\0'; /*we are cutting '\n' from it*/
    copy(restOfLine, labelName);
    LeftSpaces(restOfLine);
    if (handel_label_name(restOfLine)) {
        *symbolTemp = SearchLink(*symbolList, restOfLine);
        if (!(*symbolTemp) &&
            (!savedWord(restOfLine))) {/*check that we have a legal label and exist in the symbolList*/
            *symbolTemp = createLinked(restOfLine, IC);
            InsertLinked(symbolList, *symbolTemp);/*we have a legal label --> add to the symbol list*/
            return TRUE;
        }
        if ((*symbolTemp) != NULL)
            findError("Found multiple definition of the same label", countLine);
        else
            findError("label can't be a reserved word", countLine);
        return FALSE;
    }
    if (restOfLine[0] != '\0')
        printf("Error occurred at line %d: Invalid label name: %s\n", countLine, restOfLine);
    else
        findError("the label cannot be empty", countLine);
    return FALSE;
}

GeneralList SearchAndAddSymbol(char *fileName,
                               int *markValues) {/* each cell of the markValues array represent a flag (5 cell - ICcounter, DCcounter, EX, EN, ERROR ) */
    FILE *inputFile, *checksLabelFile, *checksEntry;/* a FILE pointer to received file and 2 check file (Label and Entry) */
    char lineCheck[MAX_LINE_LENGTH] = {0};
    GeneralList symbolList = NULL, symbolTemp = NULL;
    char *curWord = NULL; /* Holds the current word */
    int countLine = 0;/* Holds the number of the current line */
    int IC = 100;
    int DC = 0;
    inputFile = open_file(fileName, "am", "r");/*open the received file for reading and check the process */
    /*open the Label File to write all labels that appear in received file, that we check after */
    checksLabelFile = open_file("LabelFile", "am", "w+");
    checksEntry = open_file("EntryFile", "am", "w+");/*open Entry File to write all labels with .entry declaration */
    if (!inputFile) {/* If the file couldn't be opened for reading */
        findError("Couldn't open input file!", countLine);
        markValues[ERROR] = EXIST;
        return NULL;
    }
    printf("Started first iteration on the file: %s...\n", fileName);
    while (!feof(inputFile)) { /*over all file, line by line*/
        symbolTemp = NULL;
        memset(lineCheck, 0, MAX_LINE_LENGTH); /* nullity (put 0 in all cell of) the array */
        fgets(lineCheck, MAX_LINE_LENGTH, inputFile); /*save the current line*/
        countLine++;
        curWord = strtok(lineCheck, " \t\n\v\f\r");/* cut the first word in the line */
        if (!curWord || curWord[0] == ';')/* if line is empty or note ( ';' represent a note) */
            continue;/*continue to the next line*/
        if (curWord[strlen(curWord) - 1] == ':') {/* if the current word is label (label must end with ':') */
            if (!handel_label(&symbolList, curWord, IC, countLine, &symbolTemp)) {/*checks validate of the label*/
                markValues[ERROR] = EXIST;/* if a occurred an error */
                continue;
            } else {
                curWord = strtok(NULL, " \t\n\v\f\r");/* cut the next word in the line */
                if (!curWord) {
                    findError("Found an empty label declaration", countLine);
                    markValues[ERROR] = EXIST;/* if a occurred an error */
                    continue;
                }
            }
        }
        /* Check if it's an instruction (starting with '.') */
        if (curWord[0] == '.') {
            if (!handel_instructions(&symbolList, curWord, &DC, checksEntry, countLine, &symbolTemp, markValues)) {
                markValues[ERROR] = EXIST;/* if a occurred an error */
            }
            continue;
        } /* if the current word is a command */
        if (validOpName(curWord)) {
            if (!handel_command_sentence(curWord, &IC, checksLabelFile, countLine)) {
                markValues[ERROR] = EXIST;/* if a occurred an error */
                continue;
            }
        } else {
            printf("Error occurred at line %d: Invalid command: '%s'\n", countLine, curWord);
            markValues[ERROR] = EXIST;/* if a occurred an error */
        }
    }
    update_IC_symbols(&symbolList,
                      IC); /* Updating all data and string symbol (add IC final value to their address counter) */
    if (!checksLabelEntry(checksEntry, &symbolList))
        markValues[ERROR] = EXIST;/* exist .entry label that not valid in the file*/
    if (!checksLabelNotFound(checksLabelFile, &symbolList))
        markValues[ERROR] = EXIST;/* exist label that not defined in the file*/
    markValues[ICcounter] = IC;/*save final IC to be use in the second phase*/
    markValues[DCcounter] = DC;/*save final DC to be use in the second phase*/
    close_files(inputFile, checksLabelFile, checksEntry);/* free all files*/
    if (symbolList != NULL)
        allUsed(symbolList, 1);/*this method checks for unused symbols*/
    if (markValues[ERROR] == EXIST) {/* if a error occurred we stop the process and free/delete all memory we used*/
        printf("First iteration on file: %s.am failed!\n", fileName);
        removeFile(fileName, "am");
        freelist(symbolList);/* free the linked-lists */
        return NULL;
    }
    printf("Finished first iteration on file: %s.am!\n", fileName);
    return symbolList;
}

