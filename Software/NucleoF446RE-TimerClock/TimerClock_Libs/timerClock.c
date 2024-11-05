#include "timerClock.h"
#include "utils.h"
#include "timeProcessing.h"

timer_clock_m_t recirculation_timer_clock	= {0};

int8_t daylight_saving_time_minutes_offset = 0;

// simple wrapper to skip passing arguments to a function pointer
void on(void){
	NUCLEO_LED1_ON();
}

void off(void){
	NUCLEO_LED1_OFF();
}

typedef enum{OFF, ON, UNDEFINED} timerClockStates_t;

void drive_outputs(timerClockStates_t state){
	if(state == ON){
		on();
	}else{
		off();
	}
}

bool is_current_time_in_timeslot(uint16_t now_in_minutes, uint16_t start_in_minutes, uint16_t end_in_minutes){
	if(now_in_minutes >= start_in_minutes && now_in_minutes < end_in_minutes){
		return true;
	}else{
		return false;
	}
}

bool is_current_time_in_any_active_timeslots(uint16_t now_in_minutes){
	uint8_t active_timeslots = recirculation_timer_clock.number_of_active_timeslots;
	for (uint_fast8_t i = 0; i < active_timeslots; ++i) {
		if(is_current_time_in_timeslot(
				now_in_minutes,
				recirculation_timer_clock.slot_set_points[i].start_in_minutes,
				recirculation_timer_clock.slot_set_points[i].end_in_minutes)){
			return true;
		}
	}
	/* important: only return "false" at the very end of for-loop */
	return false;
}

void print_current_time(struct tm* time) {
	myprintf("Current Time: %02d : %02d : %02d (UTC) = %d min \n",
			time->tm_hour,
			time->tm_min,
			time->tm_sec,
			time->tm_hour*60 + time->tm_min);
}

void print_active_time_slots(void) {
	int8_t correction_dls = daylight_saving_time_minutes_offset;
	uint8_t active_timeslots = recirculation_timer_clock.number_of_active_timeslots;
	myprintf("Programmed Time Slots:\n");
	for (uint_fast8_t i = 0; i < active_timeslots; i++) {
		myprintf("[Slot %d] Start: %d End: %d (Minutes from Midnight UTC)\n",
				i + 1,
				recirculation_timer_clock.slot_set_points[i].start_in_minutes,
				recirculation_timer_clock.slot_set_points[i].end_in_minutes);
	}
	myprintf("Day Light Saving set to %d minutes\n", correction_dls);
}

void timerclock_set_number_of_active_timeslots(uint8_t number_of_slot_active){
	if(number_of_slot_active <= MAX_TIMER_SLOTS){
		recirculation_timer_clock.number_of_active_timeslots = number_of_slot_active;
	}
}

void timerclock_set_start(uint8_t slot, uint16_t start_in_minutes){
	if(slot <= MAX_TIMER_SLOTS){
		recirculation_timer_clock.slot_set_points[slot].start_in_minutes = start_in_minutes;
	}
}
void timerclock_set_end(uint8_t slot, uint16_t end_in_minutes){
	if(slot <= MAX_TIMER_SLOTS){
		recirculation_timer_clock.slot_set_points[slot].end_in_minutes = end_in_minutes;
	}
}

void timerclock_run(){
	static timerClockStates_t state = UNDEFINED;
	timerClockStates_t new_state 	= OFF;		/* initiates this variable to OFF ensures the output must be driven at first call of this function */

	struct tm *curren_Date_Time 		= get_gmtime_stm32();
	uint16_t current_time_in_minuts 	= convert_gmtime_to_minuts(curren_Date_Time);
	int8_t correction_day_light_saving 	= daylight_saving_time_minutes_offset;
	uint16_t now 						= current_time_in_minuts + correction_day_light_saving;

	print_current_time(curren_Date_Time);
	print_active_time_slots();
	myprintf("\n");

	if(is_current_time_in_any_active_timeslots(now)){
		new_state = ON;
	}

	/* only write outputs once and once only on when they are off and off when the ar on */
	if(new_state != state){
		drive_outputs(new_state);
		state = new_state;
	}
}
