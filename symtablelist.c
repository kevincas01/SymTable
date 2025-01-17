/*--------------------------------------------------------------------*/
/* symtablelist.c                                                             */
/* Author: Kevin Castro                                               */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include "symtable.h"
#include <assert.h>
#include <stdlib.h> 
#include <string.h>
#include <stddef.h>

/* Each item is stored in a SymTableNode.  SymTableNodes are linked to
   form a list.  */
struct SymTablenode {
    /* The key*/
    const char *string;
    /* The value*/
    void *value;
    /* The address of the next SymTablenode. */
    struct SymTablenode *next;

};
/*A stack is a node that points to the first SymTable Node*/
struct Stack {
    /* The address of the first SymTableNode. */
    struct SymTablenode *first;
    /*Number of bindings in the Symbol table*/
    size_t numbindings;
};


SymTable_T SymTable_new(void){

    SymTable_T symtablenew;

    symtablenew =(SymTable_T)malloc(sizeof(struct Stack));

    if (symtablenew==NULL)
    {
        return NULL;
    }

    symtablenew->first=NULL;
    symtablenew->numbindings=0;

    return symtablenew;
}

void SymTable_free(SymTable_T oSymTable){
    struct SymTablenode *currnode;
    struct SymTablenode *nextnode;

    assert(oSymTable!=NULL);

    for ( currnode= oSymTable->first; currnode!=NULL; currnode=nextnode)
    {
        nextnode=currnode->next;
        free((void*)currnode->string);
        free(currnode);
    }
    
    free(oSymTable);
}

size_t SymTable_getLength(SymTable_T oSymTable){
    
    assert(oSymTable!=NULL);
    return oSymTable->numbindings;
}

int SymTable_put(SymTable_T oSymTable, const char *pcKey, const void *pvValue){
    
    struct SymTablenode *new;
    struct SymTablenode *currnode;

    assert(oSymTable!=NULL);  
    assert(pcKey!=NULL);
    
    currnode = oSymTable->first;

/*  Check to see if pckey is inside the symbol table. If we find it
    then we can just change the value of that node that it is in*/

    while (currnode!=NULL)
    {
        int cmp=strcmp(currnode->string,pcKey);
        if (cmp==0)
        {
            return 0;
        }

        currnode = currnode->next;
    }

    /*  If not in the symbol table already, then we can just add the 
        key and value pair to the front*/
    new = (struct SymTablenode*)malloc(sizeof(struct SymTablenode));

    if (new==NULL)
    {
        return 0;
    }
    
    new->string=(const char*)malloc(strlen(pcKey)+1);

    strcpy((char*)new->string,pcKey);

    new->next=oSymTable->first;
    new->value= (void*)pvValue;
    oSymTable->first=new;

    oSymTable->numbindings++; /* Only when we add a new key and value pair*/

    return 1;
    /*FIX CONST VALUES*/

}

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey,
const void *pvValue) {

    struct SymTablenode *currnode;
    void *oldval;

    assert(oSymTable!=NULL);
    assert(pcKey!=NULL);

    currnode = oSymTable->first;

    while (currnode!=NULL)
    {
        int cmp=strcmp(currnode->string,pcKey);
        if (cmp==0)
        {
            oldval=currnode->value;
            currnode->value=(void*)pvValue;
            return oldval;
        }

        currnode = currnode->next;
    }
    return NULL;
}

int SymTable_contains(SymTable_T oSymTable, const char *pcKey){
    struct SymTablenode *currnode;
    
    assert(oSymTable!=NULL);  
    assert(pcKey!=NULL);

    currnode = oSymTable->first;

    while (currnode!=NULL)
    {
        int cmp=strcmp(currnode->string,pcKey);
        if (cmp==0)
        {
            return 1;
        }

        currnode = currnode->next;
    }
    return 0;

}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey){
    struct SymTablenode *currnode;

    assert(oSymTable!=NULL);
    assert(pcKey!=NULL);

    currnode = oSymTable->first;

    while (currnode!=NULL)
    {
        int cmp=strcmp(currnode->string,pcKey);
        if (cmp==0)
        {
            return currnode->value;
        }

        currnode = currnode->next;
    }

    return NULL;
}

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey){
   
    struct SymTablenode *currnode;
    struct SymTablenode *formernode;
    void *returni;
    int cmp;

    assert(oSymTable!=NULL);
    assert(pcKey!=NULL);

    formernode=oSymTable->first;

    if(formernode==NULL){
        return NULL;
    }

    currnode = formernode->next;

    cmp=strcmp(formernode->string,pcKey);

    if (cmp==0)
    {
        oSymTable->numbindings--;
        returni=formernode->value;
        free((void*)formernode->string);
        free(formernode);
        
        oSymTable->first=currnode;
        
        return returni;
    }
    else{
        while (currnode!=NULL)
        {
            cmp=strcmp(currnode->string,pcKey);
            if (cmp==0)
            {
                oSymTable->numbindings--;
                returni=currnode->value;
        
                formernode->next=currnode->next;
                free((void*)currnode->string);
                
                free(currnode);

                return returni;
            }
            currnode = currnode->next;
            formernode=formernode->next;
        }

    }
    return NULL;
}

/* INCLUDE THE STRING LIBRARY TO USE THE STRING COMPARE FUNCTION.
AND REPLACE THE CONDITIONS IN THE IF STATEMENTS TO SAY THAT IF THEY ARE 
0, CONTINUE TO DO WHATEVER IS INSIDE*/

void SymTable_map(SymTable_T oSymTable,
void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
const void *pvExtra){

    struct SymTablenode *currnode;

    assert(oSymTable!=NULL);
    assert(pfApply!=NULL);


    for (currnode= oSymTable->first; currnode!=NULL; currnode=currnode->next)
    {
        (*pfApply)(currnode->string,(void*)currnode->value,(void*)pvExtra);
    }

}


    
