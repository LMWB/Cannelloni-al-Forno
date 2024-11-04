#ifndef SIMPLESCHEDULER_H_
#define SIMPLESCHEDULER_H_

#include <stdint.h>
#include <stdbool.h>

#define SCHEDULAR_GET_TICK() 		HAL_GetTick()
#define SCHEDULAR_MAX_SIMPLE_TASK	128

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
} simple_task_t;

extern simple_task_t *simple_tasks[SCHEDULAR_MAX_SIMPLE_TASK];

bool add_task_to_scheduler(simple_task_t *task);

void run_schedular(void);

#endif /* SIMPLESCHEDULER_H_ */
