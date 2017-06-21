#include "adt_conditional_includes.h"

#include <stdint.h>
#include <string.h>

#include "adt_synchronization.h"
#include "adt_malloc.h"
#include "adt_hash.h"
#include "adt_list.h"
#include "adt_error.h"
#include "htable_primes.h"

typedef htable_key_uint64_t htable_hash_t;
typedef uint32_t htable_hash32_t;

typedef struct htnode_struct
{
    htable_hash_t hash;
    char *key;
    void *value;
}htnode_t;

typedef htnode_t *HTNODE;

struct htable_struct
{
    size_t htable_size;
    ADT_LOCK lock;
    LIST htable_chains[0];
};

static adt_status _hash_func(const void *key, size_t key_len, uint32_t *val_p);
static HTNODE _htnode_create(void);
static void _htnode_destroy(HTNODE *htnode_p);
static size_t _htnode_index(HTABLE htable, htable_hash_t hash);
static adt_status _htable_fast_insert_nolock(HTABLE htable,char *key,size_t key_len,void *value);
static adt_status _htable_fast_insert_uint64_nolock(HTABLE htable,
                                                    htable_key_uint64_t key,void *value);

adt_status htable_closest_prime(size_t size,size_t *size_p)
{
    size_t i;
    adt_status ret=ADT_ERROR;
    if (NULL == size_p)
    {
        ret=ADT_INVALID_PARAM;
        goto end;
    }
    
    for(i=0;i<sizeof(HTABLE_PRIMES);i++)
    {
        if (HTABLE_PRIMES[i] > size)
        {
            (*size_p) = HTABLE_PRIMES[i];
            ret=ADT_OK;
            break;
        }
    }

end:
    return ret;
}

void htable_destroy(HTABLE *htable_p)
{
    HTABLE htable;
    ADT_LOCK lock;
    size_t i;
    if(NULL == htable_p || NULL == (*htable_p))
    {
        return;
    }
    
    lock=(*htable_p)->lock;
    _adt_write_lock(lock);
    
    htable=(*htable_p);
    
    for(i=0;i<htable->htable_size;i++)
    {
        list_destroy(&(htable->htable_chains[i]));
    }
    (*htable_p)->lock=NULL;
    adt_free(*htable_p);
    (*htable_p)=NULL;
    _adt_write_unlock(lock);
    _adt_rw_lock_destroy(&lock);
    
    return;
}

HTABLE htable_create(size_t size)
{
    HTABLE htable;
    size_t i;
    htable=adt_malloc(sizeof(struct htable_struct) + size*sizeof(LIST));
    
    if(NULL == htable)
    {
        goto end;
    }
    
    memset(htable,0,sizeof(struct htable_struct) + size*sizeof(LIST));
    htable->htable_size=size;
    
    for(i=0;i<size;i++)
    {
        htable->htable_chains[i]=list_create();
        if(NULL==htable->htable_chains[i])
        {
            htable_destroy(&htable);
            goto end;
        }
    }

    htable->lock=_adt_rw_lock_init();
    
end:
    return htable;
}

adt_status htable_read_lock(HTABLE htable)
{
    adt_status ret;
    
    
    if(NULL == htable)
    {
        ret = ADT_INVALID_PARAM;
        goto end;
    }
    
    ret = _adt_read_lock(htable->lock);
    
end:
    return ret;
}

adt_status htable_read_unlock(HTABLE htable)
{
    adt_status ret;
    if(NULL == htable)
    {
        ret = ADT_INVALID_PARAM;
        goto end;
    }
 
    ret = _adt_read_unlock(htable->lock);
end:
    return ret;
}

adt_status htable_write_lock(HTABLE htable)
{
    adt_status ret;
    
    if(NULL == htable)
    {
        ret = ADT_INVALID_PARAM;
        goto end;
    }
    
    ret = _adt_write_lock(htable->lock);
    
end:
    return ret;
}

adt_status htable_write_unlock(HTABLE htable)
{
    adt_status ret;
    
    if(NULL == htable)
    {
        ret = ADT_INVALID_PARAM;
        goto end;
    }
    
    ret = _adt_write_unlock(htable->lock);
end:
    return ret;
}

