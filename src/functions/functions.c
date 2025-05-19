#include "functions.h"

#include <np.h>
#include <string.h>
#include <sys/timer.h>
#include <sysutil/sysutil_userinfo.h>

#include "globals.h"
#include "printf.h"

opd_s Scr_AddInt_t = {0x5DF2A0, T5_TOC};
void (*Scr_AddInt)(int value,
                   scriptInstance_t inst) = (void (*)(int,
                                                      scriptInstance_t))&Scr_AddInt_t;
opd_s Scr_GetInt_t = {0x5D16C0, T5_TOC};
int (*Scr_GetInt)(unsigned int index, scriptInstance_t scriptInstance) =
    (int (*)(unsigned int, scriptInstance_t))&Scr_GetInt_t;
opd_s Scr_AddString_t = {0x5DE750, T5_TOC};
void (*Scr_AddString)(const char* value, scriptInstance_t inst) =
    (void (*)(const char*, scriptInstance_t))&Scr_AddString_t;
opd_s SL_GetString_t = {0x5B99C8, T5_TOC};
int (*SL_GetString)(const char* str, unsigned int user,
                    scriptInstance_t inst) = (int (*)(const char*, unsigned int,
                                                      scriptInstance_t))&SL_GetString_t;
opd_s Dvar_SetFromStringByName_t = {0x4CF160, (int32_t)T5_TOC};
void (*Dvar_SetFromStringByName)(const char* dvarName, const char* value) =
    (void (*)(const char*, const char*))&Dvar_SetFromStringByName_t;
opd_s Key_SetCatcher_t = {0x1BE2E0, (int32_t)T5_TOC};
void (*Key_SetCatcher)(int localClientNum,
                       int catcher) = (void (*)(int, int))&Key_SetCatcher_t;
opd_s Key_IsCatcherActive_t = {0x1BF680, (int32_t)T5_TOC};
bool (*Key_IsCatcherActive)(int localClientNum,
                            int mask) = (bool (*)(int, int))&Key_IsCatcherActive_t;
opd_s UI_OpenMenu_t = {0x42F398, (int32_t)T5_TOC};
void (*UI_OpenMenu)(int localClientNum,
                    const char* menuName) = (void (*)(int, const char*))&UI_OpenMenu_t;
opd_s va_t = {0x4DB3D8, (int32_t)T5_TOC};
char* (*va)(const char* format, ...) = (char* (*)(const char*, ...))&va_t;
opd_s Dvar_GetBool_t = {0x4C7BF0, (int32_t)T5_TOC};
bool (*Dvar_GetBool)(const char*) = (bool (*)(const char*))&Dvar_GetBool_t;
opd_s Dvar_GetString_t = {0x4C7370, T5_TOC};
char* (*Dvar_GetString)(const char* dvarName) =
    (char* (*)(const char*)) & Dvar_GetString_t;
opd_s cb1 = {0x399CC8, T5_TOC};
void (*Cbuf)(int client, char* cmd) = (void (*)(int, char*))&cb1;
opd_s NTFY = {0x354F08, T5_TOC};
void (*Scr_Notify)(int ent, short stringValue,
                   unsigned int paramcount) = (void (*)(int, short, unsigned int))&NTFY;
opd_s SV_Cmd_ArgvBuffer_t = {0x3997C8, T5_TOC};
void (*SV_Cmd_ArgvBuffer)(int arg, char* buffer,
                          int bufferLength) = (void (*)(int, char*,
                                                        int))&SV_Cmd_ArgvBuffer_t;
opd_s Session_IsHost_t = {0x698490, T5_TOC};
bool (*Session_IsHost)(SessionData_s* session,
                       const int clientNum) = (bool (*)(SessionData_s*,
                                                        const int))&Session_IsHost_t;
opd_s Cmd_MenuResponse_f_t = {0x2D5AE8, T5_TOC};
void (*Cmd_MenuResponse_f)(gentity_s* pEnt) = (void (*)(gentity_s*))&Cmd_MenuResponse_f_t;
opd_s CG_GameMessage_t = {0x113550, T5_TOC};
void (*CG_GameMessage)(int localClientNum,
                       const char* msg) = (void (*)(int, const char*))&CG_GameMessage_t;
opd_s CG_BoldGameMessage_t = {0x113528, T5_TOC};
void (*CG_BoldGameMessage)(int localClientNum, const char* msg,
                           int duration) = (void (*)(int, const char*,
                                                     int))&CG_BoldGameMessage_t;
opd_s Scr_GetSelf_t = {0x5BA4F0, T5_TOC};
int (*Scr_GetSelf)(scriptInstance_t inst,
                   int value) = (int (*)(scriptInstance_t, int))&Scr_GetSelf_t;
opd_s SL_ConvertToString_t = {0x5B6D10, T5_TOC};
const char* (*SL_ConvertToString)(int stringValue, scriptInstance_t inst) =
    (const char* (*)(int, scriptInstance_t))&SL_ConvertToString_t;

void Scr_ClearOutParams() { *(int*)(&scrVmPub->outparamcount) = 0; }

void setDvar(const char* dvarName, const char* value) {
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

void cBuf_addText(char* text) { Cbuf(0, text); }

bool IsHost(int clientNum) { return Session_IsHost(g_serverSession, clientNum); }

bool CompareString(const char* str1, const char* str2) { return !strcmp(str1, str2); }

void drawOkayPopup(const char* title, const char* message) {
    setDvar((char*)0x90FA58, title);    // com_errorTitle
    setDvar((char*)0x90FA40, message);  // com_errorMessage
    if (!Key_IsCatcherActive(0, 0x10)) Key_SetCatcher(0, 0x10);
    UI_OpenMenu(0, "error_popmenu");
}

void displayWelcomePopup(void) {
    drawOkayPopup(va("Welcome %s", GetSelfName()), "to Century Package: Pregame Version");
}

void iPrintln_GameMessage(const char* messageFormat, ...) {
    va_list argptr;
    char text[MAX_STRING_CHARS];
    va_start(argptr, messageFormat);
    vsprintf(text, messageFormat, argptr);
    va_end(argptr);
    CG_GameMessage(0, text);
}

void iPrintlnBold_GameMessage(const char* messageFormat, ...) {
    va_list argptr;
    char text[MAX_STRING_CHARS];
    va_start(argptr, messageFormat);
    vsprintf(text, messageFormat, argptr);
    va_end(argptr);
    CG_BoldGameMessage(0, text, 5000);
}

void sleep(usecond_t time) { sys_timer_usleep(time * 1000); }

const char* GetSelfUserName() {
    CellUserInfoUserStat stat;
    cellUserInfoGetStat(CELL_SYSUTIL_USERID_CURRENT, &stat);
    return stat.name;
}

const char* GetSelfOnlineName() {
    SceNpOnlineName onlineName;
    sceNpManagerGetOnlineName(&onlineName);
    return onlineName.data;
}

const char* GetSelfName() {
    int connectionStatus;
    sceNpManagerGetStatus(&connectionStatus);  // checks if online
    if (connectionStatus == SCE_NP_MANAGER_STATUS_ONLINE)
        return GetSelfOnlineName();
    else
        return GetSelfUserName();
}
