#ifndef _ADT_TEST_DATA_H
#define _ADT_TEST_DATA_H

#define ADT_TEST_MAX_STRLEN 1024

#define STARS "*******************************************"


typedef struct adt_test_item_struct
{
    unsigned int number;
    char key[ADT_TEST_MAX_STRLEN];
    char data[ADT_TEST_MAX_STRLEN];
    int removed;
} adt_test_item;

typedef struct adt_test_data_struct
{
    int item_count;
    int current_item;
    adt_test_item **test_items;
} adt_test_data;

adt_test_data *adt_test_data_create(unsigned int count);
void adt_test_data_destroy(adt_test_data **tdata_p);

#endif /* _ADT_TEST_DATA_H */

