#include "buttons.h"

int localButtons = 0xd19800;

bool ButtonPressed(enum Buttons Button) {
    return (*(int*)(localButtons + Button) != 0);
}
