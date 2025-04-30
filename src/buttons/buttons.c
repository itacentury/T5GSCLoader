#include "buttons.h"

bool buttonPressed(enum Buttons button) {
    return (*(int*)(LOCAL_BUTTONS + button) == 1);
}

void resetButton(enum Buttons button) {
    volatile int *ptr = (volatile int*)(LOCAL_BUTTONS + button);
    *ptr = 0;
}
