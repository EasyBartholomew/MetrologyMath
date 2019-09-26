#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <locale.h>
#include <Windows.h>

#include "cvector.h"
#include "parse.h"
#include "calculations.h"


int main() {

	setlocale(LC_CTYPE, ".1251");
	SetConsoleTitle("Metrology");

	cvector input, text;
	cstr_t target = NULL;

	text = FReadAllToVec("in.txt");
	input = CreateCVector(sizeof(double), 0);

	int last = ParseDoubleEnumToCVec(&input, (cstr_t)text.stock, "{", "}", ",", ENUM_PARSE_SYMBOL_TYPE_ALL);

	DestroyCVector(&text);

	if (!last) {
		printf("Local error code: 0x%.8X\n", GetLastLocalERROR());
		_getch();
	}

	printf("N = %u\n", input.size);

	int K_val = calcK(&input);

	printf("K = %d;\n", K_val);
	printf("dX = %d;\n", calcdX(&input, K_val));
	printf("<X> = %lf;\n", calcAvgX(&input));

	double X_val = calcX(&input);

	printf("X = %lf;\n", X_val);
	printf("S^2 = %lf;\n", calcSx(&input, X_val));

	double* p_enum = (double*)input.stock;

	printf("\n{");

	for (register unsigned i = 0; i < input.size - 1; i++) {
		printf(" %.2lf,", p_enum[i]);
	}

	printf(" %.2lf }", p_enum[input.size - 1]);

	free(target);
	DestroyCVector(&input);

	_getch();
	return 0;
}