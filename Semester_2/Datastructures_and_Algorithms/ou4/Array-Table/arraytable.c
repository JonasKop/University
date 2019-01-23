/*
 * LP3 Datastrukturer & algoritmer (C)
 * Spring 17
 * Assignment 4
 * File: arraytable.c
 * Description: Table implemented as array.
 * Author: Jonas Sjödin
 * CS-user: id16jsn
 * Date: 2017-02-27
 */


#include <stdio.h>
#include "table.h"
#include "array.h"

typedef struct ArrayTable {
    int maxElIndex;

    array *values;
    CompareFunction *cf;
    KeyFreeFunc *keyFree;
    ValueFreeFunc *valueFree;
} ArrayTable;

typedef struct TableElement {
    KEY key;
    VALUE value;
} TableElement;

#define ARRSIZE 10000


/*
 * Function:    table_create
 *
 * Description: Creates an arraytable and allocates memory for it.
 *              Function also sets the desired compare-function for the table
 *              and initiates the maxElIndex variable. Then returns the table
 *              .
 * Input:       compare_function - Pointer to a function for comparing.
 * Output:      An arraytable.
 */
Table *table_create(CompareFunction *compare_function) {
    ArrayTable *myTable = calloc(sizeof(ArrayTable), 1);

    if (!myTable)
        return NULL;

    //ARRSIZE - 1 for setting array with ARRSIZE axmount of cells.
    myTable->values = array_create(1, 0, ARRSIZE - 1);
    array_setMemHandler(myTable->values, free);

    myTable->maxElIndex = -1;

    myTable->cf = compare_function;
    return myTable;
}

/*
 * Function:    table_setKeyMemHandler
 *
 * Description: Sets a memhandler that handles allocated memory for a
 *              key.
 *
 * Input:       table - Pointer to the arrayTable
 *              freeFunc - Pointer to a function for freeing values, Ex: free.
 * Output:      NONE.
 */
void table_setKeyMemHandler(Table *table,KeyFreeFunc *freeFunc) {
    ArrayTable *myTable = (ArrayTable *) table;
    myTable->keyFree = freeFunc;
}

/*
 * Function:    table_setValueMemHandler
 *
 * Description: Sets a memhandler that handles allocated memory for a
 *              value.
 *
 * Input:       table - Pointer to the arrayTable
 *              freeFunc - Pointer to a function for freeing values, Ex: free.
 * Output:      NONE.
 */
void table_setValueMemHandler(Table *table, ValueFreeFunc *freeFunc) {
    ArrayTable *myTable = (ArrayTable *) table;
    myTable->valueFree = freeFunc;
}

/*
 * Function:    table_isEmpty
 *
 * Description: Checks if the table is empty.
 *
 * Input:       table - Pointer to the arrayTable
 * Output:      Returns "true" if table is empty and "false" if not.
 */
bool table_isEmpty(Table *table) {
    ArrayTable *myTable = (ArrayTable *) table;

    if (myTable->maxElIndex == -1)
        return true;

    return false;
}

/*
 * Function: table_insert
 *
 * Description: Inserts a value and a key in the table. If the key is in the
 *              array it is deleted and replaces it with the new element and
 *              it's key and value. If not, it allocates memory and puts it
 *              after the last element in the array.
 *
 * Input:       table - Pointer to the arrayTable
 *              key   - The key that's about to be inserted
 *              value - The value that's about to be inserted
 * Output:      An edited table with a new value added or replaced.
 */
void table_insert(Table *table, KEY key,VALUE value) {
    ArrayTable *myTable = (ArrayTable *) table;
    TableElement *element = malloc(sizeof(TableElement));
    TableElement *testElement;

    element->key = key;
    element->value = value;

    for (int i = 0; i <= myTable->maxElIndex + 1; i++) {
        testElement = array_inspectValue(myTable->values, i);

        if(testElement != 0) {
            if (myTable->cf(testElement->key, element->key) == 0) {
                if (myTable->keyFree != NULL)
                    myTable->keyFree(testElement->key);
                if (myTable->valueFree != NULL)
                    myTable->valueFree(testElement->value);

                array_setValue(myTable->values, element, i);

                return;
            }
        }
        else if (i == myTable->maxElIndex + 1) {
            myTable->maxElIndex++;

            array_setValue(myTable->values, element, myTable->maxElIndex);
            return;
        }
    }
}

/*
 * Function: table_lookup
 *
 * Description: Searches for a specific key in the arraytable. If found return
 *              true if not return false.
 *
 * Input:       table - Pointer to the arrayTable
 *              key   - The key that's about to be checked if it exists in
 *              the table
 * Output:      If the key has been found, return "true". If not return "false".
 */
VALUE table_lookup(Table *table, KEY key) {
    ArrayTable *myTable = (ArrayTable *) table;
    TableElement *element;

    for (int i = myTable->maxElIndex; i >= 0; i--) {
        element = array_inspectValue(myTable->values, i);
        if (element != 0){
            if (myTable->cf(element->key, key) == 0) {
                return element->value;
            }
        }
    }
    return NULL;
}

/*
 * Function: table_remove
 *
 * Description: Removes an element from the arraytable if the key exists in
 *              the arraytable.
 *
 * Input:       table - Pointer to the arrayTable
 *              key   - The key that's about to be removed
 * Output:      If the key exists the element has been removed from the table
 *              and the memory has been deallocated.
 */
void table_remove(Table *table, KEY key) {
    ArrayTable *myTable = (ArrayTable *) table;
    TableElement *element;

    for (int i = 0; i <= myTable->maxElIndex; i++) {
        element = array_inspectValue(myTable->values, i);
        if (myTable->cf(element->key, key) == 0) {

            if (myTable->keyFree != NULL)
                myTable->keyFree(element->key);
            if (myTable->valueFree != NULL)
                myTable->valueFree(element->value);

            if (i == myTable->maxElIndex)
                array_setValue(myTable->values, NULL, myTable->maxElIndex);
            else {
                array_setValue(myTable->values, array_inspectValue(myTable->values, myTable->maxElIndex), i);
                array_setMemHandler(myTable->values, NULL);

                array_setValue(myTable->values, NULL, myTable->maxElIndex);
                array_setMemHandler(myTable->values, free);
            }
            myTable->maxElIndex--;

            return;
        }
    }
}

/*
 * Function: table_free
 *
 * Description: Deallocates all the memory for the elements that still exists
 *              in the table and frees the array in the table and the table
 *              itself.
 *
 * Input:       table - Pointer to the arrayTable
 * Output:      An arraytable that has been completely deallocated.
 */
void table_free(Table *table) {
    ArrayTable *myTable = (ArrayTable *) table;
    TableElement *element;

    for (int i = 0; i <= myTable->maxElIndex; i++){
        element=array_inspectValue(myTable->values, i);
        if(myTable->keyFree != NULL)
            myTable->keyFree(element->key);
        if(myTable->valueFree != NULL)
            myTable->valueFree(element->value);
    }

    array_free(myTable->values);
    free(myTable);
}
