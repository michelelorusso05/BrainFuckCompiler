/*
	dynamicArray.c
	Lorusso Michele - 2021
	This is the implementation file for dynamicArray.h.
*/

#include <stdlib.h>

#include "dynamicArray.h"

// Init the array and allocate some memory
void initArray(DynamicArray *a, size_t initialSize)
{
	a->array = malloc(initialSize * sizeof(char));
	a->used = 0;
	a->size = initialSize;
}

// Insert a new element
void insertElement(DynamicArray *a, char element)
{
	// If the array is full, increase the size
	if (a->used == a->size)
	{
		a->size *= 2;
		a->array = realloc(a->array, a->size * sizeof(char));
	}
	a->array[a->used++] = element;
}

// Free the memory used by the array
void destroyArray(DynamicArray *a)
{
	free(a->array);
	a->array = NULL;
	a->used = a->size = 0;
}