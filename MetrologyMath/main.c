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


void out(cvector input) {
	printf("Расчёты:\nN = %u\n", input.size);

	int K_val = calcK(&input);

	printf("K = %d;\n", K_val);
	printf("dX = %d;\n", calcdX(&input, K_val));
	printf("<X> = %lf;\n", calcAvgX(&input));

	double X_val = calcX(&input);

	printf("X = %lf;\n", X_val);
	printf("S = %lf;\n", calcSx(&input, X_val));

	double* p_enum = (double*)input.stock;

	printf("\nОтсортированная последовательность:\n{");

	for (register unsigned i = 0; i < input.size - 1; i++) {
		printf(" %.2lf,", p_enum[i]);
	}

	printf(" %.2lf }", p_enum[input.size - 1]);
}


int main() {

	setlocale(LC_CTYPE, ".1251");
	SetConsoleTitle("MetrologyMath");

	cvector input, text;

	text = FReadAllToVec(DEFAULT_INPUT_FILE);
	input = CreateCVector(sizeof(double), 0);

	int last = 0;

	printf("Содержимое файла \'%s\':\n%s\n***************************\n\n",
		DEFAULT_INPUT_FILE, (cstr_t)text.stock);

	while (1) {

		last = ParseDoubleEnumToCVec(&input, (cstr_t)text.stock + last, "{", "}", ",", ENUM_PARSE_SYMBOL_TYPE_ALL);

		if (!last) {
			printf("Local error code: 0x%.8X;\nERRNO = 0x%.8X;\n%s", GetLastLocalERROR(), errno, strerror(errno));
			_getch();
			return -1;
		}

		out(input);
		CVectorClear(&input);

		if (last == -1)
			break;
		else
			printf("\n\n************************************\n");
	}

	DestroyCVector(&text);
	DestroyCVector(&input);

	_getch();
	return 0;
}