#ifndef _CVECTOR_H_
#define _CVECTOR_H_

#include <stdlib.h>
#include <memory.h>

typedef unsigned char byte;

typedef struct {

	size_t capacity;

	size_t size;

	size_t elem_size;

	void* stock;

}cvector, * p_cvector;


#define DEFAULT_INITIAL_CAPACITY_VALUE	0x00000004


#ifndef ENSURE_CAPACITY_DEFAULT
#define ENSURE_CAPACITY_DEFAULT			0x00000002
#endif // !ENSURE_CAPACITY_DEFAULT


//If "initial_capacity" is zero using then "initial_capacity" is DEFAULT_INITIAL_CAPACITY_VALUE
cvector CreateCVector(size_t elem_size, size_t initial_capacity);
void DestroyCVector(p_cvector);

byte CVectorPush(p_cvector target, void const* const value);
void CVectorPop(p_cvector target);
void CVectorClear(p_cvector);
void* CVectorEnumerate(const p_cvector, size_t idx);

#endif // !_CVECTOR_H_