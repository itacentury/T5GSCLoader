/* keyboard.h */
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include <wchar.h>

enum KeyboardMode {
    MODE_IDLE = 0,
    MODE_OPEN,
    MODE_RUNNING,
    MODE_CLOSE,
    MODE_ENTERED,
    MODE_CANCELED,
    SET_ABORT_TIMER,
    CHANGE_PANEL_MODE,
    CHANGE_SCALE,
    SET_CALLBACK,
    MODE_EXIT,
    START_DIALOG_TYPE,
    START_SEPARATE_TYPE_1,
    START_SEPARATE_TYPE_2
};

extern int oskdialog_mode;

char *run_keyboard_dialog(const wchar_t *prompt);
int getkbLen(char *str);
void makekbStr(char *str, char *dest, int len);
int keyboard(char *dest, wchar_t *INIT_TEXT, wchar_t *MESSAGE);

#endif /* KEYBOARD_H */
