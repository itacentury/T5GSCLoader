#include "printf.h"
#include "defines.h"
#include "keyboard.h"
#include "functions.h"

#include <stdint.h>

#include <sys/timer.h>
#include <sys/sys_time.h>
#include <cell/sysmodule.h>
#include <sysutil/sysutil_oskdialog.h>

int oskdialog_mode = MODE_IDLE;

char *getKeyboardInput(const wchar_t *prompt) {
    char dest[CELL_OSKDIALOG_STRING_SIZE + 1] = {0};
    wchar_t init_text[CELL_OSKDIALOG_STRING_SIZE + 1] = L"";
    wchar_t message[CELL_OSKDIALOG_STRING_SIZE + 1] = {0};
    wcsncpy(message, prompt, CELL_OSKDIALOG_STRING_SIZE);

    oskdialog_mode = MODE_OPEN;
    while (oskdialog_mode != MODE_EXIT) {
        sleep(16);

        int ret = keyboard(dest, init_text, message);
        if (ret < 0) {
            break;
        }
    }

    oskdialog_mode = MODE_IDLE;

    return dest;
}

static void sysutil_callback(uint64_t status, uint64_t param, void *userdata) {
    (void)param;
    (void)userdata;

    switch (status) {
        case CELL_SYSUTIL_OSKDIALOG_LOADED:
            break;
        case CELL_SYSUTIL_OSKDIALOG_FINISHED:
            oskdialog_mode = MODE_CLOSE;
            break;
        case CELL_SYSUTIL_REQUEST_EXITGAME:
            oskdialog_mode = MODE_EXIT;
            break;
        case CELL_SYSUTIL_OSKDIALOG_INPUT_ENTERED:
            oskdialog_mode = MODE_ENTERED;
            break;
        case CELL_SYSUTIL_OSKDIALOG_INPUT_CANCELED:
            oskdialog_mode = MODE_CANCELED;
            break;
        default:
            break;
    }
}

int getkbLen(char *str) {
    int nullCount = 0;
    int i;
    for (i = 0; i < 64; i++) {
        if (nullCount == 2) {
            break;
        }
        if (str[i] == 0x00) {
            nullCount++;
        } else {
            nullCount = 0;
        }
    }
    return i;
}

void makekbStr(char *str, char *dest, int len) {
    int nulls = 0;
    for (int i = 0; i < len; i++) {
        if (str[i] == 0x00) {
            nulls++;
        } else {
            dest[i - nulls] = str[i];
        }
    }
    dest[len - nulls] = 0x00;
}

int keyboard(char *dest, wchar_t *INIT_TEXT, wchar_t *MESSAGE) {
    CellOskDialogInputFieldInfo inputFieldInfo;
    inputFieldInfo.message          = (uint16_t*)MESSAGE;
    inputFieldInfo.init_text        = (uint16_t*)INIT_TEXT;
    inputFieldInfo.limit_length     = CELL_OSKDIALOG_STRING_SIZE;

    CellOskDialogCallbackReturnParam OutputInfo;
    OutputInfo.result               = CELL_OSKDIALOG_INPUT_FIELD_RESULT_OK;
    OutputInfo.numCharsResultString = CELL_OSKDIALOG_STRING_SIZE;
    static uint16_t Result_Text_Buffer[CELL_OSKDIALOG_STRING_SIZE + 1];
    OutputInfo.pResultString        = Result_Text_Buffer;

    if (cellOskDialogSetKeyLayoutOption(CELL_OSKDIALOG_10KEY_PANEL | CELL_OSKDIALOG_FULLKEY_PANEL) != CELL_OK) {
        printf(T5ERROR "Failed to set key layout options!");
        return -1;
    }

    CellOskDialogPoint pos = { .x = 0.0f, .y = 0.0f };
    int32_t LayoutMode = CELL_OSKDIALOG_LAYOUTMODE_X_ALIGN_CENTER |
                         CELL_OSKDIALOG_LAYOUTMODE_Y_ALIGN_TOP;
    if (cellOskDialogSetLayoutMode(LayoutMode) != CELL_OK) {
        printf(T5ERROR "Failed to set layout mode!");
        return -1;
    }

    CellOskDialogParam dialogParam = {
        .allowOskPanelFlg = CELL_OSKDIALOG_PANELMODE_KOREAN     |
                            CELL_OSKDIALOG_PANELMODE_ALPHABET   |
                            CELL_OSKDIALOG_PANELMODE_NUMERAL_FULL_WIDTH |
                            CELL_OSKDIALOG_PANELMODE_NUMERAL    |
                            CELL_OSKDIALOG_PANELMODE_JAPANESE   |
                            CELL_OSKDIALOG_PANELMODE_JAPANESE_KATAKANA |
                            CELL_OSKDIALOG_PANELMODE_ENGLISH,
        .firstViewPanel   = CELL_OSKDIALOG_PANELMODE_ALPHABET,
        .controlPoint     = pos,
        .prohibitFlgs     = 0
    };

    sleep(16);
    if (cellSysutilCheckCallback() != CELL_OK) {
        printf(T5ERROR "Failed to check sysutil callback!");
        return -1;
    }

    if (oskdialog_mode == MODE_OPEN) {
        if (cellSysutilRegisterCallback(0, sysutil_callback, NULL) != CELL_OK) {
            printf(T5ERROR "Couldn't register sysutil callback!");
            return -1;
        }

        if (cellOskDialogLoadAsync(SYS_MEMORY_CONTAINER_ID_INVALID, &dialogParam, &inputFieldInfo) != CELL_OK) {
            printf(T5ERROR "Failed to load OSK dialog");
            cellSysutilUnregisterCallback(0);
            return -1;
        }
    
        oskdialog_mode = MODE_RUNNING;
    }

    if (oskdialog_mode == MODE_ENTERED) {
        if (cellOskDialogGetInputText(&OutputInfo) != CELL_OK) {
            printf(T5ERROR "Failed to get input text!");
            return -1;
        }

        oskdialog_mode = MODE_RUNNING;
    }

    if (oskdialog_mode == MODE_CLOSE) {
        if (cellOskDialogUnloadAsync(&OutputInfo) != CELL_OK) {
            printf(T5ERROR "Failed to unload dialog async!");
            return -1;
        }

        int strLen = getkbLen((char*)OutputInfo.pResultString);
        makekbStr((char*)OutputInfo.pResultString, dest, strLen);

        if (cellSysutilUnregisterCallback(0) != 0) {
            printf(T5ERROR "Couldn't unregister the keyboard!");
        }

        oskdialog_mode = MODE_EXIT;
    }

    return 0;
}
