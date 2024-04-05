#include "getkeyboard.h"
#include "ssd1306.h"
#include "snake.h"

int fd;

// Clear the terminal
void clrscr() {
    system("clear");
    return;
}

int main() {
    int c;

    fd = OLED_OpenDevFile();

    clrscr();

    OLED_Clear(fd);

    do {
        Snake_loadGame(fd);
        OLED_Clear(fd);
        OLED_SetCursor(fd, 25, 2);
        OLED_Display(fd, "PLAY AGAIN ?");
        OLED_SetCursor(fd, 0, 3);
        OLED_Display(fd, "ENTER. YES  Other. NO");
        c = KEYBOARD_waitForAnyKey();
        if (c == 5) {
            OLED_Clear(fd);
        }
    } while (c == 5);

    OLED_Clear(fd);
    OLED_SetCursor(fd, 30, 3);
    OLED_Display(fd, " END GAME !");

    return 0;
}
