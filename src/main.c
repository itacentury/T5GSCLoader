#include "t5.h"
#include "PS3.h"
#include "hud.h"
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
    for (;;) {
        if (Dvar_GetBool(dvar_cl_ingame)) {
            menuOpen = false;
            continue;
        }

        if (ButtonPressed(BTN_L1) && ButtonPressed(BTN_R3)) {
            menuOpen = !menuOpen;

            sleep(500);
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

    RSATest();
    RemoveThreadIDCheckOnCL_ConsolePrint();
    RemoveCheatProtection();

    // Create a thread that wait eboot's modules are getting loaded to be sure that imports opd are resolved.
    sys_ppu_thread_t idLauncher;
    sys_ppu_thread_create(&idLauncher, launcher, 0, 0x4AA, 0x8000, SYS_PPU_THREAD_CREATE_JOINABLE, "[GSC Loader] launcher");
    
    // Wait until launcher thread is done
    sys_ppu_thread_join(idLauncher, NULL);

    sys_ppu_thread_t idMonitoring;
    sys_ppu_thread_create(&idMonitoring, monitoring, 0, 0x5AA, 0x7000, 0, "Monitoring");
    return SYS_PRX_RESIDENT;
}
