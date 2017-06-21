#include "adt_conditional_includes.h"
#include "adt_ring_buffer.h"
#include "adt_queue.h"
#include "adt_synchronization.h"
#include "adt_malloc.h"

struct ring_buffer_struct
{
    QUEUE queue;
    ADT_LOCK lock;
    size_t max_items;
};

RBUFFER rbuffer_create(size_t rbuffer_size)
{
    RBUFFER rbuffer;
    QUEUE queue;
    ADT_LOCK lock;
    queue=queue_create();

    rbuffer=NULL;
    if(NULL == queue)
    {
        return rbuffer;
    }

    rbuffer=adt_malloc(sizeof(struct ring_buffer_struct));
    if(NULL == queue)
    {
        queue_destroy(&queue);
        return rbuffer;
    }
    
    lock=_adt_rw_lock_init();
    if(!lock)
    {
        queue_destroy(&queue);
        adt_free(rbuffer);
        return NULL;
    }

    rbuffer->queue=queue;
    rbuffer->lock=lock;
    rbuffer->max_items=rbuffer_size;

    return rbuffer;
    
}

void rbuffer_destroy(RBUFFER *rbuffer_p)
{
    RBUFFER rbuffer;
    ADT_LOCK lock;
    
    if(NULL==rbuffer_p || NULL == *rbuffer_p)
    {
        return;
    }
    
    lock=(*rbuffer_p)->lock;
    _adt_write_lock(lock);
    rbuffer=(*rbuffer_p);
    
    queue_destroy(&((*rbuffer_p)->queue));
    
    rbuffer->lock=NULL;

    adt_free(*rbuffer_p);
    *rbuffer_p=rbuffer=NULL;

    _adt_write_unlock(lock);
    _adt_rw_lock_destroy(&lock);
    
    
    return;
}

adt_status rbuffer_insert(RBUFFER rbuffer,void *data,void **data_p,int lock)
{
    adt_status ret;
    if(NULL==rbuffer)
    {
        ret = ADT_INVALID_PARAM;
        goto end;
    }
    
    if(lock) { _adt_write_lock(rbuffer->lock);}
    if(queue_size(rbuffer->queue) >= rbuffer->max_items) //is the ring buffer full?
    {
        if(data_p) //caller wants to remove the oldest item if ring buffer is full
        {
            ret=queue_remove(rbuffer->queue,data_p);
            if(ret!=ADT_OK) //something went wrong with the underlying queue!
            {
                if(lock) { _adt_write_unlock(rbuffer->lock);}
                goto end;
            }
        }else //caller didn't provide an OUT data pointer, can't insert.
        {
            ret=ADT_FULL;
            if(lock) { _adt_write_unlock(rbuffer->lock);}
            goto end;
        }//end if(data_p)
    }
    
    ret=queue_insert(rbuffer->queue,data);
    if(lock) { _adt_write_unlock(rbuffer->lock);}
    
end:
    return ret;
    
}

adt_status rbuffer_remove(RBUFFER rbuffer,void **data_p, int lock)
{
    adt_status ret;
    if(NULL == rbuffer || NULL == data_p)
    {
        ret = ADT_INVALID_PARAM;
        goto end;
    }
    
    if(lock) { _adt_write_lock(rbuffer->lock);}
    //underlying queue handles empty queue, etc. returns appropriate status.
    ret=queue_remove(rbuffer->queue,data_p);
    if(lock) { _adt_write_unlock(rbuffer->lock);}
    
end:
    return ret;
}
