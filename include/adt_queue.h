#ifndef _ADT_QUEUE_H
#define _ADT_QUEUE_H

#include "adt_error.h"

typedef struct queue_struct *QUEUE;

QUEUE queue_create(void);
void queue_destroy(QUEUE *queue_p);

adt_status queue_insert(QUEUE queue,void *data);
adt_status queue_remove(QUEUE queue,void **data_p);
adt_status queue_peek(QUEUE queue, void **data_p);

int queue_size(QUEUE queue);

#endif /* _ADT_QUEUE_H */
