#include "hud.h"
#include "menu.h"
#include "hooks.h"
#include "buttons.h"
#include "functions.h"

#include "stdio.h"
#include "string.h"

int Scr_LoadScript_Hook(scriptInstance_t inst, const char *scriptName) {
    int res = Scr_LoadScript_Trampoline(inst, scriptName);

    char buffer[255];
    dvar_s *mapname = Dvar_FindVar("mapname");

    if (!mapname) {
        printf(T5ERROR "Dvar mapname not found.");
        return res;
    }

    // The game will load some script on boot (ZM), avoid it.
    if (strcmp(mapname->current.string, "frontend") == 0) {
        return res;
    }

    sprintf(buffer, isMultiplayer ? "maps/mp/%s" : "maps/%s", mapname->current.string);

    // Checking if the current gsc loaded is the one for the current map.
    if (strcmp(scriptName, buffer) == 0) {
        // Save the checksum data, to be returned later from a different hook.
        checksums[inst].checksum = scrVarPub[inst].checksum;
        checksums[inst].programLen = scrCompilePub[inst].programLen;
        checksums[inst].substract = (int)scrVarPub[inst].endScriptBuffer - (int)scrVarPub[inst].programBuffer;

        // Clean all previous values.
        memset(loader.rawFiles, 0, sizeof(loader.rawFiles));

        // Get active mod.
        char modPath[CELL_FS_MAX_FS_PATH_LENGTH];
        get_or_create_mod_path(modPath);

        if (*modPath) {
            // Create asset entry for each file in our mod directory.
            if (create_assets_from_scripts(modPath)) {
                // Load the main file; any gsc used as include inside will be loaded too.
                char *mainMod = isMultiplayer ? "maps/mp/mod/main" : "maps/zm/mod/main";
                Scr_LoadScript_Trampoline(inst, mainMod);

                // Get our main function handle to start it later in another hook.
                modHandle = Scr_GetFunctionHandle(0, mainMod, "main");

                printf(T5INFO "Main function handle of '%s' is 0x%08X.", mainMod, modHandle);
            } else {
                printf(T5ERROR "Couldn't load mod files from '%s'.", modPath);
            }
        } else {
            printf(T5WARNING "Nothing to load.");
        }
    }

    return res;
}

int cellSpursLFQueuePushBody_Hook(CellSpursLFQueue *lfqueue, const void *buffer, unsigned int isBlocking) {
    // Hooked by replacing a popd import since using CTR an instruction of the source function will overwrite toc in stack and cause crashes.
    InflateData *data = (InflateData*)(buffer);
    int ret = cellSpursLFQueuePushBody_Trampoline(lfqueue, buffer, isBlocking);
    GSCLoaderRawfile *lrf = get_loader_rawfile_from_deflated_buffer(data->deflatedBuffer);

    if (lrf) {
        char modPath[CELL_FS_MAX_FS_PATH_LENGTH];
        get_or_create_mod_path(modPath);

        printf(T5INFO "Injecting '%s' data.", lrf->data.name);
        char *name = strrchr(lrf->data.name, '/');

        if (name && *modPath) {
            // Advance pointer past '/'
            name++;
            char filePath[CELL_FS_MAX_FS_PATH_LENGTH];
            sprintf(filePath, "%s/%s", modPath, name);

            int fileSize = get_file_size(filePath);

            if (fileSize <= 0) {
                printf(T5ERROR "Cannot stat '%s' file", lrf->data.name);
                return ret;
            }

            int fd;
            CellFsErrno err = cellFsOpen(filePath, CELL_FS_O_RDONLY, &fd, NULL, 0);
            if (err == CELL_FS_SUCCEEDED) {
                uint64_t read;
                err = cellFsRead(fd, data->hunkMemoryBuffer, fileSize, &read);

                if (err == CELL_FS_SUCCEEDED && read == fileSize)
                    data->hunkMemoryBuffer[fileSize] = 0; // GSC data should be null-terminated.
                else
                    printf(T5ERROR "Failed to read '%s' file.", filePath);

                cellFsClose(fd);
            } else {
                printf(T5ERROR "Cannot open '%s' file.", filePath);
            }
        } else {
            printf(T5ERROR "Cannot get the current mod path or asset name is wrong.");
        }
    }

    return ret;
}

void Scr_GetChecksum_Hook(scrChecksum_t *checksum, scriptInstance_t inst) {
    Scr_GetChecksum_Trampoline(checksum, inst);

    if (checksums[inst].checksum != 0) {
        *checksum = checksums[inst];
        printf(T5INFO "Checksum patched (0x%08X)", checksums[inst].checksum);
        memset(&checksums[inst], 0, sizeof(scrChecksum_t));
    }
}

