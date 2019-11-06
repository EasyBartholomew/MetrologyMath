#include "coninfo.h"
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <locale.h>


bool_t SetStdConinfo(pconinfo_t info, SSCI_FLAGS flags) {

	errno_t e = 0;

	if (!info) {
		SetLastLocalERROR(LERROR_INVALID_PTR);
		return e;
	}

	if (flags & SSCI_TITLE) {
		e = SetConsoleTitle(info->title);

		if (!e)
			return e;
	}

	if (flags & SSCI_ENC) {
		setlocale(info->enc_category, info->enc);
	}

	HANDLE hStdOut;
	HWND hConsole;

	if (flags & SSCI_BUFF) {
		hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

		COORD coord;


		coord.X = info->buff_x;
		coord.Y = info->buff_y;

		e = SetConsoleScreenBufferSize(hStdOut, coord);

		if (!e)
			return e;
	}

	hConsole = GetConsoleWindow();

	if (flags & SSCI_MOVE) {
		e = SetWindowPos(hConsole, NULL, info->wnd_x, info->wnd_y,
			0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);

		if (!e)
			return e;
	}

	if (flags & SSCI_RESIZE) {
		e = SetWindowPos(hConsole, NULL, 0, 0,
			info->wnd_width, info->wnd_height,
			SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);

		if (!e)
			return e;
	}

	return 1;
}