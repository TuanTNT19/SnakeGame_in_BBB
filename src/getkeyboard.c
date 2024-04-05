#include "getkeyboard.h"

//check any key press
int KEYBOARD_CheckKey_Press()
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

//Release key press
char KEYBOARD_Key_Press()
{
    char c;
    system("stty raw");
    c= getchar();
    system("stty sane");
    return(c);
}

//wait untik key press and return it
char KEYBOARD_waitForAnyKey()
{
    int pressed;

    while(!KEYBOARD_CheckKey_Press());

    pressed = KEYBOARD_Key_Press();
    
    return((char)pressed);
}

//check keypress and return direction
int KEYBOARD_KeyPress_to_Direction(int direction)
{
    int pressed;
    if(KEYBOARD_CheckKey_Press()) //If a key has been pressed
    {
        pressed=KEYBOARD_Key_Press();
        if (direction != pressed)
        {
            if(pressed == DOWN_ARROW && direction != UP_ARROW)
                direction = pressed;
            else if (pressed == UP_ARROW && direction != DOWN_ARROW)
                direction = pressed;
            else if (pressed == LEFT_ARROW && direction != RIGHT_ARROW)
                direction = pressed;
            else if (pressed == RIGHT_ARROW && direction != LEFT_ARROW)
                direction = pressed;
        }
    }
    return(direction);
}