void Scr_LoadGameType_Hook(void) {
    // Start the gametype entry function by calling the default function.
    Scr_LoadGameType_Trampoline();

    // Start our main function from our handle if modHandle is valid.
    if (modHandle > 0) {
        unsigned short handle = Scr_ExecThread(0, modHandle, 0);
        Scr_FreeThread(handle, 0);
        printf(T5INFO "Mod '%s' started.", loader.currentModName);
    }
}

popd32 Scr_GetFunction_Hook(const char **pName, int *type) {
    popd32 opd = Scr_GetFunction_Trampoline(pName, type);
    if (opd == 0) {
        // Function names are always in lowercase; return an opd pointer if appropriate.
        if (strcmp(*pName, "setmemory") == 0) {
            printf(T5INFO "Function 'setmemory' found.");
            return (popd32)&scrfct_setmemory;
        }
        return 0;
    }
    return opd;
}

void Menu_PaintAll_Hook(int localClientNum, UiContext *dc) {
    Menu_PaintAll_Trampoline(localClientNum, dc);

    if (!firstStart) {
        displayWelcomePopup();
        firstStart = 1;
    }

    if (Dvar_GetBool(dvar_cl_ingame)) {
        return;
    }

    if (forceHostEnabled) {
        cBuf_addText("party_host 1;onlinegame 1;onlinegameandhost 1;onlineunrankedgameandhost 0;migration_msgtimeout 0;migration_timeBetween 999999;migrationPingTime 0;party_matchedPlayerCount 0;party_connectTimeout 1000;party_connectTimeout 1; \n");
    }

    if (partyMinPlayers != 0) {
        cBuf_addTextf("party_minplayers %i; \n", partyMinPlayers);
    }

    if (partyMaxPlayers != 0) {
        cBuf_addTextf("party_maxplayers %i; \n", partyMaxPlayers);
    }

    if (!menuOpen && showOverlay) {
        R_AddCmdDrawText(
            va("Press %s + %s for Century Package [Pregame]", CODE_L1, CODE_R3), 0xFF, 
            R_RegisterFont("fonts/normalfont", 1), 
            10, 710, 
            0.5f, 0.5f, 0.0f, 
            ColorWhite, 
            0
        );
    }

    if (menuOpen) {
        Menu* current = menus[currentMenuIndex];

        R_AddCmdDrawText(
            va("%s %s/%s %s - Scroll/Rotate | %s - Select | %s - Exit", CODE_DPAD_UP, CODE_DPAD_DOWN, CODE_L1, CODE_R1, CODE_SQUARE, CODE_R3), 0xFF, 
            R_RegisterFont("fonts/normalfont", 1), 
            10, 710, 
            0.5f, 0.5f, 0.0f, 
            ColorWhite, 
            0
        );

        R_AddCmdDrawStretchPic(
            SCREEN_CENTER_X - (260 / 2), 
            SCREEN_CENTER_Y - (354 / 2), 
            260, 354, 
            0.0f, 0.0f, 1.0f, 1.0f, 
            ColorBackground, 
            Material_RegisterHandle("white", 7)
        );
        R_AddCmdDrawText(
            current->title, 0xFF, 
            R_RegisterFont("fonts/extrabigfont", 1), 
            (SCREEN_CENTER_X - 110), 
            (SCREEN_CENTER_Y - (354 / 2) + 35), 
            0.55f, 0.55f, 0.0f, 
            ColorMenuTitle, 
            0
        );
    
        for (int i = 0; i < current->optionCount; i++) {
            int yPos = SCREEN_CENTER_Y - (354 / 2) + 60 + (15 * i);
            char leftText[128];
            snprintf(leftText, sizeof(leftText), "%c %s", 
                (i == currentOptionIndex ? '>' : ' '), current->options[i].text);

            R_AddCmdDrawText(
                leftText, 
                0xFF, 
                R_RegisterFont("fonts/smallfont", 1), 
                SCREEN_CENTER_X - 125, 
                yPos, 
                0.55f, 0.55f, 0.0f, 
                ColorWhite, 
                0
            );

            if (current->options[i].type == OPTION_SELECTOR) {
                char formattedValue[64];
                const char* currentValue = current->options[i].handler.selector.values[
                    current->options[i].handler.selector.current
                ];
                snprintf(formattedValue, sizeof(formattedValue), "%c%s%c", 
                    (i == currentOptionIndex ? '<' : ' '), currentValue, (i == currentOptionIndex ? '>' : ' '));
                R_AddCmdDrawText(
                    formattedValue, 
                    0xFF, 
                    R_RegisterFont("fonts/smallfont", 1), 
                    SCREEN_CENTER_X + 90, 
                    yPos, 
                    0.55f, 0.55f, 0.0f, 
                    ColorWhite, 
                    0
                );
            }
        }
    }
}
