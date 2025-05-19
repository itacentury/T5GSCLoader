#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdbool.h>

#define LOCAL_BUTTONS_ADDR (int*)0x0D19800U

#define CODE_CROSS           "\x01"
#define CODE_CIRCLE          "\x02"
#define CODE_SQUARE          "\x03"
#define CODE_TRIANGLE        "\x04"
#define CODE_L1              "\x05"
#define CODE_R1              "\x06"
#define CODE_L3              "\x10"
#define CODE_R3              "\x11"
#define CODE_L2              "\x12"
#define CODE_R2              "\x13"
#define CODE_DPAD_UP         "\x14"
#define CODE_DPAD_DOWN       "\x15"
#define CODE_DPAD_LEFT       "\x16"
#define CODE_DPAD_RIGHT      "\x17"
#define CODE_DPAD_LEFT_RIGHT "\x18"
#define CODE_DPAD_UP_DOWN    "\x19"

enum Buttons {
    BTN_DPAD_UP             = 0x1FF,
    BTN_DPAD_DOWN           = 0x20F,
    BTN_DPAD_RIGHT          = 0x21F,
    BTN_DPAD_LEFT           = -0xC47E5F,
    BTN_R3                  = 0x1CF,
    BTN_L3                  = 0x1BF,
    BTN_SQUARE              = 0xEF,
    BTN_CROSS               = 0xCF,
    BTN_TRIANGLE            = 0xFF,
    BTN_CIRCLE              = 0xDF,
    BTN_LEFT_CURSOR_UP      = 0x27F,
    BTN_LEFT_CURSOR_LEFT    = 0x29F,
    BTN_LEFT_CURSOR_RIGHT   = 0x2AF,
    BTN_LEFT_CURSOR_DOWN    = 0x28F,
    BTN_R2                  = 0x1EF,
    BTN_L2                  = 0x1DF,
    BTN_L1                  = 0x10F,
    BTN_R1                  = 0x11F,
    BTN_SELECT              = 0x1AF,
    BTN_START               = 0x19F
};

bool ButtonPressed(enum Buttons Button);

#endif /* BUTTONS_H */
