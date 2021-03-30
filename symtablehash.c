#include <stdio.h>
#include "symtable.h"
#include <assert.h>
#include <stdlib.h> 
#include <string.h>
#include <stddef.h>


static const size_t auBucketCounts[] = {509, 1021,2039,4093,8191,16381,32749,65521};

struct HashTablenode{
    const char *string;
    void *value;
    struct HashTablenode *next;
};

struct Stack {
    struct HashTablenode ** hashbuckets;
    size_t bindings;
    size_t bucketnum;
};
/* Return a hash code for pcKey that is between 0 and uBucketCount-1,
        inclusive. */
static size_t SymTable_hash(const char *pcKey, size_t uBucketCount) {
    const size_t HASH_MULTIPLIER = 65599;
    size_t u;
    size_t uHash = 0;

    assert(pcKey != NULL);

    for (u = 0; pcKey[u] != '\0'; u++){
    uHash = uHash * HASH_MULTIPLIER + (size_t)pcKey[u];
    }
    return uHash % uBucketCount;
}    

SymTable_T SymTable_new(void){
    SymTable_T symtablenew;

    symtablenew =(SymTable_T)malloc(sizeof(struct Stack));

    if (symtablenew==NULL)
    {
        return NULL;
    }

    symtablenew->hashbuckets=(struct HashTablenode**)calloc(auBucketCounts[0],sizeof(struct HashTablenode*));

    if (symtablenew->hashbuckets==NULL)
    {
        return NULL;
    }
    
    symtablenew->bindings=0;
    symtablenew->bucketnum=0;
    return symtablenew;
}

void SymTable_free(SymTable_T oSymTable){
    struct HashTablenode *currnode;
    struct HashTablenode *nextnode;
    size_t index;

    size_t size=auBucketCounts[oSymTable->bucketnum];

    assert(oSymTable!=NULL);

    for (index = 0; index < size; index++) {
    
        for ( currnode=oSymTable->hashbuckets[index]; currnode!=NULL; currnode=nextnode){
            nextnode=currnode->next;
            free((void*)currnode->string);
            free(currnode);
        }
        
    }
    free(oSymTable->hashbuckets);
    free(oSymTable);
}

size_t SymTable_getLength(SymTable_T oSymTable){

    assert(oSymTable!=NULL);

    return oSymTable->bindings;
}


static struct HashTablenode** SymTable_reposition(SymTable_T oSymTable,size_t bnum) {
    SymTable_T newSymTable;
    struct HashTablenode *new;
    struct HashTablenode *currnode;
    size_t index;
    size_t hashnum;
    size_t oldsize;
    size_t newsize;

    assert(oSymTable!=NULL);

    oldsize=auBucketCounts[bnum];
    bnum++;
    newsize=auBucketCounts[bnum];

    newSymTable=oSymTable;
    newSymTable->bucketnum=bnum;
    newSymTable->bindings=oSymTable->bindings;

    newSymTable->hashbuckets=(struct HashTablenode**)realloc(oSymTable->hashbuckets,newsize*sizeof(struct HashTablenode*));

    if (newSymTable->hashbuckets==NULL || newsize == auBucketCounts[7])
    {
       return oSymTable->hashbuckets;
       
    }

    for (index = 0; index < oldsize; index++) {
        for ( currnode=oSymTable->hashbuckets[index]; currnode!=NULL; currnode=currnode->next){

            hashnum=SymTable_hash(currnode->string,newsize);

            new=currnode;
            new->next=newSymTable->hashbuckets[hashnum];
            new->string=currnode->string;
            new->value=currnode->value;
           
            newSymTable->hashbuckets[hashnum]=new;
        }
     }

    return newSymTable->hashbuckets;
}

