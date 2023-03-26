#include "GeneralList.h"
#include "GeneralFunctions.h"
#include <stdlib.h>

struct linked {
    char *name;/* name of the node */
    int startId;/* start index of the macro */
    int endId; /* length of the macro */
    int used;
    int address; /* Address of the symbol */
    Type type;/* Symbol type */
    GeneralList next;  /* Next linked */
};

GeneralList createLinked(char *name, int address) {
    GeneralList new_general_linked = (GeneralList) calloc(1, sizeof(struct linked));
    new_general_linked->name = (char *) calloc(1, (strlen(name) + 1) * sizeof(char));
    strcpy(new_general_linked->name, name);
    new_general_linked->startId = 0;
    new_general_linked->endId = 0;
    new_general_linked->used = 0;
    new_general_linked->address = address;
    new_general_linked->next = NULL;
    new_general_linked->type = OPERATION;
    return new_general_linked;
}

void InsertLinked(GeneralList *head, GeneralList new_general_linked) {
    if (!(*head)) /* if the list is empty */
        *head = new_general_linked;
    else {
        /*Make next of new node as head*/
        new_general_linked->next = (*head);
        /*move the head to point to the new node*/
        (*head) = new_general_linked;
    }
}

GeneralList SearchLink(GeneralList linked, char *name) {
    GeneralList temp;
    if (linked != NULL) {
        temp = linked;
        if (linked != NULL) {
            if ((name != NULL) && strcmp(linked->name, name) == 0)
                /*if exist, return it*/
                return temp;
            return SearchLink(linked->next, name);/*recursive call to check next node*/
        }
    }
    return NULL;
}

void allUsed(GeneralList linked, int phase) {
    int flag = TRUE;
    while (linked != NULL) {
        if (linked->used != EXIST) {/*see unused link*/
            if (flag) {/*the flag marks the first time we see unused link*/
                if (phase == 0)/*message belonging to a reader layout at  phase zero */
                    printf("Warning occurred at the process, macros we didn't use: '%s'", linked->name);
                else/*message belonging to the phase one*/
                    printf("Warning occurred at the process, symbols we didn't use: '%s'", linked->name);
                flag = FALSE;
            } else
                printf(",'%s'", linked->name);/*Printing another unused name*/
        }
        linked = linked->next;/* move to check the next node*/
    }
    if (!flag)
        printf("\n");
}

void moveHead(GeneralList *head) {
    GeneralList temp = *head;
    *head = (*head)->next;
    freeLinked(temp);
}

char *getName(GeneralList linked) {
    return linked->name;
}

int getStartId(GeneralList linked) {
    return linked->startId;
}

int getEndId(GeneralList linked) {
    return linked->endId;
}

int getAddress(GeneralList linked) {
    return linked->address;
}

Type getType(GeneralList linked) {
    return linked->type;
}

GeneralList getNext(GeneralList linked) {
    return linked->next;
}

void setStartId(GeneralList linked, int startId) {
    linked->startId = startId;
}

void setEndId(GeneralList linked, int endId) {
    linked->endId = endId;
}

void setUsed(GeneralList linked, int used) {
    linked->used = used;
}

void setAddress(GeneralList linked, int address) {
    linked->address = address;
}

void setType(GeneralList linked, Type type) {
    linked->type = type;
}

void freeLinked(GeneralList linked) {
    free(linked->name);
    free(linked);
}

void freelist(GeneralList head) {
    GeneralList prev, current = head;
    while (current != NULL) {
        prev = current;
        current = current->next;
        freeLinked(prev);
    }
}

void update_IC_symbols(GeneralList *linked, int IC) {
    GeneralList temp = *linked;
    while (temp != NULL) {
        if (temp->type == DATA)/* if it's a data instruction */
            temp->address += IC;/* updates the IC counter */
        temp = temp->next;
    }
}

