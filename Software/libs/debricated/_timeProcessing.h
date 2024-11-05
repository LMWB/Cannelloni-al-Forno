#ifndef SMARTHUB100APPLICATION_TIMEPROCESSING_H
#define SMARTHUB100APPLICATION_TIMEPROCESSING_H

#include <time.h>
#include <stdbool.h>


#define NO_OF_TIMER_CLOCK_SLOTS 4
#define NO_OF_ACTIVE_TIMER_CLOCKS 4

typedef struct __timer_clock{
	uint16_t slot_in_minutes;
	bool state_of_output;
	bool slot_is_outdated;
} timer_clock_old_t;
extern timer_clock_old_t timer_clock_1[NO_OF_TIMER_CLOCK_SLOTS];

//uint16_t convert_gmtime_to_minuts(struct tm* time);
//void print_curretn_time(struct tm* time);
void reset_all_timer_clock_slots(void);
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



#endif //SMARTHUB100APPLICATION_TIMEPROCESSING_H
