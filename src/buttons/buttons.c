#include "buttons.h"

bool ButtonPressed(enum Buttons button) {
    return (*(int*)(LOCAL_BUTTONS + button) == 1);
}

void ResetButton(enum Buttons Button) {
    volatile int *ptr = (volatile int*)(LOCAL_BUTTONS + Button);
    *ptr = 0;
}
