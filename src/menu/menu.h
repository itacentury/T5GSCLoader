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
    MenuOption* options;
} Menu;

static const char* toggleValues[] = {"OFF", "ON"};
static const char* gametypeValues[] = {"tdm", "sd",  "dom",  "sab", "koth", "ctf",
                                       "dem", "oic", "hlnd", "gun", "shrp"};
static const char* numberValues[] = {"1",  "2",  "3",  "4",  "5",  "6",
                                     "7",  "8",  "9",  "10", "11", "12",
                                     "13", "14", "15", "16", "17", "18"};
static const char* prestigeValues[] = {"0", "1", "2",  "3",  "4",  "5",  "6",  "7",
                                       "8", "9", "10", "11", "12", "13", "14", "15"};

extern MenuOption mainMenuOptions[];
extern Menu mainMenu;
extern Menu* menus[MENU_COUNT];

extern int currentMenuIndex;
extern int currentOptionIndex;

void toggleForceHost(const char*);
void changeMinPlayers(const char*);
void changeMaxPlayers(const char*);
void changeGametype(const char*);
void toggleUnfairStreaks(const char* val);
void toggleBomb(const char*);
void toggleTimeExtension(const char* val);
void changePrestige(const char* prestige);
void basicRecovery(void);
void toggleOverlay(const char*);
void changeName(void);

void scrollUp();
void scrollDown();
void adjustOptionLeft();
void adjustOptionRight();
void selectOption();
void goBack();

#endif /* MENU_H */
