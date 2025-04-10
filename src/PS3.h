#ifndef PS3_H
#define PS3_H

#include <np.h>

#include <sysutil/sysutil_userinfo.h>

#include <sysutil/sysutil_msgdialog.h>
#include <sysutil/sysutil_oskdialog.h>

#include <cell/error.h>
#include <cell/spurs/lfqueue.h>
#include <cell/fs/cell_fs_file_api.h>

extern int yesno_dialog_result;
extern int yesno_dialog_input;
extern int ok_dialog_input;
extern int ok_dialog_result;

void sleep(usecond_t time);

void YesNoDialogCallBack(int button_type, void *userdata);
void OkDialogCallBack(int button_type, void *userdata);
bool DrawYesNoMessageDialog(const char *str);
void DrawOkMessageDialog(const char *str);

const char * GetSelfName();
const char * GetSelfUserName();
const char * GetSelfOnlineName();

void fixSpaces(char** szReturn);
void WriteProcessMemory(uint32_t address, const uint16_t value, size_t size);

#endif /* PS3_H */
