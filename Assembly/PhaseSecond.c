#include "PhaseSecond.h"
#include "Operation.h"
#include "Registers.h"
#include <stdlib.h>

void toBase2(char *parameter, int flag, FILE *dataFile, int *DC) {
    int i;
    int number;
    char *temp;
    *DC = *DC + 1;
    fprintf(dataFile, "0%d\t", *DC);/*write the current DC on the file */
    if (!flag)
        number = strtol(parameter, &temp, 10);/* convert the string to number in base 10 (we all ready check it) */
    else
        number = (int) (*parameter);/*the ascii value of the received char */
    for (i = 13; i >= 0; i--)/*encoding the input and write to the dataFile in bit 14*/
        (number & (1 << i)) ? fputc('/', dataFile) : fputc('.', dataFile);
    fprintf(dataFile, "\n");
}

void OperandToBase2(int number, int ARE, FILE *structionsFile, int *IC) {
    int i;
    fprintf(structionsFile, "0%d\t", *IC);/*write the current IC on the file */
    for (i = 11; i >= 0; i--) {/*encoding the input and write to the structionsFile in bit 12*/
        (number & (1 << i)) ? fputc('/', structionsFile) : fputc('.', structionsFile);
    }
    writeTwoBit(ARE, structionsFile);/* encoding the ARE on the file */
    fprintf(structionsFile, "\n");
}

void writeOpcode(int opcode, FILE *structionsFile) {
    int mask;
    int BIT = 3;
    while (BIT >= 0) {/*encoding the received opcode and write to the structionsFile*/
        mask = 1 << BIT;
        mask = opcode & mask;
        (mask != 0) ? fputc('/', structionsFile) : fputc('.', structionsFile);
        BIT--;
    }
}

void writeTwoBit(int encode, FILE *structionsFile) {/* encoding 2 bit on the file */
    if (encode == 0)
        fprintf(structionsFile, "..");/*00*/
    else if (encode == 1)
        fprintf(structionsFile, "./");/*01*/
    else if (encode == 2)
        fprintf(structionsFile, "/.");/*10*/
    else if (encode == 3)
        fprintf(structionsFile, "//");/*11*/
}


void
writeFirstWord(int parameter1, int parameter2, int opcode, int source, int destination, int ARE, FILE *structionsFile,
               int *IC) {
    *IC = *IC + 1;
    fprintf(structionsFile, "0%d\t", *IC);/*write the current IC on the file */
    /*encoding a binary line from all relevant fields*/
    writeTwoBit(parameter1, structionsFile);/* encoding 2 bit that represent parameter1 on the file */
    writeTwoBit(parameter2, structionsFile);/* encoding 2 bit that represent parameter2 on the file */
    writeOpcode(opcode, structionsFile);/* encoding 4 bit that represent opcode on the file */
    writeTwoBit(source, structionsFile);/* encoding 2 bit that represent source address on the file */
    writeTwoBit(destination, structionsFile);/* encoding 2 bit that represent destination address on the file */
    writeTwoBit(ARE, structionsFile);/* encoding 2 bit that represent ARE on the file */
    fprintf(structionsFile, "\n");
}

void encoding_data(char *lineCheck, int *DC, FILE *dataFile) {
    char *nextNumber = NULL;
    char restOfLine[MAX_LINE_LENGTH] = {0};
    copy(restOfLine, lineCheck);
    nextNumber = strtok(restOfLine, " \t\n\v\f\r,");/* cut the first number from the string */
    while (nextNumber != NULL) {
        toBase2(nextNumber, FALSE, dataFile, DC);/*encoding the input and write to the dataFile in bit 14*/
        nextNumber = strtok(NULL, " \t\n\v\f\r,");/* cut the next number from the string */
    }
}

