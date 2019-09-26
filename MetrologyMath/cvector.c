#include "cvector.h"
#include "lerror.h"

cvector CreateCVector(size_t elem_size, size_t initial_capacity) {
	cvector vector;

	vector.capacity = initial_capacity % 2 ? initial_capacity + 1 : initial_capacity;
	vector.elem_size = elem_size;
	vector.size = 0;

	if (!vector.capacity) {
		vector.capacity = DEFAULT_INITIAL_CAPACITY_VALUE;
	}

	vector.stock = malloc(vector.elem_size * vector.capacity);

	return vector;
}


void DestroyCVector(p_cvector target) {

	if (!target) {
		SetLastLocalERROR(LERROR_INVALID_PTR);
		return;
	}

	if (target->capacity) {
		target->capacity = 0;
		free(target->stock);
	}

	target->stock = NULL;
	target->size = 0;
	target->elem_size = 0;
}


void* CVectorEnumerate(const p_cvector target, size_t idx) {

	if (!target) {
		SetLastLocalERROR(LERROR_INVALID_PTR);
		return NULL;
	}

	if (idx >= target->capacity)
		return NULL;

	return (byte*)target->stock + idx * target->elem_size;
}


void CVectorEnsureCapacity(p_cvector target) {

	if (!target) {
		SetLastLocalERROR(LERROR_INVALID_PTR);
		return;
	}

	if (!target->elem_size)
		return;

	if (!target->capacity) {
		target->capacity = DEFAULT_INITIAL_CAPACITY_VALUE;
		target->stock = malloc(target->elem_size * target->capacity);
		return;
	}

	size_t lost_capacity = target->capacity;
	void* stock_copy = target->stock;

	target->capacity = ENSURE_CAPACITY_DEFAULT * target->capacity;
	target->stock = malloc(target->capacity * target->elem_size);

	memcpy(target->stock, stock_copy, lost_capacity * target->elem_size);

	free(stock_copy);
}


byte CVectorPush(p_cvector target, void const* const value) {

	if (!(target && value)) {

		SetLastLocalERROR(LERROR_INVALID_PTR);
		return 0;
	}

	if (target->size == target->capacity)
		CVectorEnsureCapacity(target);

	if (!target->size)
		memcpy(target->stock, value, target->elem_size);
	else
		memcpy(CVectorEnumerate(target, target->size), value, target->elem_size);
	target->size++;

	return 1;
}


void CVectorPop(p_cvector target) {

	if (!target) {
		SetLastLocalERROR(LERROR_INVALID_PTR);
		return;
	}

	if (!target->size)
		return;

	memset(CVectorEnumerate(target, target->size - 1), 0, target->elem_size);
	target->size--;

	if ((target->capacity >= 4 * (target->size)) &&
		((target->capacity / 2) >= DEFAULT_INITIAL_CAPACITY_VALUE)) {
		void* stock_copy = target->stock;

		target->capacity /= 2;
		target->stock = malloc(target->elem_size * target->capacity);

		memcpy(target->stock, stock_copy, target->size * target->elem_size);

		free(stock_copy);
	}
}


void CVectorClear(p_cvector target) {

	if (!target) {
		SetLastLocalERROR(LERROR_INVALID_PTR);
		return;
	}

	if (target->capacity) {
		free(target->stock);
		target->capacity = DEFAULT_INITIAL_CAPACITY_VALUE;
	}

	target->size = 0;
	target->stock = malloc(target->elem_size * target->capacity);
}