#include "app.h"

#include "utils.h"
#include "timeProcessing.h"
#include "timerClock.h"
#include "noRTOS.h"

// simple wrapper to attache system specific on?off functions
void star_on(void){
	LD293D_TURN_ON_OUT1();
}

void star_off(void){
	LD293D_TURN_OFF_OUT1();
}

void lamp_on(void){
	LD293D_TURN_ON_OUT2();
}

void lamp_off(void){
	LD293D_TURN_OFF_OUT2();
}

void app_main(void) {
	/* create two, or more, instances of timerclock Class */
	timer_clock_t timer_clock_Star = {0};
	timer_clock_t timer_clock_Lamp = {0};

	/* todo: rewrite the timer_clock code with
	 * - function pointer to on() - done
	 * - function pointer to off() - done
	 * - timer slots memory - ????
	 * - timer slots profile (for summer vs. winter) - done
	 * - remove myprintf() -
	 * - refactor timer_clock_lip to not have printf()
	 *
	 * */

	/* map output driver functions to timer_clock instance */
	timer_clock_Star.on = star_on;
	timer_clock_Star.off = star_off;

	timer_clock_Lamp.on = lamp_on;
	timer_clock_Lamp.off = lamp_off;


	/* this pin has internal pull up, to switch it, draw it to GND e.g. with jumper wire
	 * so default (without jumper wire) is high hans winter-time-mode */
	bool is_winter_time = READ_PIN(Winter_Summer_GPIO_Port, Winter_Summer_Pin);
	/* profile for winter time, short daylight, long night time */
	if (is_winter_time) {
		timerclock_set_number_of_active_timeslots(&timer_clock_Star, 2);
		/* RTC is in UTC, so right now in winter time (MEZ = mitteleuropaeische normalzeit) it is 1h behind */
		/* from 5:00 until 7:30 */
		timerclock_set_start(	&timer_clock_Star, TIMER_SLOTS_1, 4 * 60);
		timerclock_set_end(		&timer_clock_Star, TIMER_SLOTS_1, 6 * 60 + 30);

		/* from 16:00 until 22:00 */
		timerclock_set_start(	&timer_clock_Star, TIMER_SLOTS_2, 15 * 60);
		timerclock_set_end(		&timer_clock_Star, TIMER_SLOTS_2, 21 * 60);
	}
	/* profile for summer time, short night time, long daylight */
	else {
		timerclock_set_number_of_active_timeslots(&timer_clock_Star, 1);
		/* from 19:00 until 23:00 */
		timerclock_set_start(	&timer_clock_Star, TIMER_SLOTS_1, 18 * 60);
		timerclock_set_end(		&timer_clock_Star, TIMER_SLOTS_1, 22 * 60);
	}

	/* just to have a 2nd light source to have the app multi instance */
	/* from 17:00 until 24:00 */
	timerclock_set_number_of_active_timeslots(&timer_clock_Lamp, 1);
	timerclock_set_start(	&timer_clock_Lamp, TIMER_SLOTS_1, 18*60);
	timerclock_set_end(		&timer_clock_Lamp, TIMER_SLOTS_1, 23*60);

	myprintf("Starting timerclock and noRTOS Demo\n");

	HAL_Delay(200);
	struct tm *curren_Date_Time = get_gmtime_stm32();
	print_current_time(curren_Date_Time);

	/* activate UART Interrupt based receive (byte wise) */
	noRTOS_UART2_read_byte_with_interrupt();

	LD293D_ENABLE_OUT1_OUT2();
	LD293D_ENABLE_OUT3_OUT4();

	void print_tick_time_stamp_diff(void){
		static uint32_t time_stamp_last_call = 0;
		uint32_t now = NORTOS_SCHEDULAR_GET_TICK();
		printf("differnce from now to previous call is %ld ms\n", (now-time_stamp_last_call));
		time_stamp_last_call = now;
	}

	void print_time_now(void) {
		struct tm *curren_Date_Time = get_gmtime_stm32();
		uint32_t epochtime = (uint32_t) convert_tm_struct_to_epoch_time(curren_Date_Time);

		printf("current time (utc),%02d:%02d:%02d,", curren_Date_Time->tm_hour, curren_Date_Time->tm_min, curren_Date_Time->tm_sec);
		printf("epochtime,%ld\n", epochtime);
	}

	void uart_at_command_callback(void) {
		if( noRTOS_is_UART2_read_line_complete() ) {
			char* at_command = strstr( (char*)uart2_buffer, "AT+SETRTC=");
			if( at_command ){
				char asctime_string[26];
				struct tm timedate = { 0 };
				char *time = at_command+10;
				char *date = at_command+19;
				convert_compiler_timestamp_to_asctime(time, date, asctime_string);
				convert_asctime_to_tm_struct(asctime_string, &timedate);
				(void) change_controller_time(&timedate);
			}else{
				noRTOS_UART2_echo_whats_been_received();
			}
			noRTOS_UART2_clear_rx_buffer();
		}
	}

	void run1(void){
		timerclock_run( &timer_clock_Star );
	}

	void run2(void){
		timerclock_run( &timer_clock_Lamp );
	}

	/* now I create some tasks and add them to the scheduler */

	// wenn reihenfolge nicht chronologisch,
	// gehts auch aber es entstehen interesannte Zeiteffekte
	// dokumentieren!!

	noRTOS_task_t uart_t = { .delay = eDELAY_10milli, .task_callback = uart_at_command_callback };
	noRTOS_add_task_to_scheduler(&uart_t);

	noRTOS_task_t print_time_now_t = { .delay = eDELAY_1s, .task_callback = print_time_now };
	noRTOS_add_task_to_scheduler(&print_time_now_t);

	noRTOS_task_t exe_time = { .delay = eDELAY_1s, .task_callback = print_tick_time_stamp_diff };
	noRTOS_add_task_to_scheduler(&exe_time);

	/* in case the function that needs to be run is not of type void foo(void)
	 * just create a simple wrapper that manages parameter handling */
	noRTOS_task_t timerClock1 = { .delay = eDELAY_10s, .task_callback = run1 };
	noRTOS_add_task_to_scheduler(&timerClock1);

	noRTOS_task_t timerClock2 = { .delay = eDELAY_10s, .task_callback = run2 };
	noRTOS_add_task_to_scheduler(&timerClock2);

	/* this runs for ever */
	noRTOS_run_schedular();

	/* never get here! */
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if (GPIO_Pin == B1_Pin) {
		LD293D_TOGGLE_OUT1();
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

	if (huart->Instance == USART2){
		noRTOS_UART2_receive_byte_callback();
	}
}