void encoding_string(char *lineCheck, int *DC, FILE *dataFile) {
    int i;
    char restOfLine[MAX_LINE_LENGTH] = {0};
    copy(restOfLine, lineCheck);
    for (i = 1; restOfLine[i] != '\"'; i++) {/*over the string char by char*/
        toBase2(&restOfLine[i], TRUE, dataFile, DC);/*encoding the input and write to the dataFile in bit 14*/
    }
    *DC = *DC + 1;
    fprintf(dataFile, "0%d\t..............\n", *DC);/*encoding 0 line in the end of the string*/

}

void handle_instructions(GeneralList *symbolList, char *instruction, int *DC, FILE *dataFile) {
    if (!strcmp(".data", instruction))
        encoding_data(strtok(NULL, ""), DC, dataFile);
    if (!strcmp(".string", instruction))
        encoding_string(strtok(NULL, ""), DC, dataFile);
}

void encoding_one_operands(GeneralList *symbolList, char *command, int *IC, FILE *structionsFile, FILE *extFile) {
    int opCode = 0;/* the opCode of the command */
    char *operand = NULL;/* the operand of the command */
    addressingMode source = 0;/* the source address of the command */
    addressingMode destination = 0;/* the destination address of the command */
    char *firstOperand = NULL;
    char *secondOperand = NULL;
    opCode = whichOpcode(command);
    if ((strcmp(command, "jmp") == 0) || (strcmp(command, "bne") == 0) ||
        (strcmp(command, "jsr") == 0)) {/* we handle special commands (jmp/bne\jsr) */
        operand = strtok(NULL, "(\t\n\v\f\r ");/* cut the label operand from the line */
        firstOperand = strtok(NULL, " \t\n\v\f\r,");/* cut the first parameter from the line */
        if (firstOperand != NULL) {/*  if exist we will relate to it like jumpWithParametersAddress cass*/
            source = handel_address(firstOperand, NULL, FALSE);
            secondOperand = strtok(NULL, " \t\n\v\f\r,)");/* cut the second parameter from the line */
            destination = handel_address(secondOperand, NULL, FALSE);
            /*encoding the first word for the command as a binary line from all relevant fields*/
            writeFirstWord(source, destination, opCode, 0, jumpWithParametersAddress, 0, structionsFile, IC);
            /*encoding the label of the command as a binary line*/
            handleAddress(directAddress, symbolList, operand, IC, structionsFile, extFile, FALSE);
            /*encoding 2 parameters for the command as a binary line */
            handleTwoAddress(symbolList, command, IC, source, destination, firstOperand, secondOperand, structionsFile,
                             extFile);
        } else {/* we handle special commands (jmp/bne\jsr) but with one operand*/
            /*encoding the first word for the command as a binary line from all relevant fields*/
            writeFirstWord(0, 0, opCode, source, directAddress, 0, structionsFile, IC);
            /*encoding the label of the command as a binary line*/
            handleAddress(directAddress, symbolList, operand, IC, structionsFile, extFile, FALSE);
        }
    } else {
        operand = strtok(NULL, " \t\n\v\f\r ");/* cut the first operand from the line */
        destination = handel_address(operand, NULL, FALSE);
        /*encoding the first word for the command as a binary line from all relevant fields*/
        writeFirstWord(0, 0, opCode, source, destination, 0, structionsFile, IC);
        /*encoding the extra line of the addressingMode as a binary line*/
        handleAddress(destination, symbolList, operand, IC, structionsFile, extFile, FALSE);
    }
}

void encoding_two_operands(GeneralList *symbolList, char *command, int *IC, FILE *structionsFile, FILE *extFile) {
    int opCode = 0;/* the opCode of the command */
    addressingMode source = 0;/* the source address of the command */
    addressingMode destination = 0;/* the destination address of the command */
    char *firstOperand = NULL;
    char *secondOperand = NULL;
    opCode = whichOpcode(command);
    firstOperand = strtok(NULL, " \t\n\v\f\r,");/* cut the first parameter from the line */
    source = handel_address(firstOperand, NULL, FALSE);
    secondOperand = strtok(NULL, " \t\n\v\f\r,");/* cut the second parameter from the line */
    destination = handel_address(secondOperand, NULL, FALSE);
    /*encoding the first word for the command as a binary line from all relevant fields*/
    writeFirstWord(0, 0, opCode, source, destination, 0, structionsFile, IC);
    /*encoding the extra line of the addressingMode as a binary line*/
    handleTwoAddress(symbolList, command, IC, source, destination, firstOperand, secondOperand, structionsFile,
                     extFile);
}

