#ifndef MENU_H
#define MENU_H

#define MENU_COUNT 1

typedef enum {
    OPTION_SELECTOR,
    OPTION_ACTION,
} OptionType;

typedef struct {
    int current;
    int count;
    const char** values;
    void (*action)(const char*);
} SelectorOption;

typedef struct MenuOption {
    const char* text;
    OptionType type;
    union {
        SelectorOption selector;
        void (*action)(void);
    } handler;
} MenuOption;

typedef struct Menu {
    const char* title;
    int optionCount;
    MenuOption *options;
} Menu;

static const char* toggleValues[] = { "OFF", "ON" };
static const char* gametypeValues[]  = { 
    "tdm", "sd",  "dom", "sab",  "koth", 
    "ctf", "dem", "oic", "hlnd", "gun", "shrp" 
};
static const char* numberValues[] = {
    "1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",
    "10", "11", "12", "13", "14", "15", "16", "17", "18"
};
static const char* prestigeValues[] = {
    "0",  "1",  "2",  "3",  "4",  "5",  "6",  "7",
    "8",  "9",  "10", "11", "12", "13", "14", "15"
};
static const char* levelValues[] = {
    "1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",  "10",
    "11", "12", "13", "14", "15", "16", "17", "18", "19", "20",
    "21", "22", "23", "24", "25", "26", "27", "28", "29", "30",
    "31", "32", "33", "34", "35", "36", "37", "38", "39", "40",
    "41", "42", "43", "44", "45", "46", "47", "48", "49", "50"
};

extern MenuOption mainMenuOptions[];
extern Menu mainMenu;
extern Menu* menus[MENU_COUNT];

extern int currentMenuIndex;
extern int currentOptionIndex;

void toggleForceHost(const char*);
void changeMinPlayers(const char*);
void changeMaxPlayers(const char*);
void changeGametype(const char*);
void levelFifty();
void changePrestige(const char* prestige);
void giveMoney(void);
void toggleOverlay(const char*);
void changeName(void);

void scrollUp();
void scrollDown();
void adjustOptionLeft();
void adjustOptionRight();
void selectOption();
void goBack();

#endif /* MENU_H */