adt_status htable_statistics(HTABLE htable,htable_stats **htable_stats_p)
{
    static htable_stats stats;
    adt_status status;
    size_t length;
    float total_lengths;
    float average_length;
    size_t empty_count;
    size_t i;
    size_t longest;
    size_t shortest;
    
    LIST list;
    
    if(NULL == htable)
    {
        status=ADT_INVALID_PARAM;
        goto end;
    }
    
    _adt_read_lock(htable->lock);
    
    total_lengths=0;
    empty_count=0;
    longest=0;
    shortest=SIZE_MAX;
    
    for(i=0;i<htable->htable_size;i++)
    {
        list=htable->htable_chains[i];
        length=list_length(list);
        if(length > longest)
        {
            longest=length;
        }
        if(length<shortest)
        {
            shortest=length;
        }
        if(0==length)
        {
            empty_count++;
        }else
        {
            total_lengths+=length;
        }
    }
    average_length=(total_lengths / htable->htable_size);
    
    stats.htable_size=htable->htable_size;
    stats.htable_empty_slots=empty_count;
    stats.longest_chain_length=longest;
    stats.average_chain_length=average_length;
    stats.shortest_chain_length=shortest;
    stats.total_items=(size_t)total_lengths;
    
    (*htable_stats_p)=&stats;
    
    status = ADT_OK;
    _adt_read_unlock(htable->lock);
end:
    return status;
}

adt_status htable_safe_insert(HTABLE htable,
                              char *key,
                              size_t key_len,
                              void *value,
                              int lock)
{
    void *temp;
    adt_status status;
    
    if(NULL == htable || NULL == key)
    {
        status=ADT_INVALID_PARAM;
        goto end;
    }
    //TODO Figure out an atomic way to upgrade from read lock to write lock
    if(lock)
    {
        _adt_write_lock(htable->lock);
    }
    
    status=htable_lookup(htable,key,key_len,&temp,0);
    
    switch(status)
    {
    case ADT_NOT_FOUND:
        break;
    case ADT_OK: /* key's already been inserted. Bail. */
        status=ADT_KEY_EXISTS;
        if(lock) { _adt_write_unlock(htable->lock); }
        goto end;
    default:/* uh oh */
        if(lock) { _adt_write_unlock(htable->lock); }
        goto end;
    }

    status=_htable_fast_insert_nolock(htable,key,key_len,value);
    if(lock) { _adt_write_unlock(htable->lock); }
end:
    return status;
}

adt_status htable_safe_insert_uint64(HTABLE htable,
                                     htable_key_uint64_t key,
                                     void *value,
                                     int lock)
{
    void *temp;
    adt_status status;
    
    if(NULL == htable)
    {
        status=ADT_INVALID_PARAM;
        goto end;
    }
    //TODO Figure out an atomic way to upgrade from read lock to write lock
    if(lock) { _adt_write_lock(htable->lock); }
    
    status=htable_lookup_uint64(htable,key,&temp,0);
    
    switch(status)
    {
        case ADT_NOT_FOUND:
            break;
        case ADT_OK: /* key's already been inserted. Bail. */
            status=ADT_KEY_EXISTS;
            if(lock) { _adt_write_unlock(htable->lock); }
            goto end;
        default:/* uh oh */
            if(lock) { _adt_write_unlock(htable->lock); }
            goto end;
    }
    
    status=_htable_fast_insert_uint64_nolock(htable,key,value);
    
    if(lock) { _adt_write_unlock(htable->lock); }
end:
    return status;

}

adt_status htable_fast_insert(HTABLE htable,
                              char *key,
                              size_t key_len,
                              void *value,
                              int lock)
{
    adt_status ret;
    if(NULL == htable)
    {
        return ADT_INVALID_PARAM;
    }
    if(lock) { _adt_write_lock(htable->lock);}
    ret = _htable_fast_insert_nolock(htable, key, key_len, value);
    if(lock) { _adt_write_unlock(htable->lock); }
    
    return ret;
}