void
handleTwoAddress(GeneralList *symbolList, char *command, int *IC, addressingMode source, addressingMode destination,
                 char *firstOperand, char *secondOperand, FILE *structionsFile, FILE *extFile) {
    if (destination == directRegisterAddress &&
        source == directRegisterAddress) {/* increase IC counter for 2 register parameter like r4,r5 */
        *IC = *IC + 1; /*encoding them as a 1 line*/
        OperandToBase2((whichRegisterValue(firstOperand) << 6) | (whichRegisterValue(secondOperand)), 0, structionsFile,
                       IC);
    } else { /*encoding the extra lines of the addressingModes as a binary line*/
        handleAddress(source, symbolList, firstOperand, IC, structionsFile, extFile, TRUE);
        handleAddress(destination, symbolList, secondOperand, IC, structionsFile, extFile, FALSE);
    }
}

void handle_immediate(char *operand, int *IC, FILE *structionsFile) {
    char *end;
    int operandToInt = 0;
    operandToInt = strtol(operand, &end, 10); /* convert the string to number in base 10 (we all ready check it) */
    OperandToBase2(operandToInt, 0, structionsFile, IC);/*encoding the number as a binary line on the file*/
}

void handle_direct(GeneralList *symbolList, char *operand, int *IC, FILE *structionsFile, FILE *extFile) {
    int ARE = 2;
    GeneralList symbolTemp = NULL;
    symbolTemp = SearchLink(*symbolList, operand);/* find the symbol a like the label*/
    if (getType(symbolTemp) == EXTERNAL) { /*if it's a EXTERNAL type, we write it to the extFile file */
        fprintf(extFile, "%s\t%d\n", getName(symbolTemp), *IC);
        ARE = 1;
    }/*encoding the label address as a binary line on the file*/
    OperandToBase2(getAddress(symbolTemp), ARE, structionsFile, IC);
}

void handleAddress(addressingMode operandMode, GeneralList *symbolList, char *operand, int *IC,
                   FILE *structionsFile, FILE *extFile, int firstOrSecond) {
    *IC = *IC + 1;
    if (operandMode == immediateAddress)
        handle_immediate(operand, IC, structionsFile);/* send it to be encoding the extra lines */
    else if (operandMode == directAddress)
        handle_direct(symbolList, operand, IC, structionsFile, extFile);/* send it to be encoding the extra lines */
    else {
        if (firstOrSecond)/*encoding the register number as a binary line on the file*/
            OperandToBase2((whichRegisterValue(operand) << 6), 0, structionsFile, IC);
        else
            OperandToBase2((whichRegisterValue(operand)), 0, structionsFile, IC);
    }
}


void encoding_sentence(GeneralList *symbolList, char *command, int *IC, FILE *structionsFile, FILE *extFile) {
    int opNumber;
    opNumber = numOfOperation(command);
    if (opNumber == 0)/*encode a none operands command*/
        writeFirstWord(0, 0, whichOpcode(command), 0, 0, 0, structionsFile, IC);
    if (opNumber == 1)/*call method that will encode one operand command*/
        encoding_one_operands(symbolList, command, IC, structionsFile, extFile);
    if (opNumber == 2)/*call method that will encode two operand command*/
        encoding_two_operands(symbolList, command, IC, structionsFile, extFile);
}

void end_second_phase(char *fileName, GeneralList *symbolList, FILE *inputFile, FILE *entFile, FILE *extFile,
                      FILE *structionsFile, FILE *dataFile) {
    fclose(inputFile);
    if (entFile != NULL) fclose(entFile);/*close the file if its be created */
    if (extFile != NULL) fclose(extFile);/*close the file if its be created */
    /*close and delete the files */
    fclose(structionsFile);
    fclose(dataFile);
    removeFile(fileName, "cmd");
    removeFile(fileName, "data");
}


