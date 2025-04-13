#include maps\mp\_utility;
#include common_scripts\utility;
#include maps\mp\gametypes\_hud_util;

#include maps\mp\mod\init;
#include maps\mp\mod\utils;
#include maps\mp\mod\rights_management;

main() {
    applyPatches();

    initLevelVars();
    initDvars();
    initClassOverrides(); // doesnt work?
    precacheStuff();
    
    level thread onPlayerConnect();
}

onPlayerConnect() {
    for(;;) {
        level waittill("connected", player);

        player initPlayerVars();
        player initPlayerDvars();

        if (player checkIfUnwantedPlayers()) {
			ban(player getEntityNumber(), 1);
		}

        player thread onPlayerSpawned();
    }
}

onPlayerSpawned() {
    self endon("disconnect");

    firstSpawn = true;
    for(;;) {
        self waittill("spawned_player");

        self iPrintLn("Hello from ^1injected ^7GSC");

        if (firstSpawn) {
            if (self hasHostRights() && !self.canRevive) {
                self.canRevive = true;
            }

            firstSpawn = false;
        }
    }
}
