#ifndef _PARSE_H_
#define _PARSE_H_
#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include <stdio.h>

#include "cvector.h"
#include "lerror.h"

#define DEFAULT_SEPARATORS " \n\t"

#define LERROR_INVALID_DOUBLE_POINTS	0x00000000DU
#define LERROR_INVALID_DOUBLE_SYMBOL	0x00000000EU

#define LERROR_INVALID_DIVIDER_FORMAT	0x00000000AU
#define LERROR_INVALID_SYMBOL			0x00000000CU
#define LERROR_INVALID_FORMAT			0x0000000DDU

#define LERROR_INVALID_PATH				0x0000000AAU
#define LERROR_FILE_OPENNING_FAILED		0x0000000BBU


typedef char* cstr_t;
typedef wchar_t* wstr_t;


typedef enum {
	ENUM_PARSE_SYMBOL_TYPE_NONE = 0,
	ENUM_PARSE_SYMBOL_TYPE_NEWLINES = 1,
	ENUM_PARSE_SYMBOL_TYPE_TABS = 2,
	ENUM_PARSE_SYMBOL_TYPE_SPACES = 4,
	ENUM_PARSE_SYMBOL_TYPE_ALL = ENUM_PARSE_SYMBOL_TYPE_NEWLINES | ENUM_PARSE_SYMBOL_TYPE_TABS | ENUM_PARSE_SYMBOL_TYPE_SPACES
} ENUM_PARSE_FLAGS;


cvector FReadAllToVec(const cstr_t path);

double cstrtod(const cstr_t);

int ParseDoubleEnumToCVec(p_cvector target, const cstr_t _src,
	const cstr_t _in, const cstr_t _out,
	cstr_t _separators, const ENUM_PARSE_FLAGS _flagsToSkip);


#endif // !_PARSE_H_