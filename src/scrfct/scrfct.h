#ifndef SCRFCT_H
#define SCRFCT_H

#include <sysutil/sysutil_oskdialog.h>
#include <wchar.h>

#include "functions.h"

#define MAX_TITLE (CELL_OSKDIALOG_STRING_SIZE + 1)
#define MAX_KEYBOARD_THREADS 4

typedef struct {
    wchar_t wTitle[MAX_TITLE];
    int clientNum;
} KeyboardThreadArg;

extern KeyboardThreadArg keyboardArgs[MAX_KEYBOARD_THREADS];
extern int nextKeyboard;

void scrfct_setmemory(void);
void scrfct_callkeyboard(void);
void keyboard_thread(void* arg);

#endif /* SCRFCT_H */