adt_status htable_fast_insert_uint64(HTABLE htable,
                                     htable_key_uint64_t key,
                                     void *value,
                                     int lock)
{
    adt_status ret;
    if(NULL == htable)
    {
        return ADT_INVALID_PARAM;
    }
    if(lock) { _adt_write_lock(htable->lock);}
    ret = _htable_fast_insert_uint64_nolock(htable, key, value);
    if(lock) { _adt_write_unlock(htable->lock); }
    
    return ret;
}


static adt_status
_htable_fast_insert_uint64_nolock(HTABLE htable,
                           htable_key_uint64_t key,
                           void *value)
{
    HTNODE htnode;
    LIST list;
    adt_status ret;
    size_t index;
    
    if(NULL == value)
    {
        ret = ADT_INVALID_PARAM;
        goto end;
    }
    htnode=_htnode_create();
    if(NULL==htnode)
    {
        ret=ADT_NO_MEM;
        goto end;
    }
    
    htnode->hash=key;
    htnode->key=NULL;
    htnode->value=value;
    
    index=_htnode_index(htable,key);
    
    list=htable->htable_chains[index];
    ret=lladd(list,htnode,NULL);
end:
    return ret;

}

static adt_status
_htable_fast_insert_nolock(HTABLE htable,
                           char *key,
                           size_t key_len,
                           void *value)
{
    HTNODE htnode;
    LIST list;
    adt_status ret;
    htable_hash_t hash;
    htable_hash32_t hash32;
    size_t index;

    if(NULL == key || NULL == value)
    {
        ret = ADT_INVALID_PARAM;
        goto end;
    }
    htnode=_htnode_create();
    if(NULL==htnode)
    {
        ret=ADT_NO_MEM;
        goto end;
    }
    
    ret=_hash_func(key,key_len,&hash32);
    if(ADT_OK != ret)
    {
        goto end;
    }
    
    htnode->hash=hash=hash32;
    htnode->key=key;
    htnode->value=value;
    
    index=_htnode_index(htable,hash);
    
    list=htable->htable_chains[index];
    ret=lladd(list,htnode,NULL);
    
end:
    return ret;
}



adt_status htable_delete(HTABLE htable,
                         char *key,
                         size_t key_len,
                         void **value_p,
                         int lock)
{
    LIST list;
    NODE llnode;
    HTNODE htnode;
    adt_status status;
    htable_hash_t hash;
    htable_hash32_t hash32;
    size_t index;
    int found=0;
    int end_list=0;
    
    if(NULL == htable || NULL == key || NULL == value_p)
    {
        status = ADT_INVALID_PARAM;
        goto end;
    }


    if(lock) { _adt_write_lock(htable->lock); }
    
    status=_hash_func(key,key_len,&hash32);

    if(ADT_OK != status)
    {
        if(lock) { _adt_write_unlock(htable->lock); }
        goto end;
    }
    hash=hash32;
    index = _htnode_index(htable,hash);
    
    list=htable->htable_chains[index];
    
    
    status=llget_first(list,&llnode,(void **)&htnode);
    switch(status)
    {
    case ADT_OK:
        break;
    case ADT_EMPTY:
        if(lock) { _adt_write_unlock(htable->lock); }
        status=ADT_NOT_FOUND;
        goto end;
    default:
        if(lock) { _adt_write_unlock(htable->lock); }
        goto end;
    }

    found=0;
    
    while(!found && !end_list)
    {
        
        if(0==memcmp(key,htnode->key,key_len))
        {
            (*value_p)=htnode->value;
            found=1;
            (void)llremove_node(list,&llnode,(void **)&htnode);
            _htnode_destroy(&htnode);
        }
        
        status=llget_next(list,&llnode,(void **)&htnode);
        
        if(ADT_END_LIST == status)
        {
            end_list=1;
        }else 
        if(ADT_OK != status)
        {
            break;
        }
        
        
    }
    if(found)
    {
        status=ADT_OK;
        
    }else
    {
        status=ADT_NOT_FOUND;
    }
    if(lock) { _adt_write_unlock(htable->lock); }
end:
    return status;
}

