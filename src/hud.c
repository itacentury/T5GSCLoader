#include "hud.h"
#include "utils.h"
#include "globals.h"
#include "offsets.h"
#include "functions.h"

int HudMenuIDs_NewsBar[12];
int HudMenuIDs_NewsbarUpLine[12];
int HudMenuIDs_NewsbarBG[12];
int HudMenuIDs_NewsbarUnLine[12];
int HudMenuIDs_Scrollbar[12];
int HudMenuIDs_Shader[12];
int HudMenuIDs_Sidebar[12];
int HudMenuIDs_Sidebar2[12];
int HudMenuIDs_MenuTitle[12];
int HudMenuIDs_OptionsText[12];
int HudMenuIDs_MenuName[12];
int HudMenuIDs_Creator[12];
char* HudMenuIDs_MenuOptionText[12];
char* HudMenuIDs_MenuTitleText[12];
char* HudMenuIDs_CreatorText[12];
char* HudMenuIDs_NewsBarText[12];
int HudMenuIDs_PlayerVerification[12];

int hud_G_LocalizedStringIndex(char* txt) {
    opd_s GLS = { 0x00370638, T5_TOC };
    int (*GLSI)(char* txt) = (int (*)(char*)) &GLS;
    return GLSI(txt);
}

int hud_GetLevelTime(void) {
    return *(int*)(LEVEL_TIME);
}

int hud_GetLevelTime1(void) {
    return *(int*)0x011E7889;
}

int hud_HudElem_Alloc(void) {
    int client, i;
    for (client = 0; client < 12; client++) {
        for (i = 150; i < 1024; i++) {
            unsigned int address = 0x11F26D4 + (client * 0x2A38) + (i * 0x7C);
            char* Buffer = ReadBytes(address, 4);
            if (Buffer && Buffer[0] == 0x00) {
                *(int*)(address + HUD_TYPE) = 0x7C;
                return address;
            }
        }
    }
    return 1023;
}

int hud_SetShader(int client, char Material, short Width, short Height, float x, float y, int r, int g, int b, int a) {
    int Element = hud_HudElem_Alloc();
    *(char*)(Element + HUD_TYPE) = 8;
    *(short*)(Element + HUD_HEIGHT) = Height;
    *(short*)(Element + HUD_WIDTH) = Width;
    *(float*)(Element + HUD_X) = x;
    *(float*)(Element + HUD_Y) = y;
    *(float*)(Element + HUD_SORT) = 0;
    *(int*)(Element + HUD_TARGET_ENT_NUM) = client;
    *(char*)(Element + HUD_MATERIAL_INDEX) = Material;
    *(char*)(Element + HUD_COLOR) = r;
    *(char*)(Element + HUD_COLOR + 0x01) = g;
    *(char*)(Element + HUD_COLOR + 0x02) = b;
    *(char*)(Element + HUD_COLOR + 0x03) = a;
    *(char*)(Element + HUD_UI3D_WINDOW) = 0xFF;
    return Element;
}

int hud_SetText(int client, char* text, float x, float y, int align, int alignOrg, float fontsize, char font, int r, int g, int b, int a, int glowr, int glowg, int glowb, int glowa) {
    int Element = hud_HudElem_Alloc();
    *(char*)(Element + HUD_TYPE) = 1;
    *(int*)(Element + HUD_TARGET_ENT_NUM) = client;
    *(int*)(Element + HUD_TEXT - 2) = hud_G_LocalizedStringIndex(text);
    *(float*)(Element + HUD_X) = x;
    *(float*)(Element + HUD_Y) = y;
    *(int*)(Element + HUD_ALIGN_SCREEN) = align;
    *(int*)(Element + HUD_ALIGN_ORG) = alignOrg;
    *(float*)(Element + HUD_FONTSCALE) = fontsize;
    *(char*)(Element + HUD_FONT) = font;
    *(char*)(Element + HUD_COLOR) = r;
    *(char*)(Element + HUD_COLOR + 0x01) = g;
    *(char*)(Element + HUD_COLOR + 0x02) = b;
    *(char*)(Element + HUD_COLOR + 0x03) = a;
    *(char*)(Element + HUD_GLOW_COLOR) = glowr;
    *(char*)(Element + HUD_GLOW_COLOR + 0x01) = glowg;
    *(char*)(Element + HUD_GLOW_COLOR + 0x02) = glowb;
    *(char*)(Element + HUD_GLOW_COLOR + 0x03) = glowa;
    *(char*)(Element + HUD_UI3D_WINDOW) = 0xFF;
    return Element;
}

void hud_ChangeHUDColor(int Element, int r, int g, int b, int a) {
    *(char*)(Element + HUD_COLOR) = r;
    *(char*)(Element + HUD_COLOR + 0x01) = g;
    *(char*)(Element + HUD_COLOR + 0x02) = b;
    *(char*)(Element + HUD_COLOR + 0x03) = a;
}

void hud_ChangeMaterial(int Element, char Material) {
    *(char*)(Element + HUD_MATERIAL_INDEX) = Material;
}

void hud_MoveShaderX(unsigned int Element, float x) {
    *(float*)(Element + HUD_X) = x;
}

void hud_MoveShaderXY(unsigned int Element, float x, float y) {
    *(float*)(Element + HUD_X) = x;
    *(float*)(Element + HUD_Y) = y;
}

