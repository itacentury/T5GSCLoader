#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "offsets.h"

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    int32_t sub;
    int32_t toc;
} opd_s;

extern opd_s ExecuteKeyboardCallback_t;
extern void(*ExecuteKeyboardCallback)(int localClientNum);

extern opd_s Dvar_SetFromStringByName_t;
extern void (*Dvar_SetFromStringByName)(const char *dvarName, const char *value);

extern opd_s Key_SetCatcher_t;
extern void (*Key_SetCatcher)(int localClientNum, int catcher);

extern opd_s Key_IsCatcherActive_t;
extern bool (*Key_IsCatcherActive)(int localClientNum, int mask);

extern opd_s UI_OpenMenu_t;
extern void (*UI_OpenMenu)(int localClientNum, const char *menuName);

extern opd_s va_t;
extern char *(*va)(const char *format, ...);

extern opd_s Dvar_GetBool_t;
extern bool (*Dvar_GetBool)(const char *);

void setDvar(const char *dvarName, const char *value);
void drawOkayPopup(const char *title, const char *message);
void displayWelcomePopup(void);

#endif /* FUNCTIONS_H */