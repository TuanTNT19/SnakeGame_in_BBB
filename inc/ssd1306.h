#pragma once

#ifndef SSD1306_H
#define SSD1306_H

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

//Linux Libraries
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int OLED_OpenDevFile();
void OLED_SetCursor(int fd, int x, int y);
void OLED_Display(int fd, char *str);
void OLED_Clear(int fd);

#endif