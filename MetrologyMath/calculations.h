#ifndef _CALCULATIONS_H_
#define _CALCULATIONS_H_
#include <math.h>
#include "cvector.h"
#include "parse.h"

#define OUT

void sortCVec(p_cvector _nums);
int calcK(const p_cvector _nums);
double calcdX(const p_cvector _nums, int K);
double calcAvgX(const p_cvector _nums);

double calcX(const p_cvector _nums);
double calcSigm(const p_cvector _nums, double X);
double calcSx(double sigm, size_t n);

void calcRngsSt(OUT p_cvector U, OUT p_cvector Ui, p_cvector source, int K, double dx);
cvector calcPi(p_cvector ltable, p_cvector t, p_cvector t_prev);
cvector calcMT(p_cvector pi, int n);
cvector calcKsy(p_cvector mEm, p_cvector mT);
cvector calcM(p_cvector source, int K, double dx);
cvector calcPEm(p_cvector m_src, int n, double dX);
cvector calct(p_cvector src, double dx, double sigm);
cvector calcflaplace(p_cvector t, p_cvector table);

double flaplace(p_cvector table, double arg);

#endif // !_CALCULATIONS_H_