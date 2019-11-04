#ifndef _OUTPUT_H_
#define _OUTPUT_H_
#define _CRT_SECURE_NO_WARNINGS


#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#include "cvector.h"


void report(char const* const _msg);
void PrintCalculations2Stream(FILE* stream, p_cvector input, p_cvector lf_table, size_t num);

#endif // !_OUTPUT_H_