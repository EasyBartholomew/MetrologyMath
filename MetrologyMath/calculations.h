#ifndef _CALCULATIONS_H_
#define _CALCULATIONS_H_
#include <math.h>
#include "cvector.h"

int K(const p_cvector _nums);
int dX(const p_cvector _nums, int K);
double AvgX(const p_cvector _nums);

double X(const p_cvector _nums);
double Sx(const p_cvector _nums, double X);

#endif // !_CALCULATIONS_H_