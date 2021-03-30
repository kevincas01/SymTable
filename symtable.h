/*--------------------------------------------------------------------*/
/* symtable.h                                                           */
/* Author: Kevin Castro                                               */
/*--------------------------------------------------------------------*/

#ifndef SYM_TABLE_INCLUDED
#define SYM_TABLE_INCLUDED

#include <stddef.h>

typedef struct Stack *SymTable_T;

/*Returns a SymTable_T that is empty with no bindings. Returns NULL
if the memory allocation results in NULL.
*/
SymTable_T SymTable_new(void);

/* Frees all memory that is allocated for oSymTable. */
void SymTable_free(SymTable_T oSymTable);

/*Returns the  number of bindings in oSymTable*/
size_t SymTable_getLength(SymTable_T oSymTable);

/*Adds a new binding in oSymTable with the key and value pair, pcKey 
and pvValue, if pcKey is not already in a binding in oSymTable and 
returns 1. Returns 0 and oSymtable remains unchanged if a binding with 
pcKey exists in oSymtable or if memory allocation results in NULL.
*/
int SymTable_put(SymTable_T oSymTable, const char *pcKey, 
const void *pvValue);

/* Returns a void * to the old value in the binding with key pcKey and 
replaces the binding's value with pvValue if there exists a binding with 
pcKey in oSymTable. Returns NULL if no such binding with pcKey exists in 
oSymTable.
*/
void *SymTable_replace(SymTable_T oSymTable, const char *pcKey,
const void *pvValue);

/* Returns 1 if oSymTable contains a binding with key pcKey. Returns 
0 if there is no binding with key pcKey in oSymTable.
*/
int SymTable_contains(SymTable_T oSymTable, const char *pcKey);

/* Returns a void* to the value of the binding with key pcKey in 
oSymTable if it exists. Returns NULL if no such binding exists */
void *SymTable_get(SymTable_T oSymTable, const char *pcKey);

/* Returns a void * to the value of the binding with key pcKey if it 
exists in oSymTable and removes the binding from the table. Returns 
NULL if the binding with key pcKey does not exist in oSymTable*/
void *SymTable_remove(SymTable_T oSymTable, const char *pcKey);

/*Applies the function *pfApply to the bindings in oSymTable. The 
function  *pfApply takes three parameters: a key pcKey, value pvValue, 
and an extra parameter, pvExtra. */
void SymTable_map(SymTable_T oSymTable,
void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
const void *pvExtra);


#endif


/* DO WE HAVE TO SPECIFY IF WE DONT RETURN ANYTHING???
