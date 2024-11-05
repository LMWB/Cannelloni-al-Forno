#include "simpleScheduler.h"
#include "stm32f4xx.h"


simple_task_t *simple_tasks[SCHEDULAR_MAX_SIMPLE_TASK];
static uint32_t number_of_active_task = 0;

bool add_task_to_scheduler(simple_task_t *task){
	if(number_of_active_task < SCHEDULAR_MAX_SIMPLE_TASK){
		simple_tasks[number_of_active_task] = task;
		number_of_active_task++;
		return true;
	}
	return false;
}

void run_schedular(void) {
	while(1){
		uint32_t now = SCHEDULAR_GET_TICK();

		for(uint_fast32_t i = 0; i < number_of_active_task; i++){
			if( (now - simple_tasks[i]->tick) >= simple_tasks[i]->delay){
				simple_tasks[i]->tick = now;
				simple_tasks[i]->task_callback();
			}
		}
	}
}
