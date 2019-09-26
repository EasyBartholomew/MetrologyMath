#ifndef _CALCULATIONS_H_
#define _CALCULATIONS_H_
#include <math.h>
#include "cvector.h"

int calcK(const p_cvector _nums);
int calcdX(const p_cvector _nums, int K);
double calcAvgX(const p_cvector _nums);

double calcX(const p_cvector _nums);
double calcSx(const p_cvector _nums, double X);

#endif // !_CALCULATIONS_H_