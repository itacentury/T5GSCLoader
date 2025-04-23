#include "t5.h"
#include "utils.h"
#include "printf.h"
#include "scrfct.h"
#include "globals.h"
#include "keyboard.h"

#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ppu_thread.h>
#include <sysutil/sysutil_oskdialog.h>

KeyboardThreadArg gKbArgs[MAX_KB_THREADS];
int gNextKb = 0;

void scrfct_setmemory() {
    // Verify that we have 2 parameters for this function.
    if (Scr_GetNumParam(0) == 2) {
        // Getting parameters, we don't check the type using a function again here but we could have did it.
        char *hexAddress = Scr_GetString(0, 0);
        char *hexData = Scr_GetString(1, 0);

        if (hexAddress && hexData) {
            // Allocate input size +2 in case to handle zero padding and null terminated char.
            char hexAddressFixed[strlen(hexAddress) + 2];
            char hexDataFixed[strlen(hexData) + 2];
               
            hex_str_to_padded_hex_str(hexAddressFixed, hexAddress);
            hex_str_to_padded_hex_str(hexDataFixed, hexData);

            size_t addressSize = strlen(hexAddressFixed)/2;
            size_t dataSize = strlen(hexDataFixed)/2;

            int offset = hex_str_to_int32(hexAddressFixed, addressSize);

            char buffer[dataSize];
            memset(buffer, 0, dataSize);
            hex_str_to_buffer(buffer, hexDataFixed, dataSize);

            sys_dbg_process_write(offset, buffer, dataSize);

            printf(T5INFO "Function 'setMemory' called from gsc with the following parameters: %s, %s.", hexAddress, hexData);
        }
        else
            printf(T5ERROR "Cannot resolve setmemory parameters call from gsc.");
    }
}

void scrfct_callkeyboard() {
    if (Scr_GetNumParam(0) != 2) {
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
        printf(T5ERROR "'callkeyboard' called with wrong parameters.");
        return;
    }

    char* title = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    int clientNum = Scr_GetInt(1, SCRIPTINSTANCE_SERVER);

    printf(T5INFO "'callkeyboard' called with title '%s' and clientnum '%i'", title, clientNum);

    int idx = gNextKb;
    gNextKb = (gNextKb + 1) % MAX_KB_THREADS;
    KeyboardThreadArg *ka = &gKbArgs[idx];
    ka->clientNum = clientNum;

    size_t len = strlen(title) + 1;
    wchar_t wtitle[len];
    StringToWideCharacter(ka->wtitle, title, len);

    sys_ppu_thread_t idKeyboard;
    int ret = sys_ppu_thread_create(&idKeyboard, keyboard_thread, ka, 0, 0x7000, 0, "Keyboard thread");
    if (ret != CELL_OK) {
        printf(T5ERROR "Failed to create keyboard thread (0x%X)", ret);
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    } else {
        printf(T5INFO "Started keyboard thread");
    }
}

void keyboard_thread(void *arg) {
    KeyboardThreadArg *ka = (KeyboardThreadArg*)arg;

    Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    const char *message = getKeyboardInput(ka->wtitle);

    printf(T5INFO "User entered message '%s'. Notifying..", message);

    Scr_ClearOutParams();
    Scr_AddString(message, SCRIPTINSTANCE_SERVER);
    Scr_Notify(0x012AB290 + (ka->clientNum * 0x2F8), SL_GetString("keyboard_input", 0, SCRIPTINSTANCE_SERVER), 1);

    printf(T5INFO "Notified. Exiting keyboard thread..");
    sys_ppu_thread_exit(0);
}
