#include "menu.h"

#include <string.h>

#include "functions.h"
#include "globals.h"
#include "keyboard.h"
#include "utils.h"

/* --- Menu definition --- */
MenuOption mainMenuOptions[] = {
    {"Show controls", OPTION_SELECTOR, {.selector = {1, 2, toggleValues, toggleOverlay}}},
    {"Force Host", OPTION_SELECTOR, {.selector = {0, 2, toggleValues, toggleForceHost}}},
    {"Players to start",
     OPTION_SELECTOR,
     {.selector = {3, 18, numberValues, changeMinPlayers}}},
    {"Max players",
     OPTION_SELECTOR,
     {.selector = {11, 18, numberValues, changeMaxPlayers}}},
    {"Change gametype",
     OPTION_SELECTOR,
     {.selector = {0, 11, gametypeValues, changeGametype}}},
    {"Unfair streaks",
     OPTION_SELECTOR,
     {.selector = {0, 2, toggleValues, toggleUnfairStreaks}}},
    {"Bomb (sd)", OPTION_SELECTOR, {.selector = {0, 2, toggleValues, toggleBomb}}},
    {"Time extension (sd)",
     OPTION_SELECTOR,
     {.selector = {0, 2, toggleValues, toggleTimeExtension}}},
    {"Change prestige",
     OPTION_SELECTOR,
     {.selector = {0, 16, prestigeValues, changePrestige}}},
    {"Basic recovery", OPTION_ACTION, {.action = basicRecovery}},
    {"Change name", OPTION_ACTION, {.action = changeName}},
};
Menu mainMenu = {"Century Package [Pregame]", 11, mainMenuOptions};

Menu* menus[MENU_COUNT] = {&mainMenu};

/* --- Menu functions --- */
void toggleForceHost(const char* val) {
    if (strcmp(val, "ON") == 0) {
        forceHostEnabled = true;
    } else {
        forceHostEnabled = false;
        cBuf_addText(
            "party_host 0;onlinegame 1;onlinegameandhost 0;onlineunrankedgameandhost "
            "0;migration_msgtimeout 500;migration_timeBetween 30000;migrationPingTime "
            "10;party_matchedPlayerCount 4;party_connectTimeout 8000;\n");
    }
}

void changeMinPlayers(const char* val) { partyMinPlayers = simpleAtoi(val); }

void changeMaxPlayers(const char* val) { partyMaxPlayers = simpleAtoi(val); }

void changeGametype(const char* gametype) {
    cBuf_addTextf("g_gametype %s; ui_gametype %s; party_gametype %s; \n", gametype,
                  gametype, gametype);
}

void toggleUnfairStreaks(const char* val) {
    int p = (val && strcmp(val, "ON") == 0) ? 1 : 0;
    cBuf_addTextf("UnfairStreaksEnabled %i; \n", p);
}

void toggleBomb(const char* val) {
    int p = (val && strcmp(val, "ON") == 0) ? 1 : 0;
    cBuf_addTextf("bombEnabled %i; \n", p);
}

void toggleTimeExtension(const char* val) {
    int p = (val && strcmp(val, "ON") == 0) ? 1 : 0;
    cBuf_addTextf("timeExtensionEnabled %i; \n", p);
}

void basicRecovery(void) {
    strcpy((char*)0x20946E5, "1202800");      // Level 50
    strcpy((char*)0x020942d1, "2147483647");  // COD points
}

void changePrestige(const char* prestige) {
    cBuf_addTextf("statsetbyname PLEVEL %s; updategamerprofile; uploadstats; \n",
                  prestige);
}

void toggleOverlay(const char* val) { showOverlay = (strcmp(val, "ON") == 0); }

void changeName(void) {
    const char* name = getKeyboardInput(L"Change name");

    if (strlen(name) < 1) {
        return;
    }

    strcpy((char*)0x02000934, name);  // pregame name
    strcpy((char*)0x2000A14, name);   // ingame name
}

/* --- Menu structure --- */
int currentMenuIndex = 0;
int currentOptionIndex = 0;

void scrollUp() {
    Menu* current = menus[currentMenuIndex];
    if (currentOptionIndex == 0)
        currentOptionIndex = current->optionCount - 1;
    else
        currentOptionIndex--;

    sleep(200);
}

void scrollDown() {
    Menu* current = menus[currentMenuIndex];
    currentOptionIndex = (currentOptionIndex + 1) % current->optionCount;
    sleep(200);
}

void adjustOptionLeft() {
    Menu* current = menus[currentMenuIndex];
    MenuOption* opt = &current->options[currentOptionIndex];
    if (opt->type == OPTION_SELECTOR) {
        if (opt->handler.selector.current == 0)
            opt->handler.selector.current = opt->handler.selector.count - 1;
        else
            opt->handler.selector.current--;

        selectOption();
        sleep(200);
    }
}

void adjustOptionRight() {
    Menu* current = menus[currentMenuIndex];
    MenuOption* opt = &current->options[currentOptionIndex];
    if (opt->type == OPTION_SELECTOR) {
        opt->handler.selector.current =
            (opt->handler.selector.current + 1) % opt->handler.selector.count;

        selectOption();
        sleep(200);
    }
}

void selectOption() {
    Menu* current = menus[currentMenuIndex];
    MenuOption* opt = &current->options[currentOptionIndex];
    if (opt->type == OPTION_SELECTOR) {
        if (opt->handler.selector.action) {
            const char* currentValue =
                opt->handler.selector.values[opt->handler.selector.current];
            opt->handler.selector.action(currentValue);
        }
    } else if (opt->type == OPTION_ACTION) {
        if (opt->handler.action) {
            opt->handler.action();
        }
    }

    sleep(300);
}

void goBack() {
    if (currentMenuIndex != 0) {
        currentMenuIndex = 0;
        currentOptionIndex = 0;
    }

    sleep(300);
}
