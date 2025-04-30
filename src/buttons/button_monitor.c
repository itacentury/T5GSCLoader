#include "printf.h"
#include "defines.h"
#include "button_monitor.h"

uint32_t pad = 0, oldPad = 0, curPad = 0;
CellPadData mypdata;

void init_pad(void) {
    cellPadInit(1);
}

void sync_pad(uint32_t sync_pad) {
    pad = sync_pad;
}

bool button_pressed(uint32_t button) {
    return (pad & button) != 0;
}

bool button_pressed_wrapped(uint32_t button) {
    if (button_pressed(button)) {
        // printf(T5INFO "pad '%i', button '%i'", pad, button);
        pad = 0;
        return true;
    }

    return false;
}

void poll_pad(void) {
    uint32_t type;

    int ret = cellPadGetDataExtra(0, &type, &mypdata);
    if (ret == CELL_OK && mypdata.len > 0) {
        curPad = (mypdata.button[2] | (mypdata.button[3] << 8));
        memset(&mypdata, 0, sizeof(CellPadData));

        if (curPad != oldPad) {
            sync_pad(curPad);
        }
        
        oldPad = curPad;
    } else {
        oldPad = 0;
    }
}
