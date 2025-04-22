#include "defines.h"
#include "keyboard.h"

#include <wchar.h>
#include <stdint.h>
#include <sys/timer.h>
#include <sys/sys_time.h>
#include <cell/sysmodule.h>
#include <sysutil/sysutil_oskdialog.h>
#include <sysutil/sysutil_msgdialog.h>

int oskdialog_mode = MODE_IDLE;

char *run_keyboard_dialog(const wchar_t *prompt) {
    char dest[CELL_OSKDIALOG_STRING_SIZE + 1] = {0};
    wchar_t init_text[CELL_OSKDIALOG_STRING_SIZE + 1] = L"";
    wchar_t message[CELL_OSKDIALOG_STRING_SIZE + 1] = {0};
    wcsncpy(message, prompt, CELL_OSKDIALOG_STRING_SIZE);

    oskdialog_mode = MODE_OPEN;
    while (oskdialog_mode != MODE_EXIT) {
        sys_timer_usleep(16 * 1000);
        keyboard(dest, init_text, message);
    }

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
    int ret;

    CellOskDialogInputFieldInfo inputFieldInfo;
    inputFieldInfo.message          = (uint16_t*)MESSAGE;
    inputFieldInfo.init_text        = (uint16_t*)INIT_TEXT;
    inputFieldInfo.limit_length     = CELL_OSKDIALOG_STRING_SIZE;

    CellOskDialogCallbackReturnParam OutputInfo;
    OutputInfo.result               = CELL_OSKDIALOG_INPUT_FIELD_RESULT_OK;
    OutputInfo.numCharsResultString = CELL_OSKDIALOG_STRING_SIZE;
    static uint16_t Result_Text_Buffer[CELL_OSKDIALOG_STRING_SIZE + 1];
    OutputInfo.pResultString        = Result_Text_Buffer;

    ret = cellOskDialogSetKeyLayoutOption(
        CELL_OSKDIALOG_10KEY_PANEL |
        CELL_OSKDIALOG_FULLKEY_PANEL
    );

    CellOskDialogPoint pos = { .x = 0.0f, .y = 0.0f };
    int32_t LayoutMode = CELL_OSKDIALOG_LAYOUTMODE_X_ALIGN_CENTER |
                         CELL_OSKDIALOG_LAYOUTMODE_Y_ALIGN_TOP;
    ret = cellOskDialogSetLayoutMode(LayoutMode);

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

    sys_timer_usleep(16 * 1000);
    ret = cellSysutilCheckCallback();

    if (oskdialog_mode == MODE_OPEN) {
        if (cellSysutilRegisterCallback(0, sysutil_callback, NULL) != 0) {
            printf(T5ERROR "Couldn't register the keyboard!");
        }

        ret = cellOskDialogLoadAsync(SYS_MEMORY_CONTAINER_ID_INVALID, &dialogParam, &inputFieldInfo);
        oskdialog_mode = MODE_RUNNING;
    }

    if (oskdialog_mode == MODE_ENTERED) {
        ret = cellOskDialogGetInputText(&OutputInfo);
        oskdialog_mode = MODE_RUNNING;
    }

    if (oskdialog_mode == MODE_CLOSE) {
        ret = cellOskDialogUnloadAsync(&OutputInfo);

        int strLen = getkbLen((char*)OutputInfo.pResultString);
        makekbStr((char*)OutputInfo.pResultString, dest, strLen);

        if (cellSysutilUnregisterCallback(0) != 0) {
            printf(T5ERROR "Couldn't unregister the keyboard!");
        }

        oskdialog_mode = MODE_EXIT;
    }

    return 0;
}
