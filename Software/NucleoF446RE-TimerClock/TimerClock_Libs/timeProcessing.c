#include "timeProcessing.h"
#include "utils.h"
#include <string.h>
#include <stdint.h>

static const char MONTH_NAMES[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
static const char WEEK_NAMES[] = "SunMonTueWedThuFriSat";


bool is_new_day(uint16_t current_time_in_minutes){
	static uint16_t time = 0;
	bool return_value = false;
	if(current_time_in_minutes < time){
		return_value = true;
	}
	/* update the local time variable */
	time = current_time_in_minutes;
	return return_value;
}

time_t cvt_asctime(const char *linux_asctime_str, struct tm *time) {
	struct tm t = {0};
	int hour, minutes, seconds;
	int year, month, day;
	char buffer_day[5];
	char buffer_month[5];

	sscanf(linux_asctime_str, "%s %s %2d %2d:%2d:%2d %4d", buffer_day,
		   buffer_month, &day, &hour, &minutes, &seconds, &year);

	// Find where is s_month in month_names. Deduce month value.
	month = (strstr(MONTH_NAMES, buffer_month) - MONTH_NAMES) / 3;

	t.tm_year = year - 1900;
	t.tm_mon = month;
	t.tm_mday = day;

	t.tm_hour = hour;
	t.tm_min = minutes;
	t.tm_sec = seconds;

	// not that important
	t.tm_wday = (strstr(WEEK_NAMES, buffer_day) - WEEK_NAMES) / 3;

	/* return value for param 1 */
	time->tm_year = t.tm_year;
	time->tm_mon = t.tm_mon;
	time->tm_mday = t.tm_mday;
	time->tm_hour = t.tm_hour;
	time->tm_min = t.tm_min;
	time->tm_sec = t.tm_sec;
	time->tm_wday = t.tm_wday;

	/* return */
	return mktime(&t);
}

void convert_compiler_timestamp_to_asctime(char* time, char* date, char* asctime_string) {

	/*ESP32 Answer to SNTP Query: +CIPSNTPTIME:Tue Oct 19 15:17:56 2021 = Www Mmm dd hh:mm:ss yyyy\n*/
	char* compiler_time = time;
	char* compiler_date = date;

	/* Day of Week: 'Www ' -> always return 'Sun '*/
	asctime_string[0] = 'S';
	asctime_string[1] = 'u';
	asctime_string[2] = 'n';
	asctime_string[3] = ' ';
	/* Month: 'Mmm '*/
	asctime_string[4] = compiler_date[0];
	asctime_string[5] = compiler_date[1];
	asctime_string[6] = compiler_date[2];
	asctime_string[7] = compiler_date[3];
	/* Day: 'dd '*/
	asctime_string[8] = compiler_date[4];
	asctime_string[9] = compiler_date[5];
	asctime_string[10] = compiler_date[6];
	/* Hours: 'hh:'*/
	asctime_string[11] = compiler_time[0];
	asctime_string[12] = compiler_time[1];
	asctime_string[13] = compiler_time[2];
	/* Minuts: 'mm:'*/
	asctime_string[14] = compiler_time[3];
	asctime_string[15] = compiler_time[4];
	asctime_string[16] = compiler_time[5];
	/* Seconds: 'ss '*/
	asctime_string[17] = compiler_time[6];
	asctime_string[18] = compiler_time[7];
	asctime_string[19] = ' ';

	/* Year: 'yyyy\n'*/
	asctime_string[20] = compiler_date[7];
	asctime_string[21] = compiler_date[8];
	asctime_string[22] = compiler_date[9];
	asctime_string[23] = compiler_date[10];
	asctime_string[24] = '\n';

	/* Null terminator string*/
	asctime_string[25] = 0;

}

//uint16_t convert_gmtime_to_minuts(struct tm* time)
//{
//	return (time->tm_hour*60 + time->tm_min);
//}


struct tm *get_gmtime_stm32() {
	/* Reference: https://cplusplus.com/reference/ctime/tm/
	 *
	 * tm_sec	int	seconds after the minute	0-60*
	 * tm_min	int	minutes after the hour		0-59
	 * tm_hour	int	hours since midnight		0-23
	 * tm_mday	int	day of the month			1-31
	 * tm_mon	int	months since January		0-11
	 * tm_year	int	years since 				1900
	 * tm_wday	int	days since Sunday			0-6
	 * tm_yday	int	days since January 1		0-365
	 * tm_isdst	int	Daylight Saving Time flag
	 *
	 * tm_sec is generally 0-59. The extra range is to accommodate for leap seconds in certain systems.
	 * */

	static struct tm ts;

	/* how to check for RTC is working */
	if (true) {
		RTC_TimeTypeDef current_time = {0};
		RTC_DateTypeDef current_date = {0};

		GET_DEVICE_TIME(&current_time);
		GET_DEVICE_DATE(&current_date);

		ts.tm_year = current_date.Year + 100;
		ts.tm_mday = current_date.Date;
		ts.tm_mon = current_date.Month - 1;
		ts.tm_hour = current_time.Hours;
		ts.tm_min = current_time.Minutes;
		ts.tm_sec = current_time.Seconds;
	} else {
		// default timestamp
		ts.tm_year = 123;
		ts.tm_mday = 1;
		ts.tm_mon = 0;
		ts.tm_hour = 0;
		ts.tm_min = 0;
		ts.tm_sec = 0;
	}

	return &ts;
}

time_t get_epoch_time(void) {
	time_t timestamp = 0;
	struct tm *ptm;

	ptm = get_gmtime_stm32();
	timestamp = mktime(ptm);

	return timestamp;
}

uint16_t convert_gmtime_to_minuts(struct tm* time)
{
	return (time->tm_hour*60 + time->tm_min);
}

uint8_t change_controller_time(struct tm *time) {
	RTC_TimeTypeDef s_time = {0};
	RTC_DateTypeDef s_date = {0};
	s_time.Hours = time->tm_hour;
	s_time.Minutes = time->tm_min;
	s_time.Seconds = time->tm_sec;
	s_time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	s_time.StoreOperation = RTC_STOREOPERATION_RESET;
	if (SET_DEVICE_TIME(&s_time) != DEVICE_OK) {
		Error_Handler();
	}
	s_date.WeekDay = time->tm_wday;
	s_date.Month = time->tm_mon + 1;
	s_date.Date = time->tm_mday;
	s_date.Year = time->tm_year - 100;
	if (SET_DEVICE_DATE(&s_date) != DEVICE_OK) {
		Error_Handler();
	}

	return 1;
}


/* not relevant here since we got no internet module */
//void synchronize_controller_clock_with_internet(void) {
//	/* if Connection to Internet is established, get timestamp from time server */
//	char asc_time_string[26] = {0};
//	struct tm timedate = {0};
//	(void) get_atom_clock_time(asc_time_string);
//
//	/* set RTC on target MCU */
//	(void) cvt_asctime(asc_time_string, &timedate);
//	(void) change_controller_time(&timedate);
//	myprintf("Smart hub time synchronized with the internet!\n");
//}
