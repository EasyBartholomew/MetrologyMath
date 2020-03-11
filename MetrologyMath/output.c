#include "output.h"
#include "calculations.h"
#include <Windows.h>

void report(char const* const _msg) {
	printf("%s\nLocal ERROR = %u;\nLast ERROR = %d\n", _msg, GetLastLocalERROR(), GetLastError());
	printf("%s", strerror(GetLastError()));
}


void PrintCalculations2Stream(FILE* where, p_cvector input, p_cvector lf_table, size_t num) {
	fprintf(where, "\nРасчёты #%u:\nN = %u;\n\n", num, input->size);

	sortCVec(input); //Sorting of input

	fprintf(where, "Отсортированная последовательность:\n{ ");
	for (register short i = 0; i < input->size - 1; i++) {
		fprintf(where, "%.3lf; ", *(double*)CVectorEnumerate(input, i));
	}
	fprintf(where, "%.3lf }\n\n", *(double*)CVectorEnumerate(input, input->size - 1));

	int k_val = calcK(input);
	double dx_val = calcdX(input, k_val); //(max - min) / K
	double avgx_val = calcAvgX(input); //(min + max) / 2
	double x_val = calcX(input); // (sum) / n
	double sigm_val = calcSigm(input, x_val); //выборочное среднее
	double sx_val = calcSx(sigm_val, input->size);

	fprintf(where, "K = %d;\ndx = %.3lf;\t//(max - min) / K\n<X> = %.3lf;\t//(min + max) / 2\n", k_val, dx_val, avgx_val);
	fprintf(where, "x = %.3lf;\t//(sum) / n, выборочное среднее\nSigm = %.3lf;\t//Среднеквадратическое отклонение\nSx = %.3lf;\n",
		x_val, sigm_val, sx_val);

	cvector xi, xi_prev, me, pem, pi, t, t_prev, fl, fl_prev, mt, sx;

	xi = CreateCVector(sizeof(double), DEFAULT_INITIAL_CAPACITY_VALUE);
	xi_prev = CreateCVector(sizeof(double), DEFAULT_INITIAL_CAPACITY_VALUE);


	calcRngsSt(&xi, &xi_prev, input, k_val, dx_val);
	me = calcM(input, k_val, dx_val);
	pem = calcPEm(&me, input->size, dx_val);

	t = calct(&xi, x_val, sigm_val);
	t_prev = calct(&xi_prev, x_val, sigm_val);

	fl = calcflaplace(&t, lf_table);
	fl_prev = calcflaplace(&t_prev, lf_table);

	pi = calcPi(lf_table, &fl, &fl_prev);
	mt = calcMT(&pi, input->size);

	sx = calcKsy(&me, &mt);

	fprintf(where, "\nx[i]\t\tx[i-1]\t\tMe\t\tPem\t\t\tPi\t\t\tt[i]\t\tt[i-1]\t\tLAP[i]\t\tLAP[i-1]\t\tMt");
	fprintf(where, "\t\t\t\tX^2\n");

	for (register short i = 0; i < xi.size; i++) {
		fprintf(where, "%.3lf\t\t%.3lf\t\t%u\t\t%.3lf\t\t%.3lf\t\t%.3lf\t\t%.3lf\t\t%.3lf\t\t%.3lf\t\t\t%.4lf",
			round(*(double*)CVectorEnumerate(&xi, i) * 1000) / 1000,
			round(*(double*)CVectorEnumerate(&xi_prev, i) * 1000) / 1000,
			*(size_t*)CVectorEnumerate(&me, i),
			round(*(double*)CVectorEnumerate(&pem, i) * 1000) / 1000,
			round(*(double*)CVectorEnumerate(&pi, i) * 1000) / 1000,
			round(*(double*)CVectorEnumerate(&t, i) * 1000) / 1000,
			round(*(double*)CVectorEnumerate(&t_prev, i) * 1000) / 1000,
			round(*(double*)CVectorEnumerate(&fl, i) * 1000) / 1000,
			round(*(double*)CVectorEnumerate(&fl_prev, i) * 1000) / 1000,
			round(*(double*)CVectorEnumerate(&mt, i) * 10000) / 10000);

		fprintf(where, "\t\t%.8lf\n", *(double*)CVectorEnumerate(&sx, i));
	}

	DestroyCVector(&xi);
	DestroyCVector(&xi_prev);
	DestroyCVector(&me);
	DestroyCVector(&pem);
	DestroyCVector(&pi);
	DestroyCVector(&t);
	DestroyCVector(&t_prev);
	DestroyCVector(&fl);
	DestroyCVector(&fl_prev);
	DestroyCVector(&mt);

	//xi, xi_prev, me, pem, pi, t, t_prev, fl, fl_prev, mt, sx;

	double sum_sx = 0.0;

	for (register short i = 0; i < sx.size; i++) {
		sum_sx += *(double*)CVectorEnumerate(&sx, i);
	}

	DestroyCVector(&sx);

	fprintf(where, "\n\nSum(X^2) = %.8lf;\n", sum_sx);
}