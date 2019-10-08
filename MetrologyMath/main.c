#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <locale.h>
#include <Windows.h>

#include "cvector.h"
#include "parse.h"
#include "calculations.h"


#define DEFAULT_INPUT_FILE "in.txt"


int main() {

	setlocale(LC_CTYPE, ".1251");
	SetConsoleTitle("MetrologyMath");

	cvector input, text;

	text = FReadAllToVec(DEFAULT_INPUT_FILE);
	input = CreateCVector(sizeof(double), 0);

	int last = ParseDoubleEnumToCVec(&input, (cstr_t)text.stock, "{", "}", ",", ENUM_PARSE_SYMBOL_TYPE_ALL);

	printf("Содержимое файла \'%s\':\n%s\n***************************\n\n",
		DEFAULT_INPUT_FILE, (cstr_t)text.stock);
	DestroyCVector(&text);

	if (!last) {
		printf("Local error code: 0x%.8X;\nERRNO = 0x%.8X;\n%s", GetLastLocalERROR(), errno, strerror(errno));
		_getch();
	}

	printf("Расчёты:\nN = %u\n", input.size);

	int K_val = calcK(&input);

	printf("K = %d;\n", K_val);
	printf("dX = %d;\n", calcdX(&input, K_val));
	printf("<X> = %lf;\n", calcAvgX(&input));

	double X_val = calcX(&input);

	printf("X = %lf;\n", X_val);
	printf("S = %lf;\n", sqrt(calcSx(&input, X_val)));

	double* p_enum = (double*)input.stock;

	printf("\nОтсортированная последовательность:\n{");

	for (register unsigned i = 0; i < input.size - 1; i++) {
		printf(" %.2lf,", p_enum[i]);
	}

	printf(" %.2lf }", p_enum[input.size - 1]);

	DestroyCVector(&input);

	_getch();
	return 0;
}