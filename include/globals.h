#ifndef GLOBALS_H
#define GLOBALS_H

#include "defines.h"

#include <stdbool.h>

#define MAX_STRING_CHARS 1024

extern int firstStart;
extern char* dvar_cl_ingame;

extern bool GSC_Keyboardbool;
extern int HostNum;
extern scrVmPub_t *scrVmPub;

extern char GSC_Keyboardbuffer[MAX_STRING_CHARS];

#endif /* GLOBALS_H */
