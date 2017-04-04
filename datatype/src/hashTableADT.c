///
// hashTableADT.c a C/C++ implementation for Hash ADT module
//  
// Author: T. Wilgenbusch
///

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "hashTableADT.h"

/// The load factor used when the table needs to be resized
#define LOAD_FACTOR .75

///
// makeEntry - creates an entry pointer
// @param key the key for this entry
// @param val the value for this entry
// @return a pointer to an Entry struct
///
static Entry *makeEntry(void *key, void*val)
{
    Entry *entry = (Entry *)malloc(sizeof(Entry));
    entry->next = NULL;
    entry->key = key;
    entry->val = val;
    return entry;
}

///
// create - creates a new empty hash table
// @param initialCapacity the hash tables initial capacity
// @param hashFunction the user defined hash function
// @param equal the user defined equal function
// @param printKeyVal the user defined print function
// @return a new HashTableADT pointer
///
HashTableADT create( unsigned long initialCapacity, 
    unsigned long (*hashFunction)(const void *key, const unsigned long capacity), 
    bool (*equal)(const void *key1, const void *key2), 
    void (*printKeyVal)(const void* key, const void* val) )
{
    //allocate space for the hTable
    HashTableADT hTable = (HashTableADT)malloc(sizeof(struct hashTableADT));

    //Set default values
    hTable->capacity = initialCapacity;
    hTable->size = 0;
    hTable->hashFunction = hashFunction;
    hTable->printKeyVal = printKeyVal;
    hTable->equal = equal;


    // Create a pointer to an Entry **, allocating enough space for the 
    // initialCapacity of Entry * and set all of those pointers to NULL;
    hTable->table = (Entry **)malloc(sizeof(Entry *) * initialCapacity);
    for(unsigned long i = 0; i < initialCapacity; i++)
    {
        hTable->table[i] = NULL;
    }

    return hTable;
}

///
// freeChain - helper function used by destroy() and resize() to deallocate a 
//              chain of entries in a hash table
// @param chain - the beginning of a chain of entries to be removed
///
void freeChain(Entry *chain)
{
    // We've reached the last entry in the chain free it
    if(chain != NULL && chain->next == NULL)
    {
        free(chain);
        return;
    }

    // Recurse through chain until we reach the last element, then free the 
    // elements in reverse order
    freeChain(chain->next);

    // Free the current entry
    if(chain != NULL)
    {
        free(chain);
    }
}

///
// destroy - deallocates all the memory associated with a has table
// @param hTable the hash table being destroyed
///
void destroy(HashTableADT hTable)
{
    for(unsigned long i = 0; i < hTable->capacity; i++)
    {
        // Ignore the the NULL entries in the hash table
        if(hTable->table[i] != NULL)
        {
            // Free the entry
            freeChain(hTable->table[i]);
        }
    }

    free(hTable->table);
    free(hTable);
}

///
// contains - determines if a key is in a hash table
// @param hTable - the hash table we are searching through
// @param key - the key we are looking for
// @return true if key is already in hTable, otherwise false
///
bool contains(HashTableADT hTable, void *key)
{
    unsigned long index = hTable->hashFunction(key, hTable->capacity);
    
    // If entry is NULL, then this key is not in the hTable, return false
    if(hTable->table[index] != NULL)
    {
        Entry *entry = hTable->table[index];
        while(entry != NULL)
        {
            if(hTable->equal(key, entry->key))
            {
                return true;
            }
            entry = entry->next;
        }
    }

    return false;
}

///
// resize - used to resize the hash function  after it reaches a certain load
// @param oldHTable the older (smaller) hash function
// @param newHTable a new empty hTable with a larger capacity
///
void resize(HashTableADT oldHTable, HashTableADT newHTable)
{
    Entry ** oldTable = oldHTable->table;
    
    // Go through all of the old entries, putting them in the new table
    for(unsigned long i = 0; i < oldHTable->capacity; i++)
    {
        if(oldTable[i] != NULL)
        {
            Entry *entry = oldTable[i];
            Entry *first_entry = entry;
            while(entry != NULL)
            {
                put(&newHTable, entry->key, entry->val);
                entry = entry->next;
            }

            // remove old (now unused) entries
            freeChain(first_entry);
        }
    }
}

///
// put - puts a key value pair into a hash table
// @param hTable the table we are updating
//   NOTE: put() requires a double pointer to a hash table struct so that is can
//         successfully resize the hash table if need be
// @param key the key being put into the hash table
// @param val the value being put into the hash table
///
void put(HashTableADT *hTablePtr, void *key, void *val)
{
    HashTableADT hTable = *hTablePtr;
    unsigned long index = hTable->hashFunction(key, hTable->capacity);

    // If this index is not NULL, then there have already been entries that 
    // have hashed to this location
    if(hTable->table[index] != NULL)
    {
        Entry *entry = hTable->table[index];
        Entry *prev = NULL;
        while(entry != NULL)
        {
            // If we find the key we are currently trying to insert, update 
            // this entries val and exit
            if(hTable->equal(key, entry->key))
            {
                entry->val = val;
                return;
            }
            prev = entry;
            entry = entry->next;
        }

        // If we go through the whole chain and don't find the key we want 
        // to insert, then make a new entry and add it to the end of the chain
        Entry *newEntry = makeEntry(key, val);

        // This shouldn't happen, but should check anyway; if this check fails, 
        // then don't add the entry
        if(prev != NULL)
        {
            prev->next = newEntry;
            hTable->size += 1;
        }
    }

    // This index has nothing there, put the new entry here
    else
    {
        Entry *newEntry = makeEntry(key, val);
        hTable->table[index] = newEntry;
        hTable->size += 1;
    }

    // If our size is close to reach full capacity, we need to resize the hashtable
    if(hTable->size >= LOAD_FACTOR * (hTable->capacity))
    {
        // Increase the size of our resize table and create a new, empty table
        unsigned long new_capacity = hTable->capacity * 2 + 1;
        HashTableADT newHTable = create(new_capacity, 
            hTable->hashFunction, hTable->equal, hTable->printKeyVal);
        
        // fill the new table with all of the old entries
        resize(hTable, newHTable);

        // Deallocate the old hash table memory
        HashTableADT oldHTable = hTable;
        free(oldHTable->table);
        free(oldHTable);
        
        // Set our hash table pointer to be the new table
        *hTablePtr = newHTable;
    }
}

///
// get - grabs a value associated with a specific key from a hash table
// @param hTable the hash table we are searching through
// @param key the key associated with the value we are getting
// @return the value associated with the key, or if no value was found NULL
///
void *get(HashTableADT hTable, void *key)
{
    unsigned long index = hTable->hashFunction(key, hTable->capacity);
    
    // If the index is empty, then this key does not exist in hTable
    if(hTable->table[index] != NULL)
    {
        // Go through the chain of entries and find the one that matches our key
        Entry *entry = hTable->table[index];
        while(entry != NULL)
        {

            // Return the value
            if(hTable->equal(key, entry->key))
            {
                return entry->val;
            }
            entry = entry->next;
        }
    }

    return NULL;
}

///
// printTable - prints the current hash table to std-out (used for debugging)
// @param hTable the table being printed
///
void printTable(HashTableADT hTable)
{
    // Go through all of the entries in the table
    for(unsigned long i = 0; i < hTable->capacity; i++)
    {
        // Ignore spots that have not been filled
        if(hTable->table[i] != NULL)
        {
            Entry *entry = hTable->table[i];
            while(entry != NULL)
            {
                hTable->printKeyVal(entry->key, entry->val);

                // To indicate chaining (important to verify how well the 
                // hashFunction is working)
                if(entry->next != NULL)
                {
                    printf("    ->");
                }
                entry = entry->next;
            }
        }
    }
}