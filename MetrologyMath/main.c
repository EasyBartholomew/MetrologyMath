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

	cvector input;
	cstr_t target = NULL;

	input = CreateCVector(sizeof(double), 0);

	FReadAllTo(&target, "in.txt");

	error_t err = ParseDoubleEnumToCVec(&input, target, NULL, "{", "}", ",", ENUM_PARSE_SYMBOL_TYPE_ALL);

	if (err) {
		printf("Local error code: 0x%X\n", err);
		_getch();
	}

	printf("N = %u\n", input.size);

	int K_val = K(&input);

	printf("K = %d;\n", K_val);
	printf("dX = %d;\n", dX(&input, K_val));
	printf("<X> = %lf;\n", AvgX(&input));

	double X_val = X(&input);

	printf("X = %lf;\n", X_val);
	printf("S^2 = %lf;\n", Sx(&input, X_val));

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