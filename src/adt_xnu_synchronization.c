

#include "adt_synchronization.h"

#include <mach/semaphore.h>
#include <mach/task.h>
#include <kern/task.h>
#include <kern/locks.h>
#include "adt_malloc.h"
#include "adt_xnu_synchronization.h"

#ifdef ADT_XNU_SYNC


typedef struct adt_lock_t
{
    lck_rw_t    *rw_lock;
    lck_grp_t   *rw_lock_grp;
    lck_attr_t  *rw_lock_attr;
    lck_grp_attr_t  *rw_lock_grp_attr;
}adt_lock_t;

ADT_LOCK _adt_xnu_rw_lock_init(int max_threads)
{
    ADT_LOCK rwlock;
    
    rwlock = adt_malloc(sizeof(adt_lock_t));
    if(rwlock == NULL)
    {
        goto end;
    }

    rwlock->rw_lock_grp_attr=lck_grp_attr_alloc_init();
    //TODO:not sure if the name needs to be unique
    rwlock->rw_lock_grp=lck_grp_alloc_init("adt_rw_lock", rwlock->rw_lock_grp_attr);
    rwlock->rw_lock_attr=lck_attr_alloc_init();
    rwlock->rw_lock=lck_rw_alloc_init(rwlock->rw_lock_grp, rwlock->rw_lock_attr);


end:
    return rwlock;
}

adt_status _adt_xnu_read_lock(ADT_LOCK lock)
{
    adt_status ret;
    if(NULL == lock)
    {
        ret=ADT_INVALID_PARAM;
        goto end;
    }
    
    lck_rw_lock_shared(lock->rw_lock);
    ret=ADT_OK;
end:
    return ret;
}

adt_status _adt_xnu_read_unlock(ADT_LOCK lock)
{
    adt_status ret;
    if(NULL == lock)
    {
        ret=ADT_INVALID_PARAM;
        goto end;
    }
    
    lck_rw_unlock_shared(lock->rw_lock);
    ret=ADT_OK;
end:
    return ret;
}

adt_status _adt_xnu_write_lock(ADT_LOCK lock)
{
    adt_status ret;
    if(NULL == lock)
    {
        ret=ADT_INVALID_PARAM;
        goto end;
    }
    
    lck_rw_lock_exclusive(lock->rw_lock);
    ret=ADT_OK;
end:

    return ret;
}

adt_status _adt_xnu_write_unlock(ADT_LOCK lock)
{
    adt_status ret;
    if(NULL == lock)
    {
        ret=ADT_INVALID_PARAM;
        goto end;
    }

    lck_rw_unlock_exclusive(lock->rw_lock);
    ret=ADT_OK;
end:
    return ret;
}

void _adt_xnu_rw_lock_destroy(ADT_LOCK *lockp)
{
    ADT_LOCK rwlock;
    if(NULL == lockp || NULL == *lockp)
    {
        return;
    }
    rwlock=*lockp;
    lck_rw_free(rwlock->rw_lock, rwlock->rw_lock_grp);
    lck_attr_free(rwlock->rw_lock_attr);
    lck_grp_free(rwlock->rw_lock_grp);
    lck_grp_attr_free(rwlock->rw_lock_grp_attr);
    adt_free(rwlock);
    *lockp=NULL;
}


#endif /* ADT_XNU_SYNC */
