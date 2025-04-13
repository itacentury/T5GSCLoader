#include "hud.h"
#include "utils.h"
#include "globals.h"
#include "offsets.h"
#include "functions.h"

float ColorVividTeal[4] = {0.0f, 1.0f, 1.0f, 1.0f};
float ColorMutedTeal[4] = {0.0f, 0.7f, 0.7f, 1.0f};
float ColorWhite[4] = {1.0f, 1.0f, 1.0f, 1.0f};
float ColorBackground[4] = {0.0f, 0.0f, 0.0f, 0.7f};
float ColorBlack[4] = {0.0f, 0.0f, 0.0f, 1.0f};
float ColorMenuTitle[4] = {0.9882f, 0.6667f, 0.4549f, 1.0f};

opd_s R_AddCmdDrawStretchPic_t = { 0x751940, T5_TOC };
opd_s Material_RegisterHandle_t = { 0x7458D8, T5_TOC };
opd_s R_AddCmdDrawText_t = { 0x754410, T5_TOC };
opd_s R_RegisterFont_t = { 0x734350, T5_TOC };

void(*R_AddCmdDrawStretchPic)(float, float, float, float, float, float, float, float, float *, int) = 
    (void(*)(float, float, float, float, float, float, float, float, float *, int))&R_AddCmdDrawStretchPic_t;
int(*Material_RegisterHandle)(char *, int) = 
    (int(*)(char *, int))&Material_RegisterHandle_t;
void(*R_AddCmdDrawText)(const char *, int, int, float, float, float, float, float, float *, int) = 
    (void(*)(char *text, int maxChars, int font, float x, float y, float xScale, float yScale, float rotation, float *color, int style))&R_AddCmdDrawText_t;
int(*R_RegisterFont)(char *, int) = 
    (int(*)(char *name, int imageTrack))&R_RegisterFont_t;
