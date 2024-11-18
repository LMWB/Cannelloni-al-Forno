#include "app.h"

#include "utils.h"
#include "timeProcessing.h"
#include "timerClock.h"
#include "noRTOS.h"

void app_main(void) {

	timerclock_set_number_of_active_timeslots(3);

	/* from 5:00 until 9:00 */
	timerclock_set_start(TIMER_SLOTS_1, 5 * 60);
	timerclock_set_end( TIMER_SLOTS_1, 9 * 60);

	/* from 14:26 until 15:00 */
	timerclock_set_start(TIMER_SLOTS_2, 14 * 60 + 26);
	timerclock_set_end( TIMER_SLOTS_2, 15 * 60);

	/* from 16:00 until 23:00 */
	timerclock_set_start(TIMER_SLOTS_3, 16 * 60);
	timerclock_set_end( TIMER_SLOTS_3, 23 * 60);

	myprintf("Starting timerclock and noRTOS Demo\n");

	HAL_Delay(200);
	struct tm *curren_Date_Time = get_gmtime_stm32();
	print_current_time(curren_Date_Time);

	/* activate UART Interrupt based receive (byte wise) */
	noRTOS_UART2_read_byte_with_interrupt();

	LD293D_ENABLE_OUT1_OUT2();
	LD293D_ENABLE_OUT3_OUT4();

	void test_callback(void) {
		myprintf("Hello World Task 1\n");
	}

	void test_callback2(void) {
		myprintf("\tHello World Task 2\n");
	}

	void test_callback3(void) {
		timerclock_run();
	}

	void test_callback4(void) {
		if( noRTOS_is_UART2_read_line_complete() ) {
			char* at_command = strstr( (char*)uart2_buffer, "AT+SETRTC:");
			if( at_command ){
				char timestampe_string[26];
				struct tm timedate = { 0 };
				char *time = at_command+10;
				char *date = at_command+18;
				convert_compiler_timestamp_to_asctime(time, date, timestampe_string);
				cvt_asctime(timestampe_string, &timedate);
				(void) change_controller_time(&timedate);

				noRTOS_UART2_clear_rx_buffer();
			}else{
				noRTOS_UART2_echo_whats_been_received();
				noRTOS_UART2_clear_rx_buffer();
			}
		}
	}

	noRTOS_task_t test_task = { .delay = eDELAY_1s, .task_callback = test_callback };
	noRTOS_add_task_to_scheduler(&test_task);

	noRTOS_task_t test_task2 = { .delay = eDELAY_5s, .task_callback = test_callback2 };
	noRTOS_add_task_to_scheduler(&test_task2);

	noRTOS_task_t test_task3 = { .delay = eDELAY_10s, .task_callback = test_callback3 };
	noRTOS_add_task_to_scheduler(&test_task3);

	noRTOS_task_t test_task4 = { .delay = eDELAY_10milli, .task_callback = test_callback4 };
	noRTOS_add_task_to_scheduler(&test_task4);

	/* runs for ever */
	noRTOS_run_schedular();

	/* never get here */
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

