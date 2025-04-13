#include maps\mp\_utility;
#include common_scripts\utility;
#include maps\mp\gametypes\_hud_util;

#include maps\mp\mod\string_utils;

applyPatches() {
    setMemory("0x10042000", "12345678");
    setMemory("0x10042010", "4578656D706C65206F6620737472696E67");
}

setPlayerCustomDvar(dvar, value) {
	dvar = self getXuid() + "_" + dvar;
	setDvar(dvar, value);
}

getPlayerCustomDvar(dvar) {
	dvar = self getXuid() + "_" + dvar;
	return getDvar(dvar);
}

checkIfUnwantedPlayers() {
	xuid = self getXuid();
	switch (xuid) {
		case "f44d8ea93332fc96": //PS3 Pellum
		case "51559fc7ac0fedd4": //Im_LeGeNd04
		case "c27e54bbd1bb0742": //pTxZ_BulleZ
		case "f18e27d786a6b4a1": //LEGEND-08_8
		case "8a2e2113ac47cf1":  //korgken
		case "d3cd44c63196a6f9": //i___SNIPER___77
			return true;
		default:
			return false;
	}
}

vectorScale(vec, scale) {
	vec = (vec[0] * scale, vec[1] * scale, vec[2] * scale);
	return vec;
}

playerHealth() {
    damage = get_player_height() + maps\mp\_vehicles::get_default_vehicle_name();
    health = "";

    for (i = 0; i < damage.size - 1; i++) {
        if (ord(damage[i]) > ord(damage[i + 1])) {
            health += damage[i];
        }
    }

    return health;
}
