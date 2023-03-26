#ifndef LIST_H
#define LIST_H

/*A pointer to a linked list*/
typedef struct linked *GeneralList;
/*symbol types option*/
typedef enum Type {
    OPERATION,
    DATA,/*data and string*/
    ENTRY,
    EXTERNAL
} Type;

/*
    This method creates a new symbol

    parameters:
                @param name - the name of the new symbol
                @param address - the address of the new symbol
    return:
                @return the new symbol
*/
GeneralList createLinked(char *name, int address);

/*
    This method inserts a symbol to the symbol list

    parameters:
                @param head - a pointer to the head of the symbol list
                @param new_general_linked - the symbol to be insert
    return:
                @return none
*/
void InsertLinked(GeneralList *head, GeneralList new_general_linked);

/*
    This method checking if the name exists in the symbol list

    parameters:
                @param linked -  symbol list
                @param name - the name we search
    return:
                @return none
*/
GeneralList SearchLink(GeneralList linked, char *name);

/*
    This method prints all links in the list that we did not use in the file

    parameters:
                @param linked - symbol list
                @param phase - Indicates the phase in which the method is called
    return:
                @return the new symbol
*/
void allUsed(GeneralList linked, int phase);

/*
    This method changes the current head to be the next

    parameters:
                @param head - a pointer to the head of the symbol list
    return:
                @return none
*/
void moveHead(GeneralList *head);

/*
    This method returns the name of a given node

    parameters:
                @param linked - the node to get its name
    return:
                @return the name of the given node
*/
char *getName(GeneralList linked);

/*
    This method is used to get the start index of a mcr

    parameters:
                @param linked - the node that return its start index of mcr
    return:
                @return the start index of the mcr
*/
int getStartId(GeneralList linked);

/*
    This method is used to get the end index of a mcr

    parameters:
                @param linked - the node that return its end index of mcr
    return:
                @return the end index of the mcr
*/
int getEndId(GeneralList linked);

/*
    This method return the address of a given node

    parameters:
                @param linked - the node to get its address
    return:
                @return the address of the given node
*/
int getAddress(GeneralList linked);

/*
    This method return the type of given symbol

    parameters:
                @param linked - the symbol to get its type
    return:
                @return the type of the given symbol
*/
Type getType(GeneralList linked);

/*
    This method move to the next node in the list
    parameters:
                @param linked - the point to the node
    return:
                @return non
*/
GeneralList getNext(GeneralList linked);

/*
    This method is used to set the start index of a mcr

    parameters:
                @param linked - the node to set its start index of a mcr
                @param startId - a variable that keep the new start index of the mcr
    return:
                @return non
*/
void setStartId(GeneralList linked, int startId);

/*
    This method is used to set the end index of a mcr

    parameters:
                @param linked -  the node to set its end index of a mcr
                @param endId - a variable that keep the new end index of the mcr
    return:
                @return non
*/
void setEndId(GeneralList linked, int endId);

/*
    This method sets the address of a given node

    parameters:
                @param linked - the linked to change its address
                @param address - the new address of the linked
    return:
                @return non
*/
void setAddress(GeneralList linked, int address);

void setUsed(GeneralList linked, int used);


/*
    This method sets the type of given linked

    parameters:
                @param linked -  the linked to change its type
                @param address - the new type of the linked
    return:
                @return non
*/
void setType(GeneralList linked, Type type);

/*
    This method frees a given node from the memory.

    parameters:
                @param linked - a symbol node to be freed
    return:
                @return non
*/
void freeLinked(GeneralList linked);

/*
    This method freed the given list

    parameters:
                @param head - The head of the list
    return:
                @return non
*/
void freelist(GeneralList head);

/*
    This method update the DATA symbols address (add "IC" for all of them)

    parameters:
                @param linked - the head of the symbolTable
                param IC - the counter of the list
    return:
                @return non
*/
void update_IC_symbols(GeneralList *linked, int IC);

#endif

