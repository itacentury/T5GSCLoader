#include "printf.h"
#include "functions.h"
#include "globals.h"

opd_s Scr_AddInt_t = { 0x5DF2A0, T5_TOC };
void(*Scr_AddInt)(int value, scriptInstance_t inst) = 
    (void(*)(int, scriptInstance_t))&Scr_AddInt_t;
opd_s Scr_GetInt_t = { 0x5D16C0, T5_TOC };
int(*Scr_GetInt)(unsigned int index, scriptInstance_t scriptInstance) = 
    (int(*)(unsigned int, scriptInstance_t))&Scr_GetInt_t;
opd_s Scr_AddString_t = { 0x5DE750, T5_TOC };
void(*Scr_AddString)(const char* value, scriptInstance_t inst) = 
    (void(*)(const char*, scriptInstance_t))&Scr_AddString_t;
opd_s SL_GetString_t = { 0x5B99C8, T5_TOC };
int(*SL_GetString)(const char* str, unsigned int user, scriptInstance_t inst) = 
    (int(*)(const char*, unsigned int, scriptInstance_t))&SL_GetString_t;
opd_s ExecuteKeyboardCallback_t = { 0x56A270, (int32_t)T5_TOC };
void(*ExecuteKeyboardCallback)(int localClientNum) = 
    (void(*)(int))&ExecuteKeyboardCallback_t;
opd_s UI_DrawKeyboard_t = { 0x56A118, T5_TOC };
void(*UI_DrawKeyboard)(int localClientNum, const wchar_t *title, const wchar_t *presetMessage, size_t size, UI_KeyboardCallback function) = 
    (void(*)(int, const wchar_t *, const wchar_t *, size_t, UI_KeyboardCallback))&UI_DrawKeyboard_t;
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
opd_s cb1 = { 0x399CC8, T5_TOC };
void(*Cbuf)(int client, char* cmd) = (void(*)(int, char*))&cb1;
opd_s NTFY = { 0x354F08, T5_TOC };
void(*scr_Notify)(int ent, short stringValue, unsigned int paramcount) = 
    (void(*)(int, short, unsigned int))&NTFY;

void Scr_ClearOutParams() {
    *(int*)(&scrVmPub->outparamcount) = 0;
}

void setDvar(const char *dvarName, const char *value) {
	Dvar_SetFromStringByName(dvarName, value);
}

void cBuf_addTextf(const char* format, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    Cbuf(0, buffer);
}

void cBuf_addText(char* text) {
    Cbuf(0, text);
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

void DrawKeyboard(char *title, const char *presetMessage, size_t size, uint32_t panelMode, UI_KeyboardCallback function) {
	*(bool*)0x1B29434 = true;
	WriteProcessMemory(0x56A122, HIWORD(panelMode), sizeof(uint16_t));
	wchar_t titleBuffer[MAX_STRING_CHARS], presetMessageBuffer[MAX_STRING_CHARS];
	StringToWideCharacter(titleBuffer, title, MAX_STRING_CHARS);
	StringToWideCharacter(presetMessageBuffer, presetMessage, MAX_STRING_CHARS);
	UI_DrawKeyboard(0, titleBuffer, presetMessageBuffer, size, function);
	WriteProcessMemory(0x56A122, HIWORD(CELL_OSKDIALOG_PANELMODE_ALPHABET), sizeof(uint16_t));
}