int hud_SetTypewriter(int clientIndex, char* Text, int Font, float FontSize, float X, float Y, int Type, short Lettertime, short fxDecayStartTime, short fxDecayDuration, int r, int g, int b, int a, int glowr, int glowg, int glowb, int glowa) {
    int elemIndex = hud_SetText(clientIndex, Text, X, Y, 0, 0, FontSize, Font, r, g, b, a, glowr, glowg, glowb, glowa);
    *(int*)(elemIndex + HUD_FLAGS) = Type;
    *(int*)(elemIndex + HUD_FX_BIRTH_TIME) = hud_GetLevelTime();
    *(short*)(elemIndex + HUD_FX_LETTER_TIME) = Lettertime;
    *(short*)(elemIndex + HUD_FX_DECAY_START_TIME) = fxDecayStartTime;
    *(short*)(elemIndex + HUD_FX_DECAY_DURATION) = fxDecayDuration;
    return elemIndex;
}

void hud_DestroyAllElems(void) {
    unsigned int i;
    for (i = 0; i < 1024; i++) {
        char input[0x7C] = {0};
        WriteBytes(0x11F26D4 + (i * 0x7C), input, 0x7C);
    }
}

int hud_GetHost(void) {
    char* str = ReadString(0x02000934);
    int i;
    for (i = 0; i < 12; i++) {
        char* str2 = ReadString(0x013978d0 + (i * 0x2A38));
        if (str == str2) {
            return i;
        }
    }
    return -1;
}

void hud_MoveOverTime1(unsigned int Elem, short time, float X, float Y) {
    *(float*)(Elem + HUD_FROM_X) = *(float*)Elem;
    *(float*)(Elem + HUD_FROM_Y) = *(float*)(Elem + HUD_Y);
    *(int*)(Elem + HUD_MOVE_START_TIME) = hud_GetLevelTime1();
    *(int*)(Elem + HUD_MOVE_TIME) = time;
    *(float*)Elem = X;
    *(float*)(Elem + HUD_Y) = Y;
}

void hud_MoveOverTime(unsigned int Elem, short time, float X, float Y) {
    *(float*)(Elem + HUD_FROM_X) = *(float*)Elem;
    *(float*)(Elem + HUD_FROM_Y) = *(float*)(Elem + HUD_Y);
    *(int*)(Elem + HUD_MOVE_START_TIME) = hud_GetLevelTime();
    *(short*)(Elem + HUD_MOVE_TIME) = time;
    *(float*)Elem = X;
    *(float*)(Elem + HUD_Y) = Y;
}

void hud_FadeOverTime(int elemIndex, short Time, int R, int G, int B, int A) {
    *(int*)(elemIndex + HUD_FROM_COLOR) = *(int*)(elemIndex + HUD_COLOR);
    *(int*)(elemIndex + HUD_FADE_START_TIME) = hud_GetLevelTime();
    {
        char ColorArray[4] = { (char)R, (char)G, (char)B, (char)A };
        WriteBytes(elemIndex + HUD_COLOR, ColorArray, 4);
    }
}

void hud_FadeGlowOverTimez(int elemIndex, short Time, int RG, int GG, int BG, int A) {
    *(int*)(elemIndex + HUD_GLOW_COLOR) = *(int*)(elemIndex + HUD_GLOW_COLOR);
    *(int*)(elemIndex + HUD_FADE_START_TIME) = hud_GetLevelTime();
    {
        char ColorArray[4] = { (char)RG, (char)GG, (char)BG, (char)A };
        WriteBytes(elemIndex + HUD_GLOW_COLOR, ColorArray, 4);
    }
}

void hud_MoveHudOverTime(int elemIndex, float X) {
    WriteFloat(elemIndex + HUD_X, X);
}

void hud_FadeAlphaOverTime(int elemIndex, short Time, char A) {
    char* readColorArray = ReadBytes(elemIndex + HUD_COLOR, 3);
    *(int*)(elemIndex + HUD_FROM_COLOR) = *(int*)(elemIndex + HUD_COLOR);
    *(int*)(elemIndex + HUD_FADE_START_TIME) = hud_GetLevelTime();
    {
        char ColorArray[4] = { readColorArray[0], readColorArray[1], readColorArray[2], A };
        WriteBytes(elemIndex + HUD_COLOR, ColorArray, 4);
    }
}

void hud_ScaleOverTime(int elemIndex, short time, short width, short height) {
    *(short*)(elemIndex + HUD_FROM_HEIGHT) = *(short*)(elemIndex + HUD_HEIGHT);
    *(short*)(elemIndex + HUD_FROM_WIDTH) = *(short*)(elemIndex + HUD_WIDTH);
    *(int*)(elemIndex + HUD_SCALE_START_TIME) = hud_GetLevelTime();
    *(short*)(elemIndex + HUD_SCALE_TIME) = time;
    *(short*)(elemIndex + HUD_WIDTH) = width;
    *(short*)(elemIndex + HUD_HEIGHT) = height;
}

void hud_FontScaleOverTime(int elemIndex, float FontSize, short time) {
    /* Funktionalität war im Original auskommentiert.
       Hier kann später der Code ergänzt werden, falls benötigt. */
}

void hud_ChangeText(int Element, char* text) {
    *(int*)(Element + HUD_TEXT - 2) = hud_G_LocalizedStringIndex(text);
}
