#ifndef NORTOS_H_
#define NORTOS_H_

#include <stdint.h>
#include <stdbool.h>
#include "main.h"

#define NORTOS_SCHEDULAR_GET_TICK()	HAL_GetTick() // returns tick in milliseconds
#define NORTOS_NO_OF_MAX_TASK		128

typedef void(*VoidFunctionPointer)(void);

typedef enum {
	eDELAY_1s = 1 * 1000,
	eDELAY_5s = 5 * 1000,
	eDELAY_10s = 10 * 1000,
	eDELAY_30s = 30 * 1000,
	eDELAY_1m = 60 * 1000,
	eDELAY_10m = 10 * 60 * 1000,
	eDELAY_1h = 60 * 60 * 1000,
} task_delay_e;

typedef struct {
	uint32_t tick;
	task_delay_e delay;
	VoidFunctionPointer task_callback;
} noRTOS_task_t;

extern noRTOS_task_t *global_list_of_tasks[NORTOS_NO_OF_MAX_TASK];

bool noRTOS_add_task_to_scheduler(noRTOS_task_t *task);
void noRTOS_run_schedular(void);

#endif /* NORTOS_H_ */
