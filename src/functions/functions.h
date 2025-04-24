#ifndef FUNCTIONS_H
#define FUNCTIONS_H

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
extern opd_s NTFY;
extern void(*Scr_Notify)(int ent, short stringValue, unsigned int paramcount);
extern opd_s SV_Cmd_ArgvBuffer_t;
extern void(*SV_Cmd_ArgvBuffer)(int arg, char *buffer, int bufferLength);
extern opd_s Session_IsHost_t;
extern bool(*Session_IsHost)(SessionData_s *session, const int clientNum);
extern opd_s Cmd_MenuResponse_f_t;
extern void(*Cmd_MenuResponse_f)(gentity_s *pEnt);
extern opd_s CG_BoldGameMessage_t;
extern void(*CG_BoldGameMessage)(int localClientNum, const char *msg, int duration);
extern opd_s Scr_GetSelf_t;
extern int(*Scr_GetSelf)(scriptInstance_t inst,int value);
extern opd_s SL_ConvertToString_t;
extern const char*(*SL_ConvertToString)(int stringValue, scriptInstance_t inst);

void Scr_ClearOutParams();
void cBuf_addTextf(const char* format, ...);
void cBuf_addText(char* text);
void setDvar(const char *dvarName, const char *value);
bool IsHost(int clientNum);
bool CompareString(const char *str1, const char *str2);
void drawOkayPopup(const char *title, const char *message);
void displayWelcomePopup(void);
void iPrintlnBold_GameMessage(const char *messageFormat, ...);
void sleep(usecond_t time);
const char *GetSelfName();
const char *GetSelfUserName();
const char *GetSelfOnlineName();

#endif /* FUNCTIONS_H */
