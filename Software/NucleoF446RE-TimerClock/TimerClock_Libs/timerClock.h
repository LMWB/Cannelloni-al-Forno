#ifndef TIMERCLOCK_H_
#define TIMERCLOCK_H_

#include <time.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_TIMER_SLOTS 	5
#define TIMER_SLOTS_1 		0
#define TIMER_SLOTS_2 		1
#define TIMER_SLOTS_3 		2
#define TIMER_SLOTS_4 		3
#define TIMER_SLOTS_5 		4

typedef struct _timer_clock{
	uint16_t 	start_in_minutes;
	uint16_t 	end_in_minutes;
} timer_clock_t;

typedef struct _recirculation_timer_clock{
	uint8_t 		number_of_active_timeslots;
	timer_clock_t 	slot_set_points[MAX_TIMER_SLOTS];
} recirculation_timer_clock_t;

/* later after proper testing this does not need to be global */
extern recirculation_timer_clock_t recirculation_timer_clock;

void set_recirculation_number_of_timeslots_active(uint8_t number_of_slot_active);
void set_recirculation_start(uint8_t slot, uint16_t start_in_minutes);
void set_recirculation_end(uint8_t slot, uint16_t start_in_minutes);
void run_recirculation_timer_clock();

uint16_t convert_gmtime_to_minuts(struct tm* time);
void print_current_time(struct tm* time);
void print_active_time_slots(void);

#endif
