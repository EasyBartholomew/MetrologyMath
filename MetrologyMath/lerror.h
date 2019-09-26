#ifndef _LERROR_H_
#define _LERROR_H_

typedef unsigned int error_t;

#define LERROR_INVALID_PARAM	0x0000000FFU
#define LERROR_INVALID_PTR		0x00000000FU
#define LERROR_OKAY				0x000000000U

error_t GetLastLocalERROR();
void SetLastLocalERROR(error_t);

#endif // !_LERROR_H_