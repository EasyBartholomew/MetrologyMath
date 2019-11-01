#include <math.h>
#include <stdlib.h>
#include "calculations.h"

int calcK(const p_cvector _nums) {

	int pr = (int)(1 + 3.2 * log10(_nums->size)) + 1;

	return pr % 2 ? pr : pr + 1;
}


int compare_double(const void* a, const void* b) {

	if ((*(double*)a - *(double*)b) < 0) {
		return -1;
	}
	else if ((*(double*)a - *(double*)b) > 0)
		return 1;

	return 0;
}

void sortCVec(p_cvector _nums) {
	qsort(_nums->stock, _nums->size, _nums->elem_size, compare_double);
}


double square(double n) {
	return n * n;
}


double calcdX(const p_cvector _nums, int K) {
	qsort(_nums->stock, _nums->size, _nums->elem_size, compare_double);

	double* nums = (double*)_nums->stock;

	return (nums[_nums->size - 1] - nums[0]) / K;
}


double calcAvgX(const p_cvector _nums) {
	qsort(_nums->stock, _nums->size, _nums->elem_size, compare_double);

	double* nums = (double*)_nums->stock;

	return (nums[0] + nums[_nums->size - 1]) / 2.0;
}


double calcX(const p_cvector _nums) {
	double sum = 0.0;
	double* nums = (double*)_nums->stock;

	for (register unsigned i = 0; i < _nums->size; i++)
		sum += nums[i];

	return sum / _nums->size;
}


double calcSigm(const p_cvector _nums, double X) {
	double sum = 0.0;
	double* nums = (double*)_nums->stock;

	for (register unsigned i = 0; i < _nums->size; i++)
		sum += square(nums[i] - X);

	return sqrt(sum / (_nums->size - 1));
}


double calcSx(double sigm, size_t n) {
	return sigm / sqrt(n);
}


void calcRngsSt(OUT p_cvector U, OUT p_cvector Ui, p_cvector source, int K, double dx) {

	double rng[2] = { *(double*)CVectorEnumerate(source, 0), *(double*)CVectorEnumerate(source, 0) };

	double* src = (double*)source->stock;

	rng[1] += dx;

	for (register short i = 0; i < K; i++) {
		CVectorPush(Ui, &rng[0]);
		CVectorPush(U, &rng[1]);

		rng[0] = rng[1];
		rng[1] += dx;
	}
}

cvector calcPi(p_cvector ltable, p_cvector u, p_cvector ui) {

	cvector vec = CreateCVector(sizeof(double), DEFAULT_INITIAL_CAPACITY_VALUE);

	double current = 0;

	for (register short i = 0; i < u->size; i++) {
		current = (((double*)u->stock)[i] -
			((double*)ui->stock)[i]);

		CVectorPush(&vec, &current);
	}

	return vec;
}

cvector calcMT(p_cvector pi, int n) {

	cvector vec = CreateCVector(sizeof(double), DEFAULT_INITIAL_CAPACITY_VALUE);

	double current = 0;

	for (register short i = 0; i < pi->size; i++) {
		current = *(double*)CVectorEnumerate(pi, i) * n;
		CVectorPush(&vec, &current);
	}

	return vec;
}


cvector calcKsy(p_cvector mEm, p_cvector mT) {

	cvector vec = CreateCVector(sizeof(double), DEFAULT_INITIAL_CAPACITY_VALUE);

	double current = 0;

	for (register short i = 0; i < mEm->size; i++) {

		current = *(size_t*)CVectorEnumerate(mEm, i) - *(double*)CVectorEnumerate(mT, i);
		current *= current;
		current /= *(double*)CVectorEnumerate(mT, i);
		CVectorPush(&vec, &current);
	}

	return vec;
}


cvector calcM(p_cvector source, int K, double dx) {

	double rng[2] = { *(double*)CVectorEnumerate(source, 0), *(double*)CVectorEnumerate(source, 0) };
	double* src = (double*)source->stock;

	rng[1] += dx;
	size_t cnt = 1;
	size_t idx = 1;

	cvector m = CreateCVector(sizeof(size_t), DEFAULT_INITIAL_CAPACITY_VALUE);

	for (register short i = 1; i < K; i++) {
		for (; idx < source->size;) {
			if (src[idx] < rng[1]) {
				cnt++;
				idx++;
			}
			else {
				rng[0] = rng[1];
				rng[1] += dx;

				CVectorPush(&m, &cnt);

				cnt = 0;
			}
		}
	}

	if (!*(size_t*)CVectorEnumerate(&m, K - 1))
		(*(size_t*)CVectorEnumerate(&m, K - 1))++;

	return m;
}

cvector calcPEm(p_cvector m_src, int n, double dX) {

	cvector vec = CreateCVector(sizeof(double), DEFAULT_INITIAL_CAPACITY_VALUE);
	size_t* dm_src = (size_t*)m_src->stock;
	double cp = 0;

	for (register short i = 0; i < m_src->size; i++) {
		cp = ((double)dm_src[i]) / (n * dX);
		CVectorPush(&vec, &cp);
	}

	return vec;
}


cvector calct(p_cvector src, double dx, double sigm) {

	cvector t = CreateCVector(sizeof(double), DEFAULT_INITIAL_CAPACITY_VALUE);

	double current = 0;

	for (register short i = 0; i < src->size; i++) {
		current = (*(double*)CVectorEnumerate(src, i) - dx) / sigm;
		CVectorPush(&t, &current);
	}

	return t;
}


cvector calcflaplace(p_cvector t, p_cvector table) {

	cvector lf = CreateCVector(sizeof(double), DEFAULT_INITIAL_CAPACITY_VALUE);
	double current = 0;

	for (register short i = 0; i < t->size; i++) {
		current = flaplace(table, *(double*)CVectorEnumerate(t, i));
		CVectorPush(&lf, &current);
	}

	return lf;
}


double flaplace(p_cvector table, double arg) {

	if (table == NULL) {
		SetLastLocalERROR(LERROR_INVALID_PTR);
		return 0;
	}

	if (table->elem_size != sizeof(dp_t)) {
		SetLastLocalERROR(LERROR_INVALID_PARAM);
		return 0;
	}

	signed char sign = arg < 0 ? -1 : 1;
	arg = fabs(arg);
	arg *= 100;
	arg = round(arg);
	arg /= 100;

	if (arg > 4.99)
		arg = 4.99;

	pdp_t dtable = (pdp_t)table->stock;

	for (register short i = 0; i < table->size; i++) {
		if (fabs(dtable[i].key - arg) < 0.000000001) {
			return sign * dtable[i].value;
		}
	}

	return 0;
}