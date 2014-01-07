#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>

#include "adt_hash.h"
#include "adt_test_data.h"
#include "adt_stack.h"
#include "adt_error.h"

#define DATA_COUNT 50000

adt_status test_htable_insert(void)
{
    adt_status status;
    size_t htable_size;
    int i;
    HTABLE htable;
    adt_test_data *td1;
    adt_test_data *td2;
    adt_test_item *t_item;
    htable_stats *stats;
    char *data;
    
    printf("%s\n",STARS);
    printf("Testing htable_fast_insert()\n\n");

    printf("Creating test data.\n");
    
    td1=adt_test_data_create(DATA_COUNT);
    td2=adt_test_data_create(DATA_COUNT);
    
    if(NULL == td1 || NULL == td2)
    {
        status=ADT_NO_MEM;
        goto end;
    }
    
    status=htable_closest_prime(10000,&htable_size);
    if(ADT_OK != status)
    {
        printf("Got error back from htable_closest_prime() %d\n", status);
        goto end;
    }
    
    printf("Creating hash table of size: %lu\n", htable_size);
    
    htable=htable_create(htable_size);
    
    printf("Inserting items into hash table.\n");
    
    for(i=0;i<DATA_COUNT;i++)
    {
        t_item=td1->test_items[i];
        status=htable_fast_insert(htable,t_item->key,strlen(t_item->key),t_item->data);
        assert(ADT_OK==status);
    }
    
    printf("Hash table insert test successful.\n");
    
    status=htable_statistics(htable,&stats);
    printf("Hashtable size: %lu\n",stats->htable_size);
    printf("Number of empty chains: %lu\n",stats->htable_empty_slots);
    printf("Shortest chain: %lu\n",stats->shortest_chain_length);
    printf("Longest chain: %lu\n",stats->longest_chain_length);
    printf("Average chain length: %f\n",stats->average_chain_length);
    
    printf("Removing items from hash table.\n");
    for(i=0;i<DATA_COUNT;i++)
    {
        t_item=td2->test_items[i];
        status=htable_delete(htable,t_item->key,strlen(t_item->key),(void **)&data);
        if(ADT_OK !=status)
        {
            printf("Got back from htable_delete(): %d\n",status);
        }
        assert(ADT_OK == status);
        status=strcmp(t_item->data,data);
        if(status !=0)
        {
            printf("Expected: %s\n",t_item->data);
            printf("Key: %s\n",t_item->key);
            printf("Got: %s\n",data);
        }
        assert(status == 0);
    }
    
    printf("Successfully removed all items from hash table.\n");
    
    printf("Destroying hash table.\n");
    htable_destroy(&htable);
    
    
end:
    printf("Destroying test data.\n");
    adt_test_data_destroy(&td1);
    adt_test_data_destroy(&td2);
    return status;
}

adt_status test_htable_safe_insert(void)
{
    adt_status status;
    size_t htable_size;
    int i;
    int random_item;
    HTABLE htable;
    adt_test_data *td1;
    adt_test_data *td2;
    adt_test_item *t_item;
    htable_stats *stats;
    char *data;
    printf("%s\n",STARS);
    printf("Testing htable_safe_insert()\n\n");

    printf("Creating test data.\n");
    
    td1=adt_test_data_create(DATA_COUNT);
    td2=adt_test_data_create(DATA_COUNT);
    
    if(NULL == td1 || NULL == td2)
    {
        status=ADT_NO_MEM;
        goto end;
    }
    
    status=htable_closest_prime(10000,&htable_size);
    if(ADT_OK != status)
    {
        printf("Got error back from htable_closest_prime() %d\n", status);
        goto end;
    }
    
    printf("Creating hash table of size: %lu\n", htable_size);
    
    htable=htable_create(htable_size);
    
    printf("Inserting items into hash table.\n");
    
    for(i=0;i<DATA_COUNT;i++)
    {
        t_item=td1->test_items[i];
        status=htable_safe_insert(htable,t_item->key,strlen(t_item->key)+1,t_item->data);
        assert(ADT_OK==status);
    }
    
    (void)htable_statistics(htable,&stats);
    assert(stats->total_items == DATA_COUNT);
    
    printf("Attempting to insert duplicate items.\n");
    
    srandom((unsigned)time(NULL));
    
    for(i=0;i<5;i++)
    {
        random_item=random() % DATA_COUNT;
        t_item=td1->test_items[random_item];
        status=htable_safe_insert(htable,t_item->key,strlen(t_item->key)+1,
                                      t_item->data);
        assert(status==ADT_KEY_EXISTS);
        (void)htable_statistics(htable,&stats);
        assert(stats->total_items==DATA_COUNT);
    }
    
    printf("Success.\n");
    
    printf("Removing items from hash table.\n");
    
    for(i=0;i<DATA_COUNT;i++)
    {
        t_item=td2->test_items[i];
        data=NULL;
        status=htable_delete(htable,t_item->key,strlen(t_item->key)+1,(void **)&data);
        if(ADT_OK !=status)
        {
            printf("Got back from htable_delete(): %d\n",status);
        }
        assert(ADT_OK == status);
        status=strcmp(t_item->data,data);
        if(status !=0)
        {
            printf("Expected: %s\n",t_item->data);
            printf("Key: %s\n",t_item->key);
            printf("Got: %s\n",data);
        }
        assert(status == 0);
    }
    
    printf("Successfully removed all items from hash table.\n");
    
    printf("Destroying hash table.\n");
    htable_destroy(&htable);
    
    
end:
    printf("Destroying test data.\n");
    adt_test_data_destroy(&td1);
    adt_test_data_destroy(&td2);
    return status;
}

int main(void)
{
    adt_status status;
    
    status=test_htable_insert();
    assert(ADT_OK == status);
    
    status=test_htable_safe_insert();
    printf("All tests successful.\n");
    printf("%s\n\n",STARS);
    
    return 0;
}