#include <noRTOS.h>

noRTOS_task_t *global_list_of_tasks[NORTOS_NO_OF_MAX_TASK];
static uint32_t number_of_active_task = 0;

bool noRTOS_add_task_to_scheduler(noRTOS_task_t *task){
	if(number_of_active_task < NORTOS_NO_OF_MAX_TASK){
		global_list_of_tasks[number_of_active_task] = task;
		number_of_active_task++;
		return true;
	}
	return false;
}

void noRTOS_run_schedular(void) {
	while(1){
		uint32_t now = NORTOS_SCHEDULAR_GET_TICK();

		for(uint_fast32_t i = 0; i < number_of_active_task; i++){
			if( (now - global_list_of_tasks[i]->tick) >= global_list_of_tasks[i]->delay){
				global_list_of_tasks[i]->tick = now;
				global_list_of_tasks[i]->task_callback();
			}
		}
	}
}
