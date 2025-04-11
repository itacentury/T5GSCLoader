#ifndef GLOBALS_H
#define GLOBALS_H

#include "defines.h"

#include <stdbool.h>

#define MAX_STRING_CHARS 1024
#define SCREEN_CENTER_Y 640
#define SCREEN_CENTER_X 360

extern int firstStart;
extern char* dvar_cl_ingame;
extern bool menuOpen;

extern bool GSC_Keyboardbool;
extern int HostNum;
extern scrVmPub_t *scrVmPub;

extern char GSC_Keyboardbuffer[MAX_STRING_CHARS];

typedef enum Addies
{
    LEVEL_LOCALS_T = 0x1213800,
    LEVEL_TIME = LEVEL_LOCALS_T + 0x74C,
    PRECACHE_SHADER = 0x1213800 + 0x3C,
    DVAR_GETBOOL = 0x4C7BF0,
    SV_GAMESENDSERVERCOMMAND = 0x3E95F0,
} Addies;

#endif /* GLOBALS_H */
