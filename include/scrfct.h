#ifndef SCRFCT_H
#define SCRFCT_H

#include <wchar.h>

#include "functions.h"

void scrfct_setmemory(void);
void scrfct_ps3_keyboard();
void GSC_KeyboardCallback(int localClientNum, const wchar_t *text, size_t size);

#endif /* SCRFCT_H */