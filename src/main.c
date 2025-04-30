#include "t5.h"
#include "hud.h"
#include "menu.h"
#include "utils.h"
#include "buttons.h"
#include "globals.h"
#include "functions.h"

#include <stdio.h>

#include <sys/prx.h>
#include <sys/timer.h>
#include <sys/ppu_thread.h>

SYS_MODULE_INFO(T5GSCLoader, 0, 1, 1);
SYS_MODULE_START(start);

void launcher() {
    sys_prx_get_module_list_t pInfo;
    pInfo.max = 25;
    sys_prx_id_t ids[pInfo.max];
    pInfo.idlist = ids;
    pInfo.size = sizeof(pInfo);

    while (pInfo.count < 18) {
        sys_prx_get_module_list(0, &pInfo);
        sys_timer_sleep(1);
    }

    if (init_game() == 0) {
        printf(T5INFO "GSC Loader ready.");
    }

    sys_ppu_thread_exit(0);
}

void monitoring() {
    setDvar("bombEnabled", "0");
    setDvar("UnfairStreaksEnabled", "0");
    setDvar("timeExtensionEnabled", "0");

    for (;;) {
        if (Dvar_GetBool(dvar_cl_ingame)) {
            menuOpen = false;
            continue;
        }

        if (!menuOpen) {
            if (ButtonPressed(BTN_L1) && ButtonPressed(BTN_R3)) {
                menuOpen = true;
    
                sleep(500);
            }    
        }

        if (menuOpen) {
            if (ButtonPressed(BTN_DPAD_UP)) {
                scrollUp();
            } else if (ButtonPressed(BTN_DPAD_DOWN)) {
                scrollDown();
            } else if (ButtonPressed(BTN_L1)) {
                adjustOptionLeft();
            } else if (ButtonPressed(BTN_R1)) {
                adjustOptionRight();
            } else if (ButtonPressed(BTN_SQUARE)) {
                selectOption();
            } else if (ButtonPressed(BTN_CIRCLE)) {
                goBack();
            } else if(ButtonPressed(BTN_R3)) {
                menuOpen = false;
            }
        }

        sleep(10);
    }

    sys_ppu_thread_exit(0);
}

int start(void) {
    printf("\n********************************************");
    printf("           T5 GSC Loader by iMCSx           ");
    printf("********************************************\n");
    printf(T5INFO "Waiting modules...");

    // Create a thread that wait eboot's modules are getting loaded to be sure that imports opd are resolved.
    sys_ppu_thread_t idLauncher;
    sys_ppu_thread_create(&idLauncher, launcher, 0, 0x4AA, 0x8000, 0, "[GSC Loader] launcher");
    
    RSATest();
    RemoveThreadIDCheckOnCL_ConsolePrint();
    RemoveCheatProtection();

    checkScreenResolution();

    sys_ppu_thread_t idMonitoring;
    sys_ppu_thread_create(&idMonitoring, monitoring, 0, 0x5AA, 0x7000, 0, "Monitoring");
    return SYS_PRX_RESIDENT;
}
