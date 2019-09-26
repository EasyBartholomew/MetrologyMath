#include "lerror.h"


error_t last_error_code = 0;


error_t GetLastLocalERROR() {
	return last_error_code;
}


void SetLastLocalERROR(error_t error) {
	last_error_code = error;
}