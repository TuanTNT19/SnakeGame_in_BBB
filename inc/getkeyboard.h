#pragma once

#ifndef GETKEYBOARD_H
#define GETKEYBOARD_H

#include "ssd1306.h"

#define UP_ARROW  'A'
#define LEFT_ARROW  'D'
#define RIGHT_ARROW  'C'
#define DOWN_ARROW 'B'
#define ENTER_KEY 10

int KEYBOARD_CheckKey_Press();
char KEYBOARD_Key_Press();
char KEYBOARD_waitForAnyKey();
int KEYBOARD_KeyPress_to_Direction(int direction);

#endif