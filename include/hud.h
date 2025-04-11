#ifndef HUD_H
#define HUD_H

typedef enum {
    HUD_X                    = 0x00,
    HUD_Y                    = 0x04,
    HUD_Z                    = 0x08,
    HUD_FONTSCALE            = 0x0C,
    HUD_COLOR                = 0x10,
    HUD_SORT                 = 0x38,
    HUD_GLOW_COLOR           = 0x3C,
    HUD_TARGET_ENT_NUM       = 0x70,
    HUD_LABEL                = 0x48,
    HUD_WIDTH                = 0x4A,
    HUD_HEIGHT               = 0x4C,
    HUD_TEXT                 = 0x56,
    HUD_TYPE                 = 0x64,
    HUD_FONT                 = 0x65,
    HUD_MATERIAL_INDEX       = 0x68,
    HUD_UI3D_WINDOW          = 0x6D,
    HUD_FROM_COLOR           = 20,
    HUD_FADE_START_TIME      = 24,
    HUD_SCALE_START_TIME     = 28,
    HUD_FROM_X               = 32,
    HUD_FROM_Y               = 36,
    HUD_MOVE_START_TIME      = 40,
    HUD_TIME                 = 44,
    HUD_DURATION             = 48,
    HUD_VALUE                = 52,
    HUD_FX_BIRTH_TIME        = 64,
    HUD_FROM_WIDTH           = 78,
    HUD_FROM_HEIGHT          = 80,
    HUD_SCALE_TIME           = 82,
    HUD_MOVE_TIME            = 84,
    HUD_FX_LETTER_TIME       = 88,
    HUD_FX_DECAY_START_TIME  = 90,
    HUD_FX_DECAY_DURATION    = 92,
    HUD_FX_REDACT_DECAY_START_TIME = 94,
    HUD_FX_REDACT_DECAY_DURATION   = 96,
    HUD_FLAGS                = 98,
    HUD_ALIGN_ORG            = 102,
    HUD_ALIGN_SCREEN         = 103,
    HUD_OFFSCREEN_MATERIAL_IDX = 105,
    HUD_FROM_ALIGN_ORG       = 106,
    HUD_FROM_ALIGN_SCREEN    = 107,
    HUD_SOUND_ID             = 108,
    HUD_UNKNOWN              = 110
} HudOffsets;

typedef enum {
    HUD_TEXT_TYPE_NORMAL  = 0x800,
    HUD_TEXT_TYPE_CLASSIC = 0x4000
} HudTypewriterTextTypes;

extern int HudMenuIDs_NewsBar[12];
extern int HudMenuIDs_NewsbarUpLine[12];
extern int HudMenuIDs_NewsbarBG[12];
extern int HudMenuIDs_NewsbarUnLine[12];
extern int HudMenuIDs_Scrollbar[12];
extern int HudMenuIDs_Shader[12];
extern int HudMenuIDs_Sidebar[12];
extern int HudMenuIDs_Sidebar2[12];
extern int HudMenuIDs_MenuTitle[12];
extern int HudMenuIDs_OptionsText[12];
extern int HudMenuIDs_MenuName[12];
extern int HudMenuIDs_Creator[12];
extern char* HudMenuIDs_MenuOptionText[12];
extern char* HudMenuIDs_MenuTitleText[12];
extern char* HudMenuIDs_CreatorText[12];
extern char* HudMenuIDs_NewsBarText[12];
extern int HudMenuIDs_PlayerVerification[12];

int hud_G_LocalizedStringIndex(char* txt);
int hud_GetLevelTime(void);
int hud_GetLevelTime1(void);
int hud_HudElem_Alloc(void);
int hud_SetShader(int client, char Material, short Width, short Height, float x, float y, int r, int g, int b, int a);
int hud_SetText(int client, char* text, float x, float y, int align, int alignOrg, float fontsize, char font, int r, int g, int b, int a, int glowr, int glowg, int glowb, int glowa);
void hud_ChangeHUDColor(int Element, int r, int g, int b, int a);
void hud_ChangeMaterial(int Element, char Material);
void hud_MoveShaderX(unsigned int Element, float x);
void hud_MoveShaderXY(unsigned int Element, float x, float y);
int hud_SetTypewriter(int clientIndex, char* Text, int Font, float FontSize, float X, float Y, int Type, short Lettertime, short fxDecayStartTime, short fxDecayDuration, int r, int g, int b, int a, int glowr, int glowg, int glowb, int glowa);
void hud_DestroyAllElems(void);
int hud_GetHost(void);
void hud_MoveOverTime1(unsigned int Elem, short time, float X, float Y);
void hud_MoveOverTime(unsigned int Elem, short time, float X, float Y);
void hud_FadeOverTime(int elemIndex, short Time, int R, int G, int B, int A);
void hud_FadeGlowOverTimez(int elemIndex, short Time, int RG, int GG, int BG, int A);
void hud_MoveHudOverTime(int elemIndex, float X);
void hud_FadeAlphaOverTime(int elemIndex, short Time, char A);
void hud_ScaleOverTime(int elemIndex, short time, short width, short height);
void hud_FontScaleOverTime(int elemIndex, float FontSize, short time);
void hud_ChangeText(int Element, char* text);

#endif /* HUD_H */
