#include "buttons.h"

bool ButtonPressed(enum Buttons Button) {
    return (*(int*)(LOCAL_BUTTONS_ADDR + Button) != 0);
}
