#ifndef HOOKS_H
#define HOOKS_H

#include <cell/cell_fs.h>
#include <sys/fs_external.h>

#include "globals.h"
#include "scrfct.h"
#include "t5.h"
#include "utils.h"

int Scr_LoadScript_Hook(scriptInstance_t inst, const char* scriptName);
int cellSpursLFQueuePushBody_Hook(CellSpursLFQueue* lfqueue, const void* buffer,
                                  unsigned int isBlocking);
void Scr_GetChecksum_Hook(scrChecksum_t* checksum, scriptInstance_t inst);
void Scr_LoadGameType_Hook(void);
popd32 Scr_GetFunction_Hook(const char** pName, int* type);
void Menu_PaintAll_Hook(int localClientNum, UiContext* dc);
void ClientCommand_Hook(int clientNum);

void applyHostSettings(void);
void drawPregameOverlay(void);
void checkDvars(void);
void drawMenuUI(void);

#endif /* HOOKS_H */
