#ifndef _ADT_LIST_H
#define _ADT_LIST_H

#include "adt_error.h"

typedef struct list_struct *LIST;
typedef struct node_struct *NODE;

/*
 * Create structures for a linked list.
 * Returns: A handle to the list, or NULL if memory allocation fails.
 */
LIST list_create(void);

/*
 * Free the memory allocated for linked list, and set the handle to NULL.
 *
 * Note: Memory for the objects stored in the list is not freed.
 *       This is the responsibility of the caller.
 *
 * Param: [in/out]list_p Pointer to the list that will be freed.
 *        The handle referenced by this pointer will be set to NULL.
 *
 * Returns: void.
 */
void list_destroy(LIST *list_p);

/*
 * Get the number if items stored in the list.
 *
 * Param: list A handle to the list whose length should be returned.
 * Returns: The length of the list.
 */
size_t list_length(LIST list);

/*
 * Append an item to the end of the list.
 *
 * Param: list A handle to the list to which the object should be appended.
 * Param: [in]data Pointer to the object to append to the list.
 * Param: [out]node_p The address of a node pointer which will point to the
 *        list node that is created as a result from the add operation.
 *        
 */
adt_status lladd(LIST list, void *data,NODE* node_p);

/*
 * Insert an item in the list after the given node.
 * 
 * Param: list A handle to the list into which the object should be inserted.
 * Param: [in]data Pointer to the object to insert into the list.
 * Param: [out]node_p The address of a node pointer which will point to the
 *        list node that is created as a result from the insert operation.
 */
adt_status llinsert_after(LIST list, void *data, NODE *node_p);

/*
 * Insert an item in the list before the given node.
 * 
 * Param: list A handle to the list into which the object should be inserted.
 * Param: [in]data Pointer to the object to insert into the list.
 * Param: [out]node_p The address of a node pointer which will point to the
 *        list node that is created as a result from the insert operation.
 */
adt_status llinsert_before(LIST list, void *data, NODE *node_p);

/*
 * Get a handle on the first object in the given list, and set the list's
 * current node pointer to the first node.
 * 
 * Note: this is not a read-only operation, since the list's "current" node 
 * will be set.
 * 
 * Param: list A handle to the list from which the data should be retrieved.
 * Param: [out]node_p The address of a node pointer which will point to the 
 *        list node that contains the object to be retrieved.
 * Param: [out]data_p The address of a void pointer which will point to the
 *        object to be retrieved.
 */
adt_status llget_first(LIST list, NODE *node_p, void **data_p);
adt_status llget_last(LIST list, NODE *node_p, void **data_p);
adt_status llget_prev(LIST list, NODE *node_p, void **data_p);
adt_status llget_next(LIST list, NODE *node_p, void **data_p);
adt_status llget_current(LIST list, NODE *node_p, void **data_p);

/* get a handle on the list's "current" node, and get it's data */
adt_status llget_current(LIST list, NODE *node_p, void **data_p);

adt_status llremove_node(LIST list, NODE *node_p, void **data_p);
adt_status llremove_first(LIST list, void **data_p);
adt_status llremove_last(LIST list, void **data_p);

#endif /* _ADT_LIST_H */

