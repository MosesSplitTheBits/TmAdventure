#ifndef MENU_H
#define MENU_H

enum MenuOptions {
    MENU_START = 1,
    MENU_INSTRUCTIONS = 8,
    MENU_EXIT = 9
};

MenuOptions runMenu();

#endif // MENU_H