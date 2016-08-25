#include "utils.h"
#include <string.h>
#include <time.h>


void time_now(char *time_str)
{
    time_t t = time(NULL);
    struct tm *timeinfo;

    timeinfo = localtime (&t);
    strftime(time_str, TIME_SIZE, "%T", timeinfo);
}

void date_now(char *date_str)
{
    time_t t = time(NULL);
    struct tm *timeinfo;

    timeinfo = localtime (&t);
    strftime(date_str, DATE_SIZE, "%F", timeinfo);
}

void date_time_now(char *date_time)
{
    char time_str[TIME_SIZE];
    char date_str[DATE_SIZE];

    time_t t = time(NULL);
    struct tm *timeinfo;

    timeinfo = localtime (&t);
    strftime(date_str, TIME_SIZE, "%F", timeinfo);
    strftime(time_str, DATE_SIZE, "%T", timeinfo);

    strcpy(date_time, date_str);
    strcat(date_time, " ");
    strcat(date_time, time_str);
}
