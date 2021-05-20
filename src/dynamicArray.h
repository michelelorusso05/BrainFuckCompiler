/*
	dynamicArray.h
	Lorusso Michele - 2021
	This header file contains the DynamicArray struct as well as stubs for the relative functions.
*/

#include <stdlib.h>

typedef struct {
	char* array;
	size_t used;
	size_t size;
} DynamicArray;

void initArray(DynamicArray *a, size_t initialSize);
void insertElement(DynamicArray *a, char element);
void destroyArray(DynamicArray *a);