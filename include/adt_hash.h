#ifndef _ADT_HASH_TABLE_H
#define _ADT_HASH_TABLE_H

#include <sys/types.h>
#include "adt_error.h"
#include "adt_list.h"

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
typedef uint64_t htable_key_uint64_t;

/*
 * Create structures for a hash table.
 *
 * Param: htable_size The desired size of the hash table.
 *
 * Returns: A handle to the hash table, or NULL if memory allocation failed.
 */
HTABLE htable_create(size_t htable_size);

/*
 * Free the memory allocated for a hash table, and set the handle to NULL.
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
 * Obtain a read lock on the hash table.
 * Note: this is useful for performing a series of read-only operations while keeping
 * the table in a consistent state. For single operations, there is no need
 * to lock the table externally. Just set the lock flag when calling the appropriate function.
 * Param: [in]htable    The htable object on which to obtain a read lock.
 *
 * Returns: ADT_OK if the lock operation was successful.
 *          ADT_INVALID_PARAM if the htable object is NULL.
 *
 */
adt_status htable_read_lock(HTABLE htable);

/*
 * Release a read lock on the hash table.
 * Note: this is useful for performing a series of read-only operations while keeping
 * the table in a consistent state. For single operations, there is no need
 * to lock the table externally. Just set the lock flag when calling the appropriate function.
 * Param: [in]htable    The htable object on which to release a read lock.
 *
 * Returns: ADT_OK if the unlock operation was successful.
 *          ADT_INVALID_PARAM if the htable object is NULL.
 *
 */
adt_status htable_read_unlock(HTABLE htable);

/*
 * Obtain a write lock on the hash table.
 * Note: this is useful for performing a series of read/write operations while keeping
 * the table in a consistent state. For single operations, there is no need
 * to lock the table externally. Just set the lock flag when calling the appropriate function.
 * Param: [in]htable    The htable object on which to obtain a write lock.
 *
 * Returns: ADT_OK if the lock operation was successful.
 *          ADT_INVALID_PARAM if the htable object is NULL.
 *
 */
adt_status htable_write_lock(HTABLE htable);

/*
 * Relesae a write lock on the hash table.
 * Note: this is useful for performing a series of read/write operations while keeping
 * the table in a consistent state. For single operations, there is no need
 * to lock the table externally. Just set the lock flag when calling the appropriate function.
 * Param: [in]htable    The htable object on which to relese a write lock.
 *
 * Returns: ADT_OK if the unlock operation was successful.
 *          ADT_INVALID_PARAM if the htable object is NULL.
 *
 */
adt_status htable_write_lock(HTABLE htable);


/*
 *Remove an arbitrary node from the hash table. This function is primarily so that
 *the table may have all its items removed before the table is freed.
 *
 *Param: [in]htable     The htable object whose nodes will be removed.
 *Param: [out]key_p     Address of a char pointer that will hold the current item's key.
 *Param: [out]data_p    Address of a void pointer that will hold the current item's data.
 *Param: [in]lock       Flag for whether to lock the table before performing the remove operation.
 * Returns: ADT_OK if the delete was successful.
 *          ADT_EMPTY if the hash table is empty.
 *          ADT_INVALID_PARAM if <key_p> or <value_p> is NULL or point to NULL.
 *          ADT_ERROR on other error.
 */
adt_status htable_remove_item(HTABLE htable,char **key_p, void **data_p, int lock);

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
 * Param: [in]key      Key of type uint64_t to be inserted.
 * Param: [in]value    Pointer to the value object to be associated with <key>.
 *Param: [in]lock       Flag for whether to lock the table before performing the insert operation.
 *
 * Returns: ADT_OK if the insertion was successful.
 *          ADT_INVALID_PARAM if key or value_p is NULL.
 *          ADT_NO_MEM if insertion failed due to memory allocation failure.
 *          ADT_ERROR on other error.
 */
adt_status htable_fast_insert_uint64(HTABLE htable,htable_key_uint64_t key,void *value,int lock);

/*
 * Insert (key,value) without first checking if key exists in the hash table.
 *
 * Param: htable A handle on the hash table to perform the inerstion operation on.
 * Param: [in]key      Pointer to the key to be inserted.
 * Param: [in]key_len  Length of the key (including nul terminator if a string).
 * Param: [in]value    Pointer to the value object to be associated with <key>.
 *Param: [in]lock       Flag for whether to lock the table before performing the insert operation.
 * 
 * Returns: ADT_OK if the insertion was successful.
 *          ADT_INVALID_PARAM if key or value_p is NULL.
 *          ADT_NO_MEM if insertion failed due to memory allocation failure.
 *          ADT_ERROR on other error.
 */
adt_status htable_fast_insert(HTABLE htable,char *key, size_t key_len, void *value,int lock);

