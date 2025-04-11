#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdbool.h>

extern int LocalButtons;

enum Buttons {
    DpadUp = 0x1FF,
    DpadDown = 0x20F,
    DpadRight = 0x21F,
    DpadLeft = -0xC47E5F,
    R3 = 0x1CF,
    L3 = 0x1BF,
    Square = 0xEF,
    Cross = 0xCF,
    Triangle = 0xFF,
    Circle = 0xDF,
    LeftCursorUp = 0x27F,
    LeftCursorLeft = 0x29F,
    LeftCursorRight = 0x2AF,
    LeftCursorDown = 0x28F,
    R2 = 0x1EF,
    L2 = 0x1DF,
    L1 = 0x10F,
    R1 = 0x11F,
    Select = 0x1AF,
    Start = 0x19F
};

bool ButtonPressed(enum Buttons Button);

#endif /* BUTTONS_H */
