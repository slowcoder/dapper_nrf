#include <stdint.h>
#include <string.h>
#include <nrf.h>
#include <app_error.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

#include <nrf_gpio.h>
#include <nrf_drv_clock.h>

static TaskHandle_t thFirstThread;

static void reset_gpios(void) {
	int pin;

	for(pin=0;pin<32;pin++) {
		nrf_gpio_cfg(pin,
			NRF_GPIO_PIN_DIR_INPUT,
			NRF_GPIO_PIN_INPUT_DISCONNECT,
			NRF_GPIO_PIN_NOPULL,
			NRF_GPIO_PIN_D0S1,
			NRF_GPIO_PIN_NOSENSE);
	}
}

static void first_thread(void *arg) {

	reset_gpios();

	for(;;) {
		vTaskDelay(100);
	}
}

int main(void) {
	ret_code_t err_code;

	err_code = nrf_drv_clock_init();
	APP_ERROR_CHECK(err_code);

	if( xTaskCreate(first_thread, "main", 256, NULL, 1, &thFirstThread) != pdPASS ) {
		APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
	}

	/* Activate deep sleep mode */
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

	// Start FreeRTOS scheduler.
	vTaskStartScheduler();

	// The vTaskStartScheduler function should _never_ return
	while (1) {
		APP_ERROR_HANDLER(NRF_ERROR_FORBIDDEN);
	}

	return 0;
}