adt_status htable_delete_uint64(HTABLE htable,
                                htable_key_uint64_t key,
                                void **value_p,
                                int lock)
{
    LIST list;
    NODE llnode;
    HTNODE htnode;
    adt_status status;

    size_t index;
    int found=0;
    int end_list=0;
    
    if(NULL == htable || NULL == value_p)
    {
        status = ADT_INVALID_PARAM;
        goto end;
    }
    
    
    if(lock) { _adt_write_lock(htable->lock); }
    
    index = _htnode_index(htable,key);
    
    list=htable->htable_chains[index];
    
    
    status=llget_first(list,&llnode,(void **)&htnode);
    switch(status)
    {
        case ADT_OK:
            break;
        case ADT_EMPTY:
            if(lock) { _adt_write_unlock(htable->lock); }
            status=ADT_NOT_FOUND;
            goto end;
        default:
            if(lock) { _adt_write_unlock(htable->lock); }
            goto end;
    }
    
    found=0;
    
    while(!found && !end_list)
    {
        
        if(key==htnode->hash)
        {
            (*value_p)=htnode->value;
            found=1;
            (void)llremove_node(list,&llnode,(void **)&htnode);
            _htnode_destroy(&htnode);
            break;
        }
        
        status=llget_next(list,&llnode,(void **)&htnode);
        
        if(ADT_END_LIST == status)
        {
            end_list=1;
        }else
            if(ADT_OK != status)
            {
                break;
            }
        
        
    }
    if(found)
    {
        status=ADT_OK;
        
    }else
    {
        status=ADT_NOT_FOUND;
    }
    if(lock) { _adt_write_unlock(htable->lock); }
end:
    return status;
}

adt_status htable_lookup(HTABLE htable,
                         char *key,
                         size_t key_len,
                         void **value_p,
                         int lock)
 {
     LIST list;
     NODE llnode;
     HTNODE htnode;
     adt_status status;
     htable_hash_t hash;
     htable_hash32_t hash32;
     size_t index;
     int found=0;
     int end_list=0;
     
     if(NULL == htable || NULL == key || NULL == value_p)
     {
         status = ADT_INVALID_PARAM;
         goto end;
     }


     if(lock) { _adt_read_lock(htable->lock); }
     
     status=_hash_func(key,key_len,&hash32);

     if(ADT_OK != status)
     {
         if(lock) {_adt_read_unlock(htable->lock);}
         goto end;
     }
     hash=hash32;
     index = _htnode_index(htable,hash);
     
     list=htable->htable_chains[index];
     
     
     status=llget_first(list,&llnode,(void **)&htnode);
     switch(status)
     {
     case ADT_OK:
         break;
     case ADT_EMPTY:
         status=ADT_NOT_FOUND;
         if(lock) {_adt_read_unlock(htable->lock);}
         goto end;
     default:
         if(lock) {_adt_read_unlock(htable->lock);}
         goto end;
     }
     
     found=0;
     while(!found && !end_list)
     {
         if(0==memcmp(key,htnode->key,key_len))
         {
             found=1;
             break;
         }
         
         status=llget_next(list,&llnode,(void **)&htnode);
         
         if(ADT_END_LIST == status)
         {
             end_list=1;
         }else 
         if(ADT_OK != status)
         {
             break;
         }
     }
     
     if(found)
     {
         (*value_p)=htnode->value;
         status=ADT_OK;
     }else
     {
         status=ADT_NOT_FOUND;
     }
     if(lock) {_adt_read_unlock(htable->lock);}
 end:
     return status;
 }

