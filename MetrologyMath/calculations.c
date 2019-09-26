#include <math.h>
#include <stdlib.h>
#include "calculations.h"


int K(const p_cvector _nums) {
	return (int)(1 + 3.2 * log10(_nums->size)) + 1;
}


int compare_double(const void* a, const void* b) {

	if ((*(double*)a - *(double*)b) < 0) {
		return -1;
	}
	else if ((*(double*)a - *(double*)b) > 0)
		return 1;

	return 0;
}


double square(double n) {
	return n * n;
}


int dX(const p_cvector _nums, int K) {
	qsort(_nums->stock, _nums->size, _nums->elem_size, compare_double);

	double* nums = (double*)_nums->stock;

	return (int)((nums[_nums->size - 1] - nums[0]) / K) + 1;
}


double AvgX(const p_cvector _nums) {
	qsort(_nums->stock, _nums->size, _nums->elem_size, compare_double);

	double* nums = (double*)_nums->stock;

	return (nums[0] + nums[_nums->size - 1]) / 2.0;
}


double X(const p_cvector _nums) {
	double sum = 0.0;
	double* nums = (double*)_nums->stock;

	for (register unsigned i = 0; i < _nums->size; i++)
		sum += nums[i];

	return sum / _nums->size;
}


double Sx(const p_cvector _nums, double X) {
	double sum = 0.0;
	double* nums = (double*)_nums->stock;

	for (register unsigned i = 0; i < _nums->size; i++)
		sum += square(nums[i] - X);

	return sum / (_nums->size - 1);
}