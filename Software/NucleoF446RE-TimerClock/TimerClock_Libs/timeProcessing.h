#ifndef TIMEPROCESSING_H
#define TIMEPROCESSING_H

#include <time.h>
#include <stdbool.h>


bool is_new_day(uint16_t current_time_in_minutes);

/**
 * Uses the value pointed by STM32 RTC timer to fill a tm structure with the values
 * that represent the corresponding time, expressed as a UTC time
 * (i.e., the time at the GMT timezone).
 */
struct tm *get_gmtime_stm32();

/**
 * @brief  Convert the STM32 HAL RTC timestamp structure into unix epoch time in seconds from Jan 1st 1900 00:00:00
 * @param  non
 * @retval SECONDS since epoch time 1970 Jan 1st 00:00:00
 */
time_t get_epoch_time(void);

/**
 * @brief  Convert the unix epoch time string e.g. "Mar 23 15:19:26 2023"
 * 		to unix epoch time in seconds from Jan 1st 1900 00:00:00
 * 		e.g. python time.asctime()
 * @param  pointer to char array (string) asctime() String e.g. "Mar 23 15:19:26 2023".
 * @param  pointer to struct tm from time.h for application use on exact time/date Format
 * @retval time_t Unix Epoch Time
 */
time_t cvt_asctime(const char *linux_asctime_str, struct tm *time);

/**
 * @brief 	set the Microcontrollers Real Time Clock
 * 			implementation is vendor specific
 * 			here we use STM32
 */
uint8_t change_controller_time(struct tm *time);

/* */
void convert_compiler_timestamp_to_asctime(char* time, char* date, char* asctime_string);

#endif
