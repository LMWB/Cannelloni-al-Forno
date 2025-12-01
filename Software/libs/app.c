#include "app.h"

#include "utils.h"
#include "timeProcessing.h"
#include "timerClock.h"
#include "noRTOS.h"

void app_main(void) {

	timerclock_set_number_of_active_timeslots(2);

	/* RTC is in UTC, so right now in winter time (MEZ = mitteleuropaeische normalzeit) it is 1h behind */
	/* from 5:00 until 7:30 */
	timerclock_set_start(TIMER_SLOTS_1,	4*60);
	timerclock_set_end( TIMER_SLOTS_1,	6*60+30);

	/* from 16:00 until 22:00 */
	timerclock_set_start(TIMER_SLOTS_2,	15*60);
	timerclock_set_end( TIMER_SLOTS_2,	21*60);

	myprintf("Starting timerclock and noRTOS Demo RELEASE/v2.0 \n");

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

	/* now I create some tasks and add them to the scheduler */

	// wenn reihenfolge nicht chronologisch,
	// gehts auch aber es entstehen interesannte Zeiteffekte
	// dokumentieren!!

	noRTOS_task_t uart_t = { .delay = eDELAY_10milli, .task_callback = uart_at_command_callback };
	noRTOS_add_task_to_scheduler(&uart_t);

	noRTOS_task_t print_time_now_t = { .delay = eDELAY_1s, .task_callback = print_time_now };
	noRTOS_add_task_to_scheduler(&print_time_now_t);

	noRTOS_task_t test_task4 = { .delay = eDELAY_1s, .task_callback = print_tick_time_stamp_diff };
	noRTOS_add_task_to_scheduler(&test_task4);

	noRTOS_task_t test_task3 = { .delay = eDELAY_10s, .task_callback = timerclock_run };
	noRTOS_add_task_to_scheduler(&test_task3);

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