int SymTable_put(SymTable_T oSymTable, const char *pcKey, const void *pvValue){
    
    struct HashTablenode *new;
    struct HashTablenode *currnode;
    size_t bnum;
    size_t hashnum;

    assert(oSymTable!=NULL);  
    assert(pcKey!=NULL);

    bnum=oSymTable->bucketnum;
    hashnum=SymTable_hash(pcKey,auBucketCounts[bnum]);

    currnode=oSymTable->hashbuckets[hashnum];

    while (currnode!=NULL)
    {
        int cmp=strcmp(currnode->string,pcKey);
        if (cmp==0)
        {
            return 0;
        }
        currnode = currnode->next;
    }
    new = (struct HashTablenode*)malloc(sizeof(struct HashTablenode));
    if (new==NULL)
    {
        return 0;
    }
    
    oSymTable->bindings++;

    if (oSymTable->bindings==auBucketCounts[bnum])
    {
        oSymTable->hashbuckets=SymTable_reposition(oSymTable,bnum);
        oSymTable->bucketnum++;
    }

    new->string=(const char*)malloc(strlen(pcKey)+1);
    if (new->string==NULL)
    {
       return 0;
    }
    
    strcpy((char*)new->string,pcKey);

    new->next=oSymTable->hashbuckets[hashnum];
    new->value = (void*)pvValue;
    oSymTable->hashbuckets[hashnum]=new;
    
    return 1;
}
/* STILL PUT THE NODE INSIDE
THEY ACT THE SAME!*/

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey,
const void *pvValue){

    struct HashTablenode *currnode;
    size_t bnum;
    size_t hashnum;
    void *oldval;
    
    assert(oSymTable!=NULL);  
    assert(pcKey!=NULL);

    bnum=oSymTable->bucketnum;
    hashnum=SymTable_hash(pcKey,auBucketCounts[bnum]);

    currnode=oSymTable->hashbuckets[hashnum];

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
    struct HashTablenode *currnode;
    size_t bnum;
    size_t hashnum;
    
    assert(oSymTable!=NULL);  
    assert(pcKey!=NULL);

    bnum=oSymTable->bucketnum;
    hashnum=SymTable_hash(pcKey,auBucketCounts[bnum]);

    currnode=oSymTable->hashbuckets[hashnum];

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
    struct HashTablenode *currnode;
    size_t bnum;
    size_t hashnum;

    assert(oSymTable!=NULL);  
    assert(pcKey!=NULL);

    bnum=oSymTable->bucketnum;
    hashnum=SymTable_hash(pcKey,auBucketCounts[bnum]);

    currnode=oSymTable->hashbuckets[hashnum];

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
    
    struct HashTablenode *currnode;
    struct HashTablenode *formernode;
    void *returni;
    int cmp;
    size_t bnum;
    size_t hashnum;

    assert(oSymTable!=NULL);
    assert(pcKey!=NULL);

    bnum=oSymTable->bucketnum;
    hashnum=SymTable_hash(pcKey,auBucketCounts[bnum]);

    formernode=oSymTable->hashbuckets[hashnum];

    if(formernode==NULL){
        return NULL;
    }

    currnode = formernode->next;

    cmp=strcmp(formernode->string,pcKey);

    if (cmp==0)
    {
        oSymTable->bindings--;
        returni=formernode->value;
        free((void*)formernode->string);
        free(formernode);
        
        oSymTable->hashbuckets[hashnum]=currnode;
        
        return returni;
    }
    else{
        while (currnode!=NULL)
        {
            cmp=strcmp(currnode->string,pcKey);
            if (cmp==0)
            {
                oSymTable->bindings--;
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

void SymTable_map(SymTable_T oSymTable,
void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
const void *pvExtra){

    size_t size=auBucketCounts[oSymTable->bucketnum];
    struct HashTablenode *currnode;
    size_t index;

    assert(oSymTable!=NULL);
    assert(pfApply!=NULL);

    assert(oSymTable!=NULL);

    for (index = 0; index < size; index++) {

        for ( currnode=oSymTable->hashbuckets[index]; currnode!=NULL; currnode=currnode->next){

        (*pfApply)(currnode->string,(void*)currnode->value,(void*)pvExtra);
        }

    }

}
