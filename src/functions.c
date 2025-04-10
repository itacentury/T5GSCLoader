#include "functions.h"

opd_s ExecuteKeyboardCallback_t = { 0x56A270, (int32_t)T5_TOC };
void(*ExecuteKeyboardCallback)(int localClientNum) = 
    (void(*)(int))&ExecuteKeyboardCallback_t;

opd_s Dvar_SetFromStringByName_t = { 0x4CF160, (int32_t)T5_TOC };
void (*Dvar_SetFromStringByName)(const char *dvarName, const char *value) =
    (void (*)(const char *, const char *))&Dvar_SetFromStringByName_t;

opd_s Key_SetCatcher_t = { 0x1BE2E0, (int32_t)T5_TOC };
void (*Key_SetCatcher)(int localClientNum, int catcher) =
    (void (*)(int, int))&Key_SetCatcher_t;

opd_s Key_IsCatcherActive_t = { 0x1BF680, (int32_t)T5_TOC };
bool (*Key_IsCatcherActive)(int localClientNum, int mask) =
    (bool (*)(int, int))&Key_IsCatcherActive_t;

opd_s UI_OpenMenu_t = { 0x42F398, (int32_t)T5_TOC };
void (*UI_OpenMenu)(int localClientNum, const char *menuName) =
    (void (*)(int, const char *))&UI_OpenMenu_t;

opd_s va_t = { 0x4DB3D8, (int32_t)T5_TOC };
char * (*va)(const char *format, ...) =
    (char * (*)(const char *, ...))&va_t;

opd_s Dvar_GetBool_t = { 0x4C7BF0, (int32_t)T5_TOC };
bool (*Dvar_GetBool)(const char *) =
    (bool (*)(const char *))&Dvar_GetBool_t;

void setDvar(const char *dvarName, const char *value) {
	Dvar_SetFromStringByName(dvarName, value);
}

void drawOkayPopup(const char *title, const char *message) {
	setDvar((char*)0x90FA58, title); // com_errorTitle
	setDvar((char*)0x90FA40, message); // com_errorMessage
	if (!Key_IsCatcherActive(0, 0x10))
		Key_SetCatcher(0, 0x10);
	UI_OpenMenu(0, "error_popmenu");
}

void displayWelcomePopup(void) {
    drawOkayPopup(va("Welcome %s", GetSelfName()), "to Century Package: Pregame Version");
}
