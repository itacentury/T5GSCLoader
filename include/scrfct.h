#ifndef SCRFCT_H
#define SCRFCT_H

#include "functions.h"

#include <wchar.h>

#include <sysutil/sysutil_oskdialog.h>


#define MAX_TITLE (CELL_OSKDIALOG_STRING_SIZE+1)
#define MAX_KB_THREADS 4

typedef struct {
    wchar_t wtitle[MAX_TITLE];
    int clientNum;
} KeyboardThreadArg;

extern KeyboardThreadArg gKbArgs[MAX_KB_THREADS];
extern int gNextKb;

void scrfct_setmemory(void);
void scrfct_callkeyboard(void);
void keyboard_thread(void *arg);

#endif /* SCRFCT_H */