adt_status htable_lookup_uint64(HTABLE htable,
                                htable_key_uint64_t key,
                                void **value_p,
                                int lock)
{
    LIST list;
    NODE llnode;
    HTNODE htnode;
    adt_status status;

    size_t index;
    int found=0;
    int end_list=0;
    
    if(NULL == htable || NULL == value_p)
    {
        status = ADT_INVALID_PARAM;
        goto end;
    }
    
    
     if(lock) { _adt_read_lock(htable->lock); }
    
    
    index = _htnode_index(htable,key);
    
    list=htable->htable_chains[index];
    
    
    status=llget_first(list,&llnode,(void **)&htnode);
    switch(status)
    {
        case ADT_OK:
            break;
        case ADT_EMPTY:
            status=ADT_NOT_FOUND;
         if(lock) {_adt_read_unlock(htable->lock);}
            goto end;
        default:
         if(lock) {_adt_read_unlock(htable->lock);}
            goto end;
    }
    
    found=0;
    while(!found && !end_list)
    {
        if(key == htnode->hash)
        {
            found=1;
            break;
        }
        
        status=llget_next(list,&llnode,(void **)&htnode);
        
        if(ADT_END_LIST == status)
        {
            end_list=1;
        }else
            if(ADT_OK != status)
            {
                break;
            }
    }
    
    if(found)
    {
        (*value_p)=htnode->value;
        status=ADT_OK;
    }else
    {
        status=ADT_NOT_FOUND;
    }
    if(lock) {_adt_read_unlock(htable->lock);}
end:
    return status;
}

adt_status htable_remove_item(HTABLE htable,char **key_p, void **data_p,int lock)
{
    adt_status ret;
    LIST list;
    size_t i;
    int found_item;
    HTNODE htnode;
    
    if(NULL == htable || NULL == key_p || NULL == *key_p || NULL == data_p || NULL == *data_p)
    {
        ret = ADT_INVALID_PARAM;
        goto end;
    }
    
    if(lock) { _adt_write_lock(htable->lock);}

    found_item=0;
    for(i=0;i<htable->htable_size;i++)
    {
        if(htable->htable_chains[i] == NULL)
        {
            ret = ADT_EMPTY;
            continue;
        }

        ret=llremove_last(htable->htable_chains[i], (void **)&htnode);
        if(ADT_OK == ret)
        {
            *key_p = htnode->key;
            *data_p = htnode->value;
            _htnode_destroy(&htnode);
            found_item = 1;

            break;

        }else if(ret == ADT_EMPTY)
        {
            list = htable->htable_chains[i];
            list_destroy(&list);
            htable->htable_chains[i]=NULL;
        }
    }
    
    if(lock) { _adt_write_unlock(htable->lock);}
    
    if(found_item > 0)
    {
        ret = ADT_OK;
    }else if(found_item == 0)
    {
       
        ret = ADT_EMPTY;
    }
    
end:

    return ret;
}

static HTNODE _htnode_create(void)
{
    HTNODE htnode;
    htnode=adt_malloc(sizeof(htnode_t));
    
    if(NULL==htnode)
    {
        return htnode;
    }
    memset(htnode,0,sizeof(htnode_t));
    
    return htnode;
}

static void _htnode_destroy(HTNODE *htnode_p)
{
    if(NULL == htnode_p || NULL == (*htnode_p))
    {
        return;
    }
    
    adt_free(*htnode_p);
    *htnode_p=NULL;
    return;
}

static size_t _htnode_index(HTABLE htable,htable_hash_t hash)
{
    
    return (hash % htable->htable_size);
}

static adt_status _hash_func(const void *key, size_t key_len,uint32_t *val_p)
{
    /*
     * Mastering Algorithms with C, 1st ed. 1999, Kyle Loudon, O'Reilly
     * Page 146.
     */
    uint32_t val;
    uint32_t tmp;
    adt_status ret;
    
    const char *ptr;
    const char *end_ptr;
    if(NULL == key || NULL == val_p)
    {
        ret=ADT_INVALID_PARAM;
        goto end;
    }
    
    val=0;
    ptr=key;
    end_ptr=key+key_len;

    //key_len could overflow end_ptr, espeically if
    //sizeof(key_len) > sizeof(char *)
    if ((end_ptr < ptr) || ((unsigned)(end_ptr - ptr) != key_len))
    {
        ret=ADT_INVALID_PARAM;
        goto end;
    }
    
    while(ptr!=end_ptr)
    {
        val = (val << 4) + (*ptr);
        
        if((tmp=(val & 0xf0000000)))
        {
            val = val ^ (tmp >> 24);
            val = val ^ tmp;
        }
        ptr++;
    }
    
    *val_p=val;
    ret=ADT_OK;
    
end:
    return ret;
}
