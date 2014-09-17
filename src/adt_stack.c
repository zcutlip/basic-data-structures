#include "adt_conditional_includes.h"

#include "adt_stack.h"
#include "adt_list.h"

typedef struct stack_struct
{
    LIST list;
    int size;
} adt_stack_t;

STACK stack_create(void)
{
    STACK stack;

    stack=malloc(sizeof(struct stack_struct));

    if(NULL == stack)
    {
        goto end;
    }
    
    stack->list=list_create();
    if(NULL == stack->list)
    {
        free(stack);
        stack=NULL;
        goto end;
    }

    stack->size = 0;

end:
    return stack;
}

void stack_destroy(STACK *stack_p)
{
    if(NULL == stack_p)
    {
        return;
    }

    list_destroy(&((*stack_p)->list));

    free(*stack_p);
    *stack_p = NULL;

    return;
}

adt_status stack_push(STACK stack, void *data)
{
    adt_status status;

    if(NULL==stack)
    {
        status=ADT_INVALID_PARAM;
        goto end;
    }
    
    status=lladd(stack->list,data,NULL);

    if(ADT_OK != status)
    {
        goto end;
    }

    stack->size++;
    status=ADT_OK;
end:
    return status;
}

adt_status stack_pop(STACK stack, void **data_p)
{
    adt_status status;
    
    if(NULL == stack || NULL == data_p)
    {
        status=ADT_INVALID_PARAM;
        goto end;
    }
    
    if(stack->size <1 )
    {
        status=ADT_EMPTY;
        *data_p = NULL;
        goto end;
    }

    status=llremove_last(stack->list,data_p);
    if(ADT_OK != status)
    {
        goto end;
    }
    
    stack->size--;
    status=ADT_OK;
end:
    return status;
}

int stack_size(STACK stack)
{
    if(NULL == stack)
    {
        return 0;
    }

    return stack->size;
}


