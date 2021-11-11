#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include "logger.h"

#define RESET 0
#define colorFondoNegro 40

void setColor(int attr, int fg, int bg);
void setDefaultColor();

void setColor(int attr, int fg, int bg)
{
    char command[13];
    sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg, bg);
    printf("%s", command);
}

void setDefaultColor()
{
    char command[13];
    sprintf(command, "%c[%d;0;0m", 0x1B, RESET);
    printf("%s", command);
}

int isSyslog = 0;

int initLogger(char *logType)
{
    printf("Initializing Logger on: %s\n", logType);
    if (strcmp(logType, "syslog") == 0)
    {
        isSyslog = 1;
    }
    else if (strcmp(logType, "stdout") == 0 || strlen(logType) == 0)
    {
        isSyslog = 0;
    }
    else
    {
        printf("ERROR: Invalid arguments");
    }

    return 0;
}

int infof(const char *format, ...)
{
    va_list arg;
    va_start(arg, format);
    setColor(RESET, 36, colorFondoNegro); //color

    if (isSyslog == 1)
    {
        vsyslog(LOG_INFO, format, arg);
        closelog();
    }
    else if (isSyslog == 0)
    {
        vprintf(format, arg);
    }

    va_end(arg);
    setDefaultColor();
    return 0;
}

int warnf(const char *format, ...)
{
    va_list arg;
    va_start(arg, format);
    setColor(RESET, 35, colorFondoNegro); //color

    if (isSyslog == 1)
    {
        vsyslog(LOG_INFO, format, arg);
        closelog();
    }
    else if (isSyslog == 0)
    {
        vprintf(format, arg);
    }

    va_end(arg);
    setDefaultColor();
    printf("\n");
    return 0;
}

int errorf(const char *format, ...)
{
    va_list arg;
    va_start(arg, format);
    setColor(RESET, 31, colorFondoNegro); //color

    if (isSyslog == 1)
    {
        vsyslog(LOG_INFO, format, arg);
        closelog();
    }
    else if (isSyslog == 0)
    {
        vprintf(format, arg);
    }

    va_end(arg);
    setDefaultColor();
    printf("\n");
    return 0;
}

int panicf(const char *format, ...)
{
    va_list arg;
    va_start(arg, format);
    setColor(RESET, 34, colorFondoNegro); //color

    if (isSyslog == 1)
    {
        vsyslog(LOG_INFO, format, arg);
        closelog();
    }
    else if (isSyslog == 0)
    {
        vprintf(format, arg);
    }

    va_end(arg);
    setDefaultColor();
    printf("\n");
    return 0;
}