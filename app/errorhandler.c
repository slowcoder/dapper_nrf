#include <FreeRTOS.h>
#include <task.h>

void vApplicationMallocFailedHook(void) {
	for (;;);
}

void vApplicationStackOverflowHook(TaskHandle_t xTask,char *pcTaskName) {
	for (;;);
}
