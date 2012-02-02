#include <stdio.h>
#include <stdlib.h>

#include "adt_test_data.h"

#define DATA_COUNT 100
int main(void)
{
    int i;
    adt_test_data *t_data;
    adt_test_item *t_item;

    t_data=adt_test_data_create(DATA_COUNT);

    if(NULL==t_data)
    {
        printf("t_data came back NULL.\n");
        exit(1);
    }
    
    if(DATA_COUNT != t_data->item_count)
    {
        printf("t_data->item_count doesn't match DATA_COUNT. %u != %u",t_data->item_count,DATA_COUNT);
    }


    for(i=0;i<DATA_COUNT;i++)
    {
        t_item=t_data->test_items[i];
        printf("Got: %s\n",t_item->data);
    }

    adt_test_data_destroy(&t_data);

    return 0;
}
