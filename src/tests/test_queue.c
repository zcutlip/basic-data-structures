#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "adt_test_data.h"
#include "adt_queue.h"
#include "adt_error.h"

#define DATA_COUNT 10000

adt_status test_insert_remove(void)
{
    adt_status status;
    QUEUE queue;
    int i;
    char *data;
    adt_test_data *td1;
    adt_test_data *td2;
    adt_test_item *t_item;
    
    printf("%s\n",STARS);
    printf("Testing insert and remove\n\n");

    printf("Creating test data.\n");

    td1=adt_test_data_create(DATA_COUNT);
    td2=adt_test_data_create(DATA_COUNT);

    if(NULL == td1 || NULL == td2)
    {
        status=ADT_NO_MEM;
        goto end;
    }

    queue=queue_create();
    printf("Testing insert()\n");
    for(i=0;i<DATA_COUNT;i++)
    {
        t_item=td1->test_items[i];
        status=queue_insert(queue,t_item->data);
        assert(ADT_OK == status);
    }
    
    assert(queue_size(queue) == DATA_COUNT);
    printf("insert() test successful.\n");
    
    printf("Testing remove().\n");
    for(i=0;i<DATA_COUNT;i++)
    {
        t_item=td2->test_items[i];
        status=queue_remove(queue,(void **)&data);
        assert(ADT_OK == status);
        assert(strcmp(t_item->data,data) == 0);
    }
    
    assert(queue_size(queue) == 0);
    printf("remove() test successful.\n");
    
    printf("Destroying queue.\n");
    queue_destroy(&queue);
    printf("Destroyed queue.\n");

    status=ADT_OK;
    printf("Test successful.\n");
end:
    printf("Destroying test data.\n\n");
    adt_test_data_destroy(&td1);
    adt_test_data_destroy(&td2);
    return status;
}


int main(int argc, char **argv)
{
    adt_status status;

    status=test_insert_remove();
    assert(ADT_OK==status);

    printf("All tests successful.\n");
    printf("%s\n\n",STARS);

    return 0;
}