/*
 * Insert (key,value) if key does not already exist in the hash table, otherwise
 * fail.
 * 
 * Param: htable A handle on the hash table to perform the insertion operation on.
 * Param: [in]key      Pointer to the key to be inserted.
 * Param: [in]key_len  Length of the key (including nul terminator if a string).
 * Param: [in]value    Pointer to the value object to be associated with <key>.
 *Param: [in]lock       Flag for whether to lock the table before performing the insert operation.
 * 
 * Returns: ADT_OK if the insertion was successful.
 *          ADT_KEY_EXISTS if <key> already exists in the hash table.
 *          ADT_INVALID_PARAM if key or value_p is NULL.
 *          ADT_NO_MEM if insertion failed due to memory allocation failure.
 *          ADT_ERROR on other error.
 */
adt_status htable_safe_insert(HTABLE htable,char *key, size_t key_len, void *value,int lock);

/*
 * Insert (key,value) if key does not already exist in the hash table, otherwise
 * fail.
 *
 * Param: htable A handle on the hash table to perform the insertion operation on.
 * Param: [in]key      Key of type uint64_t to be inserted.
 * Param: [in]value    Pointer to the value object to be associated with <key>.
 *Param: [in]lock       Flag for whether to lock the table before performing the insert operation.
 * Returns: ADT_OK if the insertion was successful.
 *          ADT_KEY_EXISTS if <key> already exists in the hash table.
 *          ADT_INVALID_PARAM if key or value_p is NULL.
 *          ADT_NO_MEM if insertion failed due to memory allocation failure.
 *          ADT_ERROR on other error.
 */
adt_status htable_safe_insert_uint64(HTABLE htable,htable_key_uint64_t key, void *value,int lock);

/*
 * Retreive <value> associated with <key>, if it exists in the hash table.
 * 
 * Param: htable A handle on the hash table to perform the lookup operation on.
 * Param: [in]key      Pointer to the key to be looked up.
 * Param: [in]key_len  Length of the key (including nul terminator if a string).
 * Param: [out]value_p Address of the void pointer that will be set to <value> 
 *         if <key> is found in the hash table.
 *Param: [in]lock       Flag for whether to lock the table before performing the lookup operation.
 *
 * Returns: ADT_OK if the lookup was successful.
 *          ADT_NOT_FOUND if <key> was not found.
 *          ADT_INVALID_PARAM if <key> or <value_p> is NULL or if <value_p>
 *          points to NULL.
 *          ADT_ERROR on other error.
 */
adt_status htable_lookup(HTABLE htable,char *key, size_t key_len, void **value_p, int lock);

/*
 * Retreive <value> associated with <key>, if it exists in the hash table.
 *
 * Param: htable A handle on the hash table to perform the lookup operation on.
 * Param: [in]key      The key of type uint64_t to be looked up.
 * Param: [out]value_p Address of the void pointer that will be set to <value>
 *         if <key> is found in the hash table.
 *Param: [in]lock       Flag for whether to lock the table before performing the lookup operation.
 *
 * Returns: ADT_OK if the lookup was successful.
 *          ADT_NOT_FOUND if <key> was not found.
 *          ADT_INVALID_PARAM if <key> or <value_p> is NULL or if <value_p>
 *          points to NULL.
 *          ADT_ERROR on other error.
 */
adt_status htable_lookup_uint64(HTABLE htable,htable_key_uint64_t key, void **value_p,int lock);

/*
 * Retreive <value> associated with <key>, if <key> exists in the hash table,
 * and removes the pair from the table.
 * 
 * Param: htable A handle on the hash table to perform the delete operation on.
 * Param: [in]key      Pointer to the key to be deleted.
 * Param: [in]key_len  Length of the key (including nul terminator if a string).
 * Param: [out]value_p Address of the void pointer that will be set to <value> 
 *         if <key> is found in the hash table.
 *Param: [in]lock       Flag for whether to lock the table before performing the delete operation.
 *
 * Returns: ADT_OK if the delete was successful.
 *          ADT_NOT_FOUND if <key> was not found.
 *          ADT_INVALID_PARAM if <key> or <value_p> is NULL or if <value_p>
 *          points to NULL.
 *          ADT_ERROR on other error.
 */
adt_status htable_delete(HTABLE htable,char *key, size_t key_len, void **value_p, int lock);

/*
 * Retreive <value> associated with <key>, if <key> exists in the hash table,
 * and removes the pair from the table.
 *
 * Param: htable A handle on the hash table to perform the delete operation on.
 * Param: [in]key      The key of type uint64_t to be deleted.
 * Param: [out]value_p Address of the void pointer that will be set to <value>
 *         if <key> is found in the hash table.
 *Param: [in]lock       Flag for whether to lock the table before performing the delete operation.
 *
 * Returns: ADT_OK if the delete was successful.
 *          ADT_NOT_FOUND if <key> was not found.
 *          ADT_INVALID_PARAM if <key> or <value_p> is NULL or if <value_p>
 *          points to NULL.
 *          ADT_ERROR on other error.
 */
adt_status htable_delete_uint64(HTABLE htable,htable_key_uint64_t key, void **value_p,int lock);

#endif /* _ADT_HASH_TABLE_H */