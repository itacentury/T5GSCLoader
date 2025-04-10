#include "buttons.h"

int LocalButtons = 0xd19800;

bool ButtonPressed(enum Buttons Button) {
    return (*(int*)(LocalButtons + Button) != 0);
}
