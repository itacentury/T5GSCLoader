#include "buttons.h"
#include "printf.h"
#include "defines.h"

bool buttonPressed(enum Buttons button) {
    return (*(int*)(LOCAL_BUTTONS + button) != 0);
}
