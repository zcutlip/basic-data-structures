#ifndef _ADT_HASH_TABLE_H
#define _ADT_HASH_TABLE_H

#include <sys/types.h>
#include "adt_error.h"

typedef struct htable_stats_struct
{
    size_t htable_size;
    size_t htable_empty_slots;
    size_t longest_chain_length;
    size_t shortest_chain_length;
    size_t total_items;
    float average_chain_length;
} htable_stats;

typedef struct htable_struct *HTABLE;

typedef unsigned char *htable_key_t;

/*
 * Create structures for a hash table.
 *
 * Param: htable_size The desired size of the hash table.
 *
 * Returns: A handle to the hash table, or NULL if memory allocation failed.
 */
HTABLE htable_create(size_t htable_size);

/*
 * Free the memory allocated for a hash tablel, and set the handle to NULL.
 *
 * Note: Memory for the objects and keys stored in the hash table is not freed.
 *       This is the responsibility of the caller.
 *
 * Param: [in/out]htable_p Pointer to the hash table that will be freed.
 *        The handle referenced by this pointer will be set to NULL.
 *
 * Returns: void.
 */
void htable_destroy(HTABLE *htable_p);

/*
 * Calculate statistics for the given hash table, including: hash table size,
 * number of empty slots, length of the longest chain, length of the shortest
 * chain (0 if there are any empty slots), average chain length.
 *
 * Param: htable The hash table to compute statistics for.
 * Param: htable_stats_p The address of the htable_stats pointer that will point
 *        to the hash table statistics structure.
 *
 * Returns: void
 */
adt_status htable_statistics(HTABLE htable,htable_stats **htable_stats_p);

/*
 * Find the closest prime larger than size. Useful for choosing a size for the
 * hash table.
 * Param: [in]size The desired size of the hashtable
 * Param: [out]size_p Pointer to a size_t that will contain the chosen size.
 * Returns: ADT_OK if a prime larger than size was found.
 *          ADT_ERR if size was larger than the largest in the static list of primes.
 *          ADT_INVALID_PARAM if size_p is NULL.
 */
adt_status htable_closest_prime(size_t size,size_t *size_p);


/*
 * Insert (key,value) without first checking if key exists in the hash table.
 *
 * Param: htable A handle on the hash table to perform the inerstion operation on.
 * Param: [in]key      Pointer to the key to be inserted.
 * Param: [in]key_len  Length of the key (including nul terminator if a string).
 * Param: [in]value    Pointer to the value object to be associated with <key>.
 * 
 * Returns: ADT_OK if the insertion was successful.
 *          ADT_INVALID_PARAM if key or value_p is NULL.
 *          ADT_NO_MEM if insertion failed due to memory allocation failure.
 *          ADT_ERROR on other error.
 */
adt_status htable_fast_insert(HTABLE htable,char *key, size_t key_len, void *value);

/*
 * Insert (key,value) if key does not already exist in the hash table, otherwise
 * fail.
 * 
 * Param: htable A handle on the hash table to perform the inerstion operation on.
 * Param: [in]key      Pointer to the key to be inserted.
 * Param: [in]key_len  Length of the key (including nul terminator if a string).
 * Param: [in]value    Pointer to the value object to be associated with <key>.
 * 
 * Returns: ADT_OK if the insertion was successful.
 *          ADT_KEY_EXISTS if <key> already exists in the hash table.
 *          ADT_INVALID_PARAM if key or value_p is NULL.
 *          ADT_NO_MEM if insertion failed due to memory allocation failure.
 *          ADT_ERROR on other error.
 */
adt_status htable_safe_insert(HTABLE htable,char *key, size_t key_len, void *value);

/*
 * Retreive <value> associated with <key>, if it exists in the hash table.
 * 
 * Param: htable A handle on the hash table to perform the inerstion operation on.
 * Param: [in]key      Pointer to the key to be inserted.
 * Param: [in]key_len  Length of the key (including nul terminator if a string).
 * Param: [out]value_p Address of the void pointer that will be set to <value> 
 *         if <key> is found in the hash table.
 *
 * Returns: ADT_OK if the lookup was successful.
 *          ADT_NOT_FOUND if <key> was not found.
 *          ADT_INVALID_PARAM if <key> or <value_p> is NULL or if <value_p>
 *          points to NULL.
 *          ADT_ERROR on other error.
 */
adt_status htable_lookup(HTABLE htable,char *key, size_t key_len, void **value_p);

/*
 * Retreive <value> associated with <key>, if <key> exists in the hash table,
 * and removes the pair from the table.
 * 
 * Param: htable A handle on the hash table to perform the inerstion operation on.
 * Param: [in]key      Pointer to the key to be inserted.
 * Param: [in]key_len  Length of the key (including nul terminator if a string).
 * Param: [out]value_p Address of the void pointer that will be set to <value> 
 *         if <key> is found in the hash table.
 *
 * Returns: ADT_OK if the lookup was successful.
 *          ADT_NOT_FOUND if <key> was not found.
 *          ADT_INVALID_PARAM if <key> or <value_p> is NULL or if <value_p>
 *          points to NULL.
 *          ADT_ERROR on other error.
 */
adt_status htable_delete(HTABLE htable,char *key, size_t key_len, void **value_p);


#endif /* _ADT_HASH_TABLE_H */