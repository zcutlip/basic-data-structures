#include "adt_conditional_includes.h"

#include <stdint.h>
#include <string.h>


#include "adt_hash.h"
#include "adt_list.h"
#include "adt_error.h"
#include "htable_primes.h"

typedef uint32_t htable_hash_t;

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
    LIST htable_chains[0];
};

static adt_status _hash_func(const void *key, size_t key_len, uint32_t *val_p);
static HTNODE _htnode_create(void);
static void _htnode_destroy(HTNODE *htnode_p);
static size_t _htnode_index(HTABLE htable, htable_hash_t hash);

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
    size_t i;
    if(NULL == htable_p || NULL == (*htable_p))
    {
        return;
    }
    
    htable=(*htable_p);
    
    for(i=0;i<htable->htable_size;i++)
    {
        list_destroy(&(htable->htable_chains[i]));
    }
    free(*htable_p);
    (*htable_p)=NULL;
    return;
}

HTABLE htable_create(size_t size)
{
    HTABLE htable;
    size_t i;
    htable=malloc(sizeof(struct htable_struct) + size*sizeof(LIST));
    
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
end:
    return htable;
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
    
    length=0;
    total_lengths=0;
    average_length=0;
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
end:
    return status;
}

adt_status htable_safe_insert(HTABLE htable,
                              char *key,
                              size_t key_len,
                              void *value)
{
    void *temp;
    adt_status status;
    
    if(NULL == htable || NULL == key)
    {
        status=ADT_INVALID_PARAM;
        goto end;
    }
    
    status=htable_lookup(htable,key,key_len,&temp);
    
    switch(status)
    {
    case ADT_NOT_FOUND:
        break;
    case ADT_OK: /* key's already been inserted. Bail. */
        status=ADT_KEY_EXISTS;
        goto end;
    default:/* uh oh */
        goto end;
    }

    status=htable_fast_insert(htable,key,key_len,value);
    
end:
    return status;
}

adt_status htable_fast_insert(HTABLE htable,
                        char *key,
                        size_t key_len,
                        void *value)
{
    HTNODE htnode;
    LIST list;
    adt_status ret;
    htable_hash_t hash;
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
    
    ret=_hash_func(key,key_len,&hash);
    if(ADT_OK != ret)
    {
        goto end;
    }
    htnode->hash=hash;
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
                         void **value_p)
{
    LIST list;
    NODE llnode;
    HTNODE htnode;
    adt_status status;
    htable_hash_t hash;
    size_t index;
    int found=0;
    int end_list=0;
    
    if(NULL == key || NULL == value_p)
    {
        status = ADT_INVALID_PARAM;
        goto end;
    }
    status=_hash_func(key,key_len,&hash);
    if(ADT_OK != status)
    {
        goto end;
    }
    
    index = _htnode_index(htable,hash);
    
    list=htable->htable_chains[index];
    
    
    status=llget_first(list,&llnode,(void **)&htnode);
    switch(status)
    {
    case ADT_OK:
        break;
    case ADT_EMPTY:
        status=ADT_NOT_FOUND;
        goto end;
    default:
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
end:
    return status;
}

adt_status htable_lookup(HTABLE htable,
                         char *key,
                         size_t key_len,
                         void **value_p)
 {
     LIST list;
     NODE llnode;
     HTNODE htnode;
     adt_status status;
     htable_hash_t hash;
     size_t index;
     int found=0;
     int end_list=0;
     
     if(NULL == key || NULL == value_p)
     {
         status = ADT_INVALID_PARAM;
         goto end;
     }
     status=_hash_func(key,key_len,&hash);
     if(ADT_OK != status)
     {
         goto end;
     }
     
     index = _htnode_index(htable,hash);
     
     list=htable->htable_chains[index];
     
     
     status=llget_first(list,&llnode,(void **)&htnode);
     switch(status)
     {
     case ADT_OK:
         break;
     case ADT_EMPTY:
         status=ADT_NOT_FOUND;
         goto end;
     default:
         goto end;
     }
     
     found=0;
     while(!found && !end_list)
     {
         if(0==memcmp(key,htnode->key,key_len))
         {
             found=1;
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
 end:
     return status;
 }

static HTNODE _htnode_create(void)
{
    HTNODE htnode;
    htnode=malloc(sizeof(htnode_t));
    
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
    
    free(*htnode_p);
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
        tmp=0;
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
