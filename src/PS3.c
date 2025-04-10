#include "PS3.h"

#include <string.h>
#include <sys/timer.h>

int yesno_dialog_result = 0;
int yesno_dialog_input = 0;
int ok_dialog_input = 0;
int ok_dialog_result = 0;

void sleep(usecond_t time) {
    sys_timer_usleep(time * 1000);
}

void YesNoDialogCallBack(int button_type, void *userdata) {
	switch (button_type) {
	    case CELL_MSGDIALOG_BUTTON_YES:
		    yesno_dialog_result = 1;
		    break;
	    case CELL_MSGDIALOG_BUTTON_NO:
		    yesno_dialog_result = 0;
		    break;
    }

	yesno_dialog_input = 0;
}

bool DrawYesNoMessageDialog(const char *str) {
	cellMsgDialogOpen2(CELL_MSGDIALOG_TYPE_SE_TYPE_NORMAL | CELL_MSGDIALOG_TYPE_BUTTON_TYPE_YESNO | CELL_MSGDIALOG_TYPE_DISABLE_CANCEL_ON | CELL_MSGDIALOG_TYPE_DEFAULT_CURSOR_NO, str, YesNoDialogCallBack, NULL, NULL);
	yesno_dialog_input = 1;
	while (yesno_dialog_input) {
		sleep(16);
		cellSysutilCheckCallback();
	}

	return yesno_dialog_result;
}

void OkDialogCallBack(int button_type, void *userdata) {
	switch (button_type) {
	    case CELL_MSGDIALOG_BUTTON_OK:
            ok_dialog_result = 1;
            break;
	    default:
            break;
	}

	ok_dialog_input = 0;
}

void DrawOkMessageDialog(const char *str) {
	cellMsgDialogOpen2(CELL_MSGDIALOG_TYPE_SE_TYPE_NORMAL | CELL_MSGDIALOG_TYPE_BUTTON_TYPE_OK | CELL_MSGDIALOG_TYPE_DISABLE_CANCEL_ON | CELL_MSGDIALOG_TYPE_DEFAULT_CURSOR_OK, str, OkDialogCallBack, NULL, NULL);
	ok_dialog_input = 1;
	while (ok_dialog_input) {
		sleep(16);
		cellSysutilCheckCallback();
	}
}

const char * GetSelfUserName() {
    CellUserInfoUserStat stat;
    cellUserInfoGetStat(CELL_SYSUTIL_USERID_CURRENT, &stat);
    return stat.name;
}
 
const char * GetSelfOnlineName() {
    SceNpOnlineName onlineName;
    sceNpManagerGetOnlineName(&onlineName);
    return onlineName.data;
}
 
const char * GetSelfName() {
    int connectionStatus;
    sceNpManagerGetStatus(&connectionStatus); // checks if online
    if (connectionStatus == SCE_NP_MANAGER_STATUS_ONLINE)
        return GetSelfOnlineName();
    else
        return GetSelfUserName();
}

void fixSpaces(char** szReturn) {
    char *str = *szReturn;
    static char _returnStr[512];// = (char*)Malloc(128);
    int dwStrIndex = 0;
    _returnStr[0] = '\0';

    for (int i = 0; i < strlen(str) + 1; i++) {
        if (str[i] != 0x20) {
            _returnStr[dwStrIndex] = str[i];
            dwStrIndex++;
        }
        else
        {
            _returnStr[dwStrIndex] = '%';
            _returnStr[dwStrIndex + 1] = '2';
            _returnStr[dwStrIndex + 2] = '0';
            dwStrIndex += 3;
        }

        _returnStr[dwStrIndex] = '\0';
    }

    *szReturn = _returnStr;
}
