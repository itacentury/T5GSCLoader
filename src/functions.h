#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "PS3.h"
#include "offsets.h"
#include "defines.h"

#include <wchar.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    int32_t sub;
    int32_t toc;
} opd_s;

extern opd_s Scr_AddInt_t;
extern void(*Scr_AddInt)(int value, scriptInstance_t inst);
extern opd_s Scr_GetInt_t;
extern int(*Scr_GetInt)(unsigned int index, scriptInstance_t scriptInstance);
extern opd_s Scr_AddString_t;
extern void(*Scr_AddString)(const char* value, scriptInstance_t inst);
extern opd_s SL_GetString_t;
extern int(*SL_GetString)(const char* str, unsigned int user, scriptInstance_t inst);
extern opd_s ExecuteKeyboardCallback_t;
extern void(*ExecuteKeyboardCallback)(int localClientNum);
extern opd_s UI_DrawKeyboard_t;
extern void(*UI_DrawKeyboard)(int localClientNum, const wchar_t *title, const wchar_t *presetMessage, size_t size, UI_KeyboardCallback function);
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
extern opd_s cb1;
extern void(*Cbuf)(int client, char* cmd);

void Scr_ClearOutParams();
void cBuf_addText(char* text);
void setDvar(const char *dvarName, const char *value);
void drawOkayPopup(const char *title, const char *message);
void displayWelcomePopup(void);
void DrawKeyboard(char *title, const char *presetMessage, size_t size, uint32_t panelMode, UI_KeyboardCallback function);

#endif /* FUNCTIONS_H */