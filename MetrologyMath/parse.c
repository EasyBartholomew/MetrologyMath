#include "parse.h"
#include <locale.h>
#include <ctype.h>

#define NULL_AND_RETURN(target, val) {\
target = NULL;\
return val;\
}


cvector FReadAllToVec(const cstr_t path) {

	cvector stock = CreateCVector(sizeof(char), 0);

	if (!path) {
		SetLastLocalERROR(LERROR_INVALID_PATH);
		return stock;
	}

	FILE* target_file = fopen(path, "r");

	if (!target_file) {
		SetLastLocalERROR(LERROR_FILE_OPENNING_FAILED);
		return stock;
	}

	{
		char temp;

		while (1) {

			temp = fgetc(target_file);

			if (feof(target_file))
				break;

			CVectorPush(&stock, &temp);
		}
	}

	fclose(target_file);

	const char EOS = '\0';
	CVectorPush(&stock, &EOS);

	return stock;
}


double cstrtod(const cstr_t str) {

	if (!str) {
		SetLastLocalERROR(LERROR_INVALID_PTR);
		return 0.0;
	}

	char divider = localeconv()->decimal_point[0];
	size_t points, idx, first;
	signed char sign;

	sign = 1;
	idx = points = 0;

	if (((str[idx] != '-') && (str[idx] != '+')) &&
		((str[idx] < '0') || (str[idx] > '9'))) {

		SetLastLocalERROR(LERROR_INVALID_DOUBLE_SYMBOL);
		return 0.0;
	}

	while ((str[idx] == '-') || (str[idx] == '+')) {

		if (str[idx] == '-') {
			sign *= -1;
		}

		idx++;
	}

	first = idx;

	for (; idx < strlen(str); idx++) {

		if (points > 1) {
			SetLastLocalERROR(LERROR_INVALID_DOUBLE_POINTS);
			return 0.0;
		}

		if (str[idx] == divider)
			points++;
		else if ((str[idx] < '0') || (str[idx] > '9')) {
			SetLastLocalERROR(LERROR_INVALID_DOUBLE_SYMBOL);
			return 0.0;
		}
	}

	return sign * atof(str + first * sizeof(char));
}


inline byte isnum(char c) {
	return (c >= '0') && (c <= '9');
}


int ParseDoubleEnumToCVec(p_cvector target, const cstr_t _src,
	const cstr_t _in, const cstr_t _out,
	cstr_t _separators, char divider, const ENUM_PARSE_FLAGS _flagsToSkip) {

	if (!_separators) {
		_separators = DEFAULT_SEPARATORS;
	}

	if (!target) {
		SetLastLocalERROR(LERROR_INVALID_PTR);
		return 0;
	}

	if (target->elem_size != sizeof(double)) {
		SetLastLocalERROR(LERROR_INVALID_PARAM);
		return 0;
	}

	if (!_src) {
		SetLastLocalERROR(LERROR_INVALID_PTR);
		return 0;
	}

	char s_div = *localeconv()->decimal_point;

	size_t idx, srclen, sepscnt, seen_out;

	seen_out = idx = 0;

	if (strchr(_separators, divider)) {
		SetLastLocalERROR(LERROR_INVALID_PARAM);
		return 0;
	}

	sepscnt = strlen(_separators);
	srclen = strlen(_src);

	if (_in && (_in != "")) {

		size_t in_len = strlen(_in);
		int cmp_result = 1;

		while (idx < srclen) {

			cmp_result = memcmp(_in, _src + idx, in_len);

			if (!cmp_result) {
				idx += in_len;
				goto ___in_end_if___;
			}

			idx++;
		}

		SetLastLocalERROR(LERROR_NO_VALUES);
		return 0;

	___in_end_if___:;
	}


	{
		cvector coms;
		coms = CreateCVector(sizeof(char), 0);
		double got = 0.0;
		size_t outlen;

		if (_out != NULL)
			outlen = strlen(_out);

		const char EOS = '\0';

		SetLastLocalERROR(LERROR_OKAY);

		while (idx < srclen) {

			if (isnum(_src[idx]) || (_src[idx] == divider)) {

				if (coms.size && (!isnum(_src[idx - 1]) && (_src[idx - 1] != divider))) {
					SetLastLocalERROR(LERROR_INVALID_FORMAT);
					CVectorClear(target);
					DestroyCVector(&coms);
					return 0;
				}

				if ((_src[idx] == divider) &&
					!(isnum(_src[idx - 1]) && isnum(_src[idx + 1]))) {
					SetLastLocalERROR(LERROR_INVALID_DOUBLE_POINTS);
					CVectorClear(target);
					DestroyCVector(&coms);
					return 0;
				}


				if (isnum(_src[idx]))
					CVectorPush(&coms, &_src[idx]);
				else
					CVectorPush(&coms, &s_div);
			}
			else if ((_flagsToSkip != ENUM_PARSE_SYMBOL_TYPE_NONE) && (strchr(" \n\t", _src[idx]))) {

				if (_flagsToSkip & ENUM_PARSE_SYMBOL_TYPE_SPACES) {
					while (_src[idx + 1] == ' ')
						idx++;

					if (strchr(_separators, ' ') && coms.size)
						goto ___separators_block___;

				}

				if (_flagsToSkip & ENUM_PARSE_SYMBOL_TYPE_NEWLINES) {
					while (_src[idx + 1] == '\n')
						idx++;

					if (strchr(_separators, '\n') && coms.size)
						goto ___separators_block___;
				}

				if (_flagsToSkip & ENUM_PARSE_SYMBOL_TYPE_TABS) {
					while (_src[idx + 1] == '\t')
						idx++;

					if (strchr(_separators, '\t') && coms.size)
						goto ___separators_block___;
				}
			}
			else if (strchr(_separators, _src[idx])) {

			___separators_block___:;

				if (coms.size) {

					CVectorPush(&coms, &EOS);
					got = cstrtod((cstr_t)coms.stock);

					if ((got < 0.00001) && GetLastLocalERROR()) {

						CVectorClear(target);
						DestroyCVector(&coms);
						return 0;
					}

					CVectorClear(&coms);
					CVectorPush(target, &got);
					got = 0.0;
				}
			}
			else if (iscntrl(_src[idx])) {
				;
			}
			else if ((_out != NULL) && (!memcmp(_src + idx, _out, outlen))) {

				if (coms.size) {
					CVectorPush(&coms, &EOS);
					got = cstrtod((cstr_t)coms.stock);

					if ((got < 0.00001) && GetLastLocalERROR()) {

						CVectorClear(target);
						DestroyCVector(&coms);
						return 0;
					}

					CVectorClear(&coms);
					CVectorPush(target, &got);
					got = 0.0;
				}

				seen_out |= 1;

				idx += outlen;
				break;
			}
			else {
				SetLastLocalERROR(LERROR_INVALID_SYMBOL);
				CVectorClear(target);
				DestroyCVector(&coms);
				return 0;
			}

			idx++;
		}

		if (coms.size) {
			CVectorPush(&coms, &EOS);
			got = cstrtod((cstr_t)coms.stock);

			if ((got < 0.00001) && GetLastLocalERROR()) {

				CVectorClear(target);
				DestroyCVector(&coms);
				return 0;
			}

			CVectorPush(target, &got);
			got = 0.0;
		}

		DestroyCVector(&coms);
	}

	if (_out && (_out != "") && !seen_out) {
		CVectorClear(target);
		SetLastLocalERROR(LERROR_INVALID_FORMAT);
		return 0;
	}

	if (_src[idx] == '\0') {
		idx = -1;
	}

	return idx;
}


int parseDP2CVec(p_cvector target, const cstr_t _src,
	const cstr_t _in, const cstr_t _out,
	char _separator, char divider, const ENUM_PARSE_FLAGS _flagsToSkip) {

	if (target->elem_size != sizeof(dp_t)) {
		SetLastLocalERROR(LERROR_INVALID_PARAM);
		return 0;
	}

	dp_t kv_current = { 0 };
	char seps[2] = { 0 };
	int last = 0;
	int l_temp = 0;

	cvector kv_vector = CreateCVector(sizeof(double), DEFAULT_INITIAL_CAPACITY_VALUE);
	seps[0] = _separator;

	while (l_temp != -1) {
		l_temp = ParseDoubleEnumToCVec(&kv_vector, _src + last, _in, _out, seps, divider, _flagsToSkip);
		last += l_temp;

		if (kv_vector.size != 2) {
			CVectorClear(target);
			DestroyCVector(&kv_vector);

			SetLastLocalERROR(LERROR_INVALID_FORMAT);
			return 0;
		}

		kv_current.key = ((double*)kv_vector.stock)[0];
		kv_current.value = ((double*)kv_vector.stock)[1];

		CVectorClear(&kv_vector);
		CVectorPush(target, &kv_current);
	}

	DestroyCVector(&kv_vector);
	return 1;
}