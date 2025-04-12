#include "PS3.h"
#include "menu.h"
#include "globals.h"
#include "functions.h"

/* --- Menu definition --- */
MenuOption mainMenuOptions[] = {
    { "Force Host",      OPTION_SELECTOR, { .selector = {0, 2, toggleValues, toggleForceHost} } },
    { "Players to start",      OPTION_SELECTOR, { .selector = {3, 18, numberValues, toggleForceHost} } },
    { "Max players",      OPTION_SELECTOR, { .selector = {11, 18, numberValues, toggleForceHost} } },
    { "Change gametype", OPTION_SELECTOR, { .selector = {0, 11, gametypeValues, changeGametype} } },
    { "Controls overlay",OPTION_SELECTOR, { .selector = {1, 2, toggleValues, toggleOverlay} } }
};
Menu mainMenu = {"Century Package [Pregame]", 5, mainMenuOptions};

Menu* menus[MENU_COUNT] = {&mainMenu};

/* --- Menu functions --- */
void toggleForceHost(const char* val) {
    if (strcmp(val, "ON") == 0) {
        cBuf_addText("party_host 1; onlinegame 1; onlinegameandhost 1; "
            "party_connectToOthers 0; party_pregameStartTimerLength 0; "
            "party_connectTimeOut 9999; partyMigrate_disabled 1; party_maxTeamDiff 18; "
            "party_mergingEnabled 0; \n");
    } else {
        cBuf_addText("party_host 0; onlinegame 1; onlinegameandhost 0; "
            "party_connectToOthers 1; party_pregameStartTimerLength 60; "
            "party_connectTimeOut 60; partyMigrate_disabled 0; party_maxTeamDiff 4; "
            "party_mergingEnabled 1; \n");
    }
}

void changeMinPlayers(const char* val) {
    cBuf_addTextf("party_minPlayers %s; \n", val);
}

void changeMaxPlayers(const char* val) {
    cBuf_addTextf(
        "party_maxPlayers %s; "
        "party_maxlocalplayers_basictraining %s; "
        "party_maxlocalplayers_playermatch %s; "
        "party_maxlocalplayers_theater %s; "
        "party_maxlocalplayers_wagermatch %s; "
        "party_maxplayers_basictraining %s; "
        "party_maxplayers_playermatch %s; "
        "party_maxplayers_theater %s; "
        "party_maxplayers_wagermatch %s; "
        "party_maxplayers_lobby %s; "
        "party_maxplayers_partybasictraining %s; "
        "party_maxplayers_partylobby %s; "
        "\n",
        val, val, val, val, val, val, val, val, val, val, val, val 
    );
}

void changeGametype(const char* gametype) {
    cBuf_addTextf("g_gametype %s; ui_gametype %s; party_gametype %s; \n", gametype, gametype, gametype);
}

void toggleOverlay(const char* val) {
    showOverlay = (strcmp(val, "ON") == 0);
}

/* --- Menu usage --- */
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
        sleep(200);
    }
}

void adjustOptionRight() {
    Menu* current = menus[currentMenuIndex];
    MenuOption* opt = &current->options[currentOptionIndex];
    if (opt->type == OPTION_SELECTOR) {
        opt->handler.selector.current = (opt->handler.selector.current + 1) % opt->handler.selector.count;
        sleep(200);
    }
}

void selectOption() {
    Menu* current = menus[currentMenuIndex];
    MenuOption* opt = &current->options[currentOptionIndex];
    if (opt->type == OPTION_SUBMENU) {
        if (opt->handler.nextMenu < MENU_COUNT) {
            currentMenuIndex = opt->handler.nextMenu;
            currentOptionIndex = 0;
        }
    } else if (opt->type == OPTION_SELECTOR) {
        // Aktion beim Bestätigen der Option ausführen:
        if (opt->handler.selector.action) {
            const char* currentValue = opt->handler.selector.values[opt->handler.selector.current];
            opt->handler.selector.action(currentValue);
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