void final_files(char *fileName, FILE *structionsFile, FILE *dataFile) {
    char c;
    FILE *obFile = open_file(fileName, "ob", "w");
    fseek(structionsFile, 0, SEEK_SET);
    fseek(dataFile, 0, SEEK_SET);
    /* Copy contents of the command instructions to the ob file */
    while ((c = fgetc(structionsFile)) != EOF)
        fputc(c, obFile);
    /* Copy contents of the data instructions to the ob file */
    while ((c = fgetc(dataFile)) != EOF)
        fputc(c, obFile);
    fclose(obFile);
}

void openFiles(char *fileName, FILE **inputFile, FILE **structionsFile, FILE **dataFile, FILE **extFile,
               int *markValues) {
    /* we will create extFile only if needed*/
    if (markValues[EX])
        *extFile = open_file(fileName, "ext", "w+");
    *inputFile = open_file(fileName, "am", "r");/*open the received file for reading */
    *structionsFile = open_file(fileName, "cmd", "w+");/*open file to write all command sentence  */
    *dataFile = open_file(fileName, "data", "w+");/*open file to write all data and string sentence  */
    fprintf(*structionsFile, "\t%d\t%d\n", markValues[ICcounter] - 100,
            markValues[DCcounter]); /* write the final IC,DC on the file*/
}

void handle_entry(char *fileName, GeneralList *symbolList, FILE **entFile, int *markValues) {
    GeneralList temp = *symbolList;
    if (markValues[EN]) { /* we will create entFile only if needed*/
        *entFile = open_file(fileName, "ent", "w+");
        while (temp != NULL) {
            if (getType(temp) ==
                ENTRY)/*we check all the symbolList, if the symbol is type of .entry we will write it on the entry file*/
                fprintf(*entFile, "%s\t%d\n", getName(temp), getAddress(temp));
            temp = getNext(temp);
        }
    }
}

int encoding_second_pass(char *fileName, GeneralList *symbolList, int *markValues) {
    FILE *inputFile = NULL, *structionsFile = NULL, *dataFile = NULL, *entFile = NULL, *extFile = NULL;/* a FILE pointer to files */
    char line[MAX_LINE_LENGTH] = {0};/* line from files*/
    int countLine = 0;
    char *curWord = NULL;
    int DC = markValues[ICcounter] - 1;/* Data counter */
    int IC = 99;/* Instruction counter */
    printf("Started second iteration on the file: %s...\n", fileName);
    openFiles(fileName, &inputFile, &structionsFile, &dataFile, &extFile, markValues);/* open all the file */
    handle_entry(fileName, symbolList, &entFile, markValues);/* handle entry file */
    while (!feof(inputFile)) {/*over all file, line by line*/
        memset(line, 0, MAX_LINE_LENGTH);
        fgets(line, MAX_LINE_LENGTH, inputFile);/*save the current line*/
        countLine++;
        curWord = strtok(line, " \t\n\v\f\r");/* cut the first word in the line */
        if (!curWord || curWord[0] == ';')/* if line is empty or note ( ';' represent a note) */
            continue;
        if (curWord[strlen(curWord) - 1] == ':')/* if the current word is label (label must end with ':') */
            curWord = strtok(NULL, " \t\n\v\f\r,");/*continue to the next line*/
        if (curWord[0] == '.')  /* Check if it's an instruction (starting with '.') */
            handle_instructions(symbolList, curWord, &DC, dataFile);
        else /*it must be legal we check it in the PhaseOne*/
            encoding_sentence(symbolList, curWord, &IC, structionsFile, extFile);
    }
    printf("Second pass has been finished successfully.\n");
    final_files(fileName, structionsFile, dataFile);/* combining 2 files in to final object file*/
    end_second_phase(fileName, symbolList, inputFile, entFile, extFile, structionsFile,
                     dataFile);/*  free/delete all memory we used*/
    return 0;
}
