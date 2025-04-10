#include "t5.h"
#include "utils.h"
#include "scrfct.h"
#include "variables.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysutil/sysutil_oskdialog.h>

void scrfct_setmemory() {
    // Verify that we have 2 parameters for this function.
    if (Scr_GetNumParam(0) == 2) {
        // Getting parameters, we don't check the type using a function again here but we could have did it.
        char *hexAddress = Scr_GetString(0, 0);
        char *hexData = Scr_GetString(1, 0);

        if (hexAddress && hexData) {
            // Allocate input size +2 in case to handle zero padding and null terminated char.
            char hexAddressFixed[strlen(hexAddress) + 2];
            char hexDataFixed[strlen(hexData) + 2];
               
            hex_str_to_padded_hex_str(hexAddressFixed, hexAddress);
            hex_str_to_padded_hex_str(hexDataFixed, hexData);

            size_t addressSize = strlen(hexAddressFixed)/2;
            size_t dataSize = strlen(hexDataFixed)/2;

            int offset = hex_str_to_int32(hexAddressFixed, addressSize);

            char buffer[dataSize];
            memset(buffer, 0, dataSize);
            hex_str_to_buffer(buffer, hexDataFixed, dataSize);

            sys_dbg_process_write(offset, buffer, dataSize);

            printf(T5INFO "Function 'setMemory' called from gsc with the following parameters: %s, %s.", hexAddress, hexData);
        }
        else
            printf(T5ERROR "Cannot resolve setmemory parameters call from gsc.");
    }
}

void scrfct_ps3_keyboard() {
    if (Scr_GetNumParam(0) == 2) {
		char* title = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        int size = Scr_GetInt(1, SCRIPTINSTANCE_SERVER);

		if (size) {
            if (!GSC_Keyboardbool) {
                GSC_Keyboardbool = true;
                DrawKeyboard(title,"", size, CELL_OSKDIALOG_PANELMODE_ALPHABET, GSC_KeyboardCallback);
                Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
            } else {
                Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
            }
		} else {
            Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
        }
    }
}

void GSC_KeyboardCallback(int localClientNum, const wchar_t *text, size_t size) {
	if (size) {
		if (WideCharacterToString(GSC_Keyboardbuffer, text, size)) {
			Scr_ClearOutParams();
			Scr_AddString(GSC_Keyboardbuffer, SCRIPTINSTANCE_SERVER);
			// scr_Notify( 0x012AB290 + (HostNum * 0x2F8),SL_GetString("JM_Keyboard",0,SCRIPTINSTANCE_SERVER),1);
			memset(GSC_Keyboardbuffer, 0, MAX_STRING_CHARS);
			GSC_Keyboardbool = false;
		}
	}
}
