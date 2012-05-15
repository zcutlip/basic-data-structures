#include <stdlib.h>

#include "adt_queue.h"
#include "adt_error.h"
#include "adt_list.h"

typedef struct queue_struct
{
    LIST list;
    int size;
} queue_t;


QUEUE queue_create(void)
{
    QUEUE queue;
    LIST list;
    
    list=list_create();

    queue=NULL;
    if(NULL == list)
    {
        return queue;
    }

    queue=malloc(sizeof(struct queue_struct));
    if(NULL == queue)
    {
        list_destroy(&list);
        return queue;
    }

    queue->list = list;
    queue->size = 0;
    

    return queue;
}

void queue_destroy(QUEUE *queue_p)
{
    if(NULL == queue_p || NULL == *queue_p)
    {
        return;
    }

    list_destroy(&((*queue_p)->list));

    free(*queue_p);

    *queue_p = NULL;

    return;
}

adt_status queue_insert(QUEUE queue, void *data)
{
    adt_status status;
    
    if(NULL == queue)
    {
        status = ADT_INVALID_PARAM;
        goto end;
    }
    
    status=lladd(queue->list,data,0,NULL);
    if(ADT_OK == status)
    {
        queue->size++;
    }
end:
    return status;
}

adt_status queue_remove(QUEUE queue,void **data_p)
{
    adt_status status;

    if(NULL == queue || NULL == data_p)
    {
        status=ADT_INVALID_PARAM;
        goto end;
    }

    if(queue->size < 1)
    {
        status = ADT_EMPTY;
        goto end;
    }
    
    status=llremove_first(queue->list,data_p);
    if(ADT_OK == status)
    {
        queue->size--;
    }

end:
    return status;
}

adt_status queue_peek(QUEUE queue, void **data_p)
{
    adt_status status;
    NODE node;

    if(NULL == queue || NULL == data_p)
    {
        status=ADT_INVALID_PARAM;
        goto end;
    }

    if(queue->size < 1)
    {
        status=ADT_EMPTY;
        goto end;
    }

    status=llget_first(queue->list, &node,data_p);

end:
    return status;

}

int queue_size(QUEUE queue)
{
    if(NULL == queue)
    {
        return 0;
    }

    return queue->size;
}

