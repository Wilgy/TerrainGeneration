///
// floatVector.h
//
// A replacement for the C++ STL vector<float> class.
//
// Author:  Warren R. Carithers
// Date:    2013/10/16 10:43:50
///

#ifndef _FLOATVECTOR_H_
#define _FLOATVECTOR_H_

#include <sys/types.h>

///
// define an alternative to the STL vector<float> class
///
typedef struct floatVector 
{
	size_t size;	// number of occupied slots in the vector
	size_t length;	// total number of slots in the vector
	size_t growth;	// how many slots to add when growing the vector
	float *vec;	// the vector itself
} floatVector_t;

///
// Manipulation functions
///

///
// floatVectorGrowthFactor -- set the growth factor for a floatVector
//
// Returns the old growth factor.
///
size_t floatVectorGrowthFactor( floatVector_t *vec, size_t size );

///
// floatVectorClear -- return a floatVector to its original state
///
void floatVectorClear( floatVector_t *vec );

///
// floatVectorPushBack -- add a float to the end of the vector,
// automatically extending the vector if need be
//
// If the vector must be extended and the memory reallocation
// fails, this method will print an error message and exit.
///
void floatVectorPushBack( floatVector_t *vec, float coord );

///
// Pseudo-function: return the count of elements in a floatVector_t
//
// Note: the argument is a pointer, for consistency with the
// other methods that operate on floatVector_t variables.
///
#define floatVectorSize(fvp)  ((fvp)->size)

#endif
