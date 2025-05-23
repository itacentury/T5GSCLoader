#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdbool.h>

#include "defines.h"

#define MAX_STRING_CHARS 1024

typedef struct Coordinates2D {
    float width;
    float height;
} Coordinates2D;

typedef enum Addies {
    LEVEL_LOCALS_T = 0x1213800,
    LEVEL_TIME = LEVEL_LOCALS_T + 0x74C,
    PRECACHE_SHADER = 0x1213800 + 0x3C,
    DVAR_GETBOOL = 0x4C7BF0,
    SV_GAMESENDSERVERCOMMAND = 0x3E95F0,
} Addies;

extern Coordinates2D screenResolution;
extern Coordinates2D screenCenter;
extern float hudScale;

extern bool firstStart;
extern char* dvar_cl_ingame;
extern bool menuOpen;
extern bool showOverlay;

extern bool forceHostEnabled;
extern int partyMinPlayers;
extern int partyMaxPlayers;

extern bool GSC_Keyboardbool;
extern scrVmPub_t* scrVmPub;

extern gentity_s* g_entities;
extern SessionData_s* g_serverSession;

#endif /* GLOBALS_H */
