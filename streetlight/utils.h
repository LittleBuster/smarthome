#ifndef __UTILS_H__
#define __UTILS_H__

#define TIME_SIZE 15
#define DATE_SIZE 20
#define DATETIME_SIZE 30


/*
 * Getting now time string
 */
void time_now(char *time);

/*
 * Getting now date string
 */
void date_now(char *date);

/*
 * Getting now date and time string
 */
void date_time_now(char *date_time);


#endif
