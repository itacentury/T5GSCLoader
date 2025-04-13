#include "PS3.h"
#include "menu.h"
#include "utils.h"
#include "globals.h"
#include "functions.h"

/* --- Menu definition --- */
MenuOption mainMenuOptions[] = {
    { "Force Host",      OPTION_SELECTOR, { .selector = {0, 2, toggleValues, toggleForceHost} } },
    { "Players to start",      OPTION_SELECTOR, { .selector = {3, 18, numberValues, changeMinPlayers} } },
    { "Max players",      OPTION_SELECTOR, { .selector = {11, 18, numberValues, changeMaxPlayers} } },
    { "Change gametype", OPTION_SELECTOR, { .selector = {0, 11, gametypeValues, changeGametype} } },
    { "Controls overlay",OPTION_SELECTOR, { .selector = {1, 2, toggleValues, toggleOverlay} } }
};
Menu mainMenu = {"Century Package [Pregame]", 5, mainMenuOptions};

Menu* menus[MENU_COUNT] = {&mainMenu};

/* --- Menu functions --- */
void toggleForceHost(const char* val) {
    if (strcmp(val, "ON") == 0) {
        forceHostEnabled = true;
    } else {
        forceHostEnabled = false;
		cBuf_addText("party_host 0;onlinegame 1;onlinegameandhost 0;onlineunrankedgameandhost 0;migration_msgtimeout 500;migration_timeBetween 30000;migrationPingTime 10;party_matchedPlayerCount 4;party_connectTimeout 8000;\n");
    }
}

void changeMinPlayers(const char* val) {
    partyMinPlayers = simpleAtoi(val);
}

void changeMaxPlayers(const char* val) {
    partyMaxPlayers = simpleAtoi(val);
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
        
        selectOption();
        sleep(200);
    }
}

void adjustOptionRight() {
    Menu* current = menus[currentMenuIndex];
    MenuOption* opt = &current->options[currentOptionIndex];
    if (opt->type == OPTION_SELECTOR) {
        opt->handler.selector.current = (opt->handler.selector.current + 1) % opt->handler.selector.count;
        
        selectOption();
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
