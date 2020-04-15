#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <signal.h>
#include "logger.h"



int infof(const char *format, ...) {
    va_list text;
    va_start(text, format);
    textcolor(BRIGHT, GREEN, BLACK);
    int ok = vfprintf(stdout, format, text);
    textcolor(RESET, WHITE, BLACK);
    va_end(text);
    return ok;
}

int warnf(const char *format, ...) {
    va_list text;
    va_start(text, format);
    textcolor(BRIGHT, YELLOW, BLACK);
    int ok = vfprintf(stdout, format, text);
    textcolor(RESET, WHITE, BLACK);
    va_end(text);
    return ok;
}

int errorf(const char *format, ...) {
    va_list text;
    va_start(text, format);
    textcolor(BRIGHT, MAGENTA, BLACK);
    int ok = vfprintf(stdout, format, text);
    textcolor(RESET, WHITE, BLACK);
    va_end(text);
    return ok;
}
int panicf(const char *format, ...) {
    va_list text;
    va_start(text, format);
    textcolor(BRIGHT, RED, BLACK);
    int ok = vfprintf(stdout, format, text);
    textcolor(RESET, WHITE, BLACK);
    va_end(text);
    raise (SIGABRT);
    return ok;
}

void textcolor(int attr, int fg, int bg)
{	char command[13];
	sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
	printf("%s", command);
}


