///
// hashTableADT.h a C/C++ interface for Hash ADT module
// This hashTableADT implements chain hashing when dealing with collisions
// 
// Author: T. Wilgenbusch
///

#ifndef _HASHTABLEADT_H_
#define _HASHTABLEADT_H_

#include <stdbool.h>

///
// Structure for an entry object to be placed in the hash table
// next - the next entry in the chain
// key - the hash key
// val - the value being stored
///
typedef struct Entry
{
    struct Entry *next;
    void *key;
    void *val;
}Entry;

///
// Structure representing the hash table
// capacity the capacity of the hash table
// size the number of elements currently in the table
// hashFunction the hash function passed in by the user
// equal comparison function for comparing to keys
// printKeyVal prints the contents of a key value pair (for debug purposes)
// table and array of entries
///
struct hashTableADT
{
    unsigned long capacity;
    unsigned long size;
    unsigned long (*hashFunction)(const void *key, const unsigned long capacity);
    bool (*equal)(const void* key1, const void* key2);
    void (*printKeyVal)(const void* key, const void* val);
    Entry **table;
};

/// Creating a typedef for the hashTableADT struct pointer
typedef struct hashTableADT *HashTableADT;

///
// create - creates a new empty hash table
// @param initialCapacity the hash tables initial capacity
// @param hashFunction the user defined hash function
// @param equal the user defined equal function
// @param printKeyVal the user defined print function
// @return a new HashTableADT pointer
///
HashTableADT create(unsigned long initialCapacity, 
    unsigned long (*hashFunction)(const void *key, const unsigned long capacity), 
    bool (*equal)(const void *key1, const void *key2), 
    void (*printKeyVal)(const void* key, const void* val) );

///
// destroy - deallocates all the memory associated with a has table
// @param hTable the hash table being destroyed
///
void destroy(HashTableADT hTable);

///
// contains - determines if a key is in a hash table
// @param hTable - the hash table we are searching through
// @param key - the key we are looking for
// @return true if key is already in hTable, otherwise false
///
bool contains(HashTableADT hTable, void *key);

///
// put - puts a key value pair into a hash table
// @param hTable the table we are updating
//   NOTE: put() requires a double pointer to a hash table struct so that is can
//         successfully resize the hash table if need be
// @param key the key being put into the hash table
// @param val the value being put into the hash table
///
void put(HashTableADT *hTable, void *key, void *val);

///
// get - grabs a value associated with a specific key from a hash table
// @param hTable the hash table we are searching through
// @param key the key associated with the value we are getting
// @return the value associated with the key, or if no value was found NULL
///
void *get(HashTableADT hTable, void *key);

///
// printTable - prints the current hash table to std-out (used for debugging)
// @param hTable the table being printed
///
void printTable(HashTableADT hTable);

#endif