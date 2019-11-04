#ifndef _CONINFO_H_
#define _CONINFO_H_
#include "parse.h"


#define DEFAULT_WNDXY 0x0

typedef unsigned char bool_t;

typedef enum {
	SSCI_MOVE = 0x1,
	SSCI_RESIZE = 0x2,
	SSCI_TITLE = 0x4,
	SSCI_ENC = 0x8,
	SSCI_BUFF = 0x10,
	SSCI_ALL = SSCI_MOVE | SSCI_RESIZE | SSCI_TITLE | SSCI_ENC | SSCI_BUFF
} SSCI_FLAGS;

typedef struct {

	int wnd_x;
	int wnd_y;
	size_t wnd_width;
	size_t wnd_height;
	size_t buff_x;
	size_t buff_y;
	cstr_t title;
	cstr_t enc;
	int enc_category;
} coninfo_t, * pconinfo_t;


bool_t SetStdConinfo(pconinfo_t, SSCI_FLAGS flags);

#endif // !_CONINFO_H_