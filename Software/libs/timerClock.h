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

typedef enum{eOFF, eON, eUNDEFINED} timerClockStates_t;

/* basic function pointer prototype with void return and void arguments */
typedef void (*timerClockFP_t)(void);

typedef struct _timer_slot{
	uint16_t 	start_in_minutes;
	uint16_t 	end_in_minutes;
} timerClockSlot_t;

typedef struct _timer_clock{
	uint8_t 			number_of_active_timeslots;
	timerClockSlot_t 	slot_set_points[MAX_TIMER_SLOTS];
	timerClockStates_t 	state;
	timerClockFP_t 		on;
	timerClockFP_t 		off;
} timer_clock_t;

/* timer clock API */
void timerclock_print_active_time_slots(		timer_clock_t* TC);
void timerclock_set_number_of_active_timeslots(	timer_clock_t* TC, uint8_t number_of_slot_active);
void timerclock_set_start(						timer_clock_t* TC, uint8_t slot, uint16_t start_in_minutes);
void timerclock_set_end(						timer_clock_t* TC, uint8_t slot, uint16_t start_in_minutes);
void timerclock_run(							timer_clock_t* TC);

/* utils */
void print_current_time(struct tm* time);
#endif
