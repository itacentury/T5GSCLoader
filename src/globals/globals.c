#include "globals.h"

bool firstStart = true;
char* dvar_cl_ingame = (char*)0x8F07B8;
volatile bool menuOpen = false;
bool showOverlay = true;

bool forceHostEnabled = false;
int partyMinPlayers = 0;
int partyMaxPlayers = 0;

bool GSC_Keyboardbool = false;
scrVmPub_t* scrVmPub = (scrVmPub_t*)0x1AF6334;

gentity_s *g_entities = (gentity_s *)0x12AB290;
SessionData_s *g_serverSession = (SessionData_s *)0x2023500;
