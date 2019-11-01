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
#define DEFAULT_LAPLACE_TABLE_FILE "lpt.tbl"



void out(FILE* where, p_cvector input, p_cvector lf_table, size_t num) {
	fprintf(where, "\nРасчёты #%u:\nN = %u;\n\n", num, input->size);

	sortCVec(input); //Sorting of input

	fprintf(where, "Отсортированная последовательность:\n{ ");
	for (register short i = 0; i < input->size - 1; i++) {
		fprintf(where, "%.3lf, ", *(double*)CVectorEnumerate(input, i));
	}
	fprintf(where, "%.3lf }\n\n", *(double*)CVectorEnumerate(input, input->size - 1));

	int k_val = calcK(input);
	double dx_val = calcdX(input, k_val); //(max - min) / K
	double avgx_val = calcAvgX(input); //(min + max) / 2
	double x_val = calcX(input); // (sum) / n
	double sigm_val = calcSigm(input, x_val); //выборочное среднее
	double sx_val = calcSx(sigm_val, input->size);

	fprintf(where, "K = %d;\ndx = %.3lf;\t//(max - min) / K\n<X> = %.3lf;\t//(min + max) / 2\n", k_val, dx_val, avgx_val);
	fprintf(where, "x = %.3lf;\t(sum) / n\nSigm = %.3lf;\t//выборочное среднее\nSx = %.3lf;\n", x_val, sigm_val, sx_val);

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

	fprintf(where, "\nx[i]\t\tx[i-1]\t\tMe\t\tPem\t\tPi\t\tt[i]\t\tt[i-1]\t\tLAP[i]\t           LAP[i-1]           Pi              Mt");
	fprintf(where, "              X^2\n");

	for (register short i = 0; i < xi.size; i++) {
		fprintf(where, "%.3lf\t\t%.3lf\t\t%u\t\t%.3lf\t\t%.3lf\t\t%.3lf\t\t%.3lf\t\t%.3lf\t           %.3lf             %.4lf          %.4lf",
			round(*(double*)CVectorEnumerate(&xi, i) * 1000) / 1000,
			round(*(double*)CVectorEnumerate(&xi_prev, i) * 1000) / 1000,
			*(size_t*)CVectorEnumerate(&me, i),
			round(*(double*)CVectorEnumerate(&pem, i) * 1000) / 1000,
			round(*(double*)CVectorEnumerate(&pi, i) * 1000) / 1000,
			round(*(double*)CVectorEnumerate(&t, i) * 1000) / 1000,
			round(*(double*)CVectorEnumerate(&t_prev, i) * 1000) / 1000,
			round(*(double*)CVectorEnumerate(&fl, i) * 1000) / 1000,
			round(*(double*)CVectorEnumerate(&fl_prev, i) * 1000) / 1000,
			round(*(double*)CVectorEnumerate(&pi, i) * 10000) / 10000,
			round(*(double*)CVectorEnumerate(&mt, i) * 10000) / 10000);

		fprintf(where, "          %.8lf\n", *(double*)CVectorEnumerate(&sx, i));
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


void check() {
	cvector input, text;

	text = FReadAllToVec(DEFAULT_LAPLACE_TABLE_FILE);

	input = CreateCVector(sizeof(dp_t), 0);

	int sc = parseDP2CVec(&input, (cstr_t)text.stock, "<", ">", '~', ENUM_PARSE_SYMBOL_TYPE_SPACES);

	pdp_t table = (pdp_t)input.stock;

	for (int i = 0; i < input.size; i++) {
		printf("%d) %0.2lf ~ %0.7lf;\n", i + 1, table[i].key, table[i].value);
	}

	_getch();
}


void report(char const* const _msg) {
	printf("%s\nLocal ERROR = %u;\nLast ERROR = %d\n", _msg, GetLastLocalERROR(), GetLastError());
	printf("%s", strerror(GetLastError()));
}

#define BUFFER_X 193
#define BUFFER_Y 35


int main() {

	setlocale(LC_CTYPE, ".1251");
	SetConsoleTitle("MetrologyMath");

	HANDLE hConsole;
	COORD dwSize;
	SMALL_RECT rect = { 0 };

	dwSize.X = BUFFER_X + 1;
	dwSize.Y = BUFFER_Y + 1;
	rect.Bottom = BUFFER_Y;
	rect.Right = BUFFER_X;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleScreenBufferSize(hConsole, dwSize);
	SetConsoleWindowInfo(hConsole, TRUE, &rect);

	//check();

	cvector input, text, lf_table;

	text = FReadAllToVec(DEFAULT_LAPLACE_TABLE_FILE);

	if (GetLastLocalERROR()) {
		report("Не найден файл таблицы!");

		DestroyCVector(&text);
		_getch();
		return -1;
	}

	lf_table = CreateCVector(sizeof(dp_t), DEFAULT_INITIAL_CAPACITY_VALUE);

	int pdparser = parseDP2CVec(&lf_table, (cstr_t)text.stock, "<", ">", '~', ENUM_PARSE_SYMBOL_TYPE_SPACES);

	DestroyCVector(&text);

	if (!pdparser) {
		report("Файл таблицы повреждён!");

		DestroyCVector(&lf_table);
		_getch();
		return -1;
	}

	text = FReadAllToVec(DEFAULT_INPUT_FILE);
	input = CreateCVector(sizeof(double), 0);

	int last, cnt;

	last = cnt = 0;

	if (GetLastLocalERROR() == LERROR_FILE_OPENNING_FAILED) {
		printf("Файл с именем \"in.txt\" не существует!");
		_getch();
		return -1;
	}

	printf("Содержимое файла \'%s\':\n%s\n************************************\n\n",
		DEFAULT_INPUT_FILE, (cstr_t)text.stock);

	while (1) {

		last = ParseDoubleEnumToCVec(&input, (cstr_t)text.stock + last, "{", "}", ",", ENUM_PARSE_SYMBOL_TYPE_ALL);

		if (!(last || cnt)) {
			printf("Local error code: 0x%.8X;\nERRNO = 0x%.8X;\n%s", GetLastLocalERROR(), errno, strerror(errno));
			_getch();
			return -1;
		}
		else if (!last) {
			break;
		}

		cnt++;

		out(stdout, &input, &lf_table, cnt);

		CVectorClear(&input);

		if (last == -1)
			break;
		else
			printf("\n\n************************************\n");
	}

	DestroyCVector(&text);
	DestroyCVector(&input);
	DestroyCVector(&lf_table);

	_getch();
	return 0;
}