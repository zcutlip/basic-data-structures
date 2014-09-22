#include "adt_conditional_includes.h"

#include "adt_synchronization.h"
#include "adt_malloc.h"
#include "adt_stack.h"
#include "adt_list.h"

typedef struct stack_struct
{
    LIST list;
    int size;
    ADT_LOCK lock;
} adt_stack_t;

STACK stack_create(void)
{
    STACK stack;

    stack=adt_malloc(sizeof(struct stack_struct));

    if(NULL == stack)
    {
        goto end;
    }
    
    stack->list=list_create();
    if(NULL == stack->list)
    {
        adt_free(stack);
        stack=NULL;
        goto end;
    }
    stack->lock=_adt_rw_lock_init();
    stack->size = 0;

end:
    return stack;
}

void stack_destroy(STACK *stack_p)
{
    ADT_LOCK lock;
    if(NULL == stack_p)
    {
        return;
    }
    lock=(*stack_p)->lock;
    _adt_write_unlock(lock);

    list_destroy(&((*stack_p)->list));
    
    bzero((*stack_p), sizeof(struct stack_struct));
    
    adt_free(*stack_p);
    *stack_p = NULL;
    
    _adt_write_unlock(lock);
    _adt_rw_lock_destroy(&lock);
    
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
    _adt_write_lock(stack->lock);
    status=lladd(stack->list,data,NULL);

    if(ADT_OK != status)
    {
        _adt_write_unlock(stack->lock);
        goto end;
    }

    stack->size++;
    status=ADT_OK;
    _adt_write_unlock(stack->lock);
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
    _adt_write_lock(stack->lock);
    
    if(stack->size <1 )
    {
        status=ADT_EMPTY;
        *data_p = NULL;
        goto end;
    }

    status=llremove_last(stack->list,data_p);
    if(ADT_OK != status)
    {
        _adt_write_unlock(stack->lock);
        goto end;
    }
    
    stack->size--;
    status=ADT_OK;
    _adt_write_unlock(stack->lock);
    
end:
    return status;
}

int stack_size(STACK stack)
{
    int size;
    if(NULL == stack)
    {
        return 0;
    }
    _adt_read_lock(stack->lock);
    size=stack->size;
    _adt_read_unlock(stack->lock);
    return size;
}


