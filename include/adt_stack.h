#ifndef _ADT_STACK_H
#define _ADT_STACK_H

#include "adt_error.h"

typedef struct stack_struct *STACK;

STACK stack_create(void);
void stack_destroy(STACK* stack_p);

adt_status stack_push(STACK stack,void *data);
adt_status stack_pop(STACK stack,void **data_p);

int stack_size(STACK stack);

#endif /* _ADT_STACK_H */
