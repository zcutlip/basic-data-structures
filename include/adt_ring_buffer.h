#ifndef _ADT_RING_BUFFER_H
#define _ADT_RING_BUFFER_H

#include "adt_error.h"

typedef struct ring_buffer_struct *RBUFFER;

RBUFFER rbuffer_create(size_t nitems);

/*
 * Free the memory allocated for ring buffer, and set the handle to NULL.
 *
 * Note: Memory for the objects stored in the list is not freed.
 *       This is the responsibility of the caller.
 *
 * Param: [in/out]rbuffer_p Pointer to the ring buffer that will be freed.
 *        The handle referenced by this pointer will be set to NULL.
 *
 * Returns: void.
 */
void rbuffer_destroy(RBUFFER *rbuffer_p);

/*
 * Insert an item into the ring buffer
 * If the ring buffer is full and data_p is not NULL
 * the oldest item will be removed from the ring buffer and returned
 * via data_p.
 * If the ring buffer is full and data_p is NULL, ADT_FULL is returned.
 * Note: Memory for the objects stored in the ring buffer are not freed.
 *       This is the responsibility of the caller.
 * Param:   [in/out]rbuffer The ring buffer to insert the item into.
 * Param:   [in]data The data item to insert into the ring buffer.
 * Param:   [out]data_p The address of a void pointer which will point to the
 *              object to be retrieved.
 */
adt_status rbuffer_insert(RBUFFER queue,void *data,void **data_p);

/*
 * Remove the oldest item from the ring buffer.
 * Returns: ADT_EMPTY if the ring buffer is empty.
 *
 * Param:   [in/out]rbuffer The ring buffer to insert the item into.
 * Param:   [out]data_p The address of a void pointer which will point to the
 *              object to be retrieved.
 *
 */
adt_status rbuffer_remove(RBUFFER queue,void **data_p);

#endif /* _ADT_RING_BUFFER_H */