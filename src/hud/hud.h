#ifndef HUD_H
#define HUD_H

#include <stdint.h>

#include "functions.h"

extern float ColorVividTeal[4];
extern float ColorMutedTeal[4];
extern float ColorWhite[4];
extern float ColorBackground[4];
extern float ColorBlack[4];
extern float ColorMenuTitle[4];

extern opd_s R_AddCmdDrawStretchPic_t;
extern opd_s Material_RegisterHandle_t;
extern opd_s R_AddCmdDrawText_t;
extern opd_s R_RegisterFont_t;

extern void (*R_AddCmdDrawStretchPic)(float, float, float, float, float, float, float,
                                      float, float*, int);
extern int (*Material_RegisterHandle)(char*, int);
extern void (*R_AddCmdDrawText)(const char*, int, int, float, float, float, float, float,
                                float*, int);
extern int (*R_RegisterFont)(char*, int);

#endif /* HUD_H */
