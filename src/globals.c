#include "globals.h"

bool firstStart = true;
char* dvar_cl_ingame = (char*)0x8F07B8;
bool menuOpen = false;
bool showOverlay = true;

bool forceHostEnabled = false;
int partyMinPlayers = 0;
int partyMaxPlayers = 0;

bool GSC_Keyboardbool = false;
scrVmPub_t* scrVmPub = (scrVmPub_t*)0x1AF6334;

char GSC_Keyboardbuffer[MAX_STRING_CHARS] = {0};
