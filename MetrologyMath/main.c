#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <locale.h>

#include "parse.h"
#include "cvector.h"
#include "output.h"
#include "coninfo.h"


#define DEFAULT_INPUT_FILE "in.txt"
#define DEFAULT_LAPLACE_TABLE_FILE "lpt.tbl"


#define BUFFER_X 193
#define BUFFER_Y 1000


static bool_t init_window() {

	coninfo_t info;

	info.buff_x = BUFFER_X;
	info.buff_y = BUFFER_Y;

	info.enc = ".1251";
	info.enc_category = LC_CTYPE;
	info.title = "MetrologyMath";

	info.wnd_height = 600;
	info.wnd_width = 1580;

	info.wnd_x = 0;
	info.wnd_y = 0;

	return SetStdConinfo(&info, SSCI_BUFF | SSCI_ENC | SSCI_TITLE | SSCI_RESIZE);
}


int main() {

	if (!init_window()) {
		report("Initialization of window failed!");

		_getch();
		return -1;
	}

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

		DestroyCVector(&text);
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

		PrintCalculations2Stream(stdout, &input, &lf_table, cnt);

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