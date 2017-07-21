#include <FreeRTOS.h>
#include <task.h>

#define LOG(...)

void vApplicationMallocFailedHook(void) {
	for (;;);
}

void vApplicationStackOverflowHook(TaskHandle_t xTask,char *pcTaskName) {
	for (;;);
}

__attribute__((used)) void Fault_HandlerC(uint32_t *hardfault_args,uint32_t orig_lr,uint32_t nvic_icsr)  {
	uint32_t stacked_r0;
	uint32_t stacked_r1;
	uint32_t stacked_r2;
	uint32_t stacked_r3;
	uint32_t stacked_r12;
	uint32_t stacked_lr;
	uint32_t stacked_pc;
	uint32_t stacked_psr;
	uint32_t vectornum;

	vectornum = nvic_icsr & 0x1FF;

	vTaskSuspendAll();
	taskDISABLE_INTERRUPTS();

	stacked_r0  = hardfault_args[0];
	stacked_r1  = hardfault_args[1];
	stacked_r2  = hardfault_args[2];
	stacked_r3  = hardfault_args[3];
	stacked_r12 = hardfault_args[4];
	stacked_lr  = hardfault_args[5];
	stacked_pc  = hardfault_args[6];
	stacked_psr = hardfault_args[7];

	// Temp warning-squasher until we get our logging going
	(void)stacked_r0;  (void)stacked_r1; (void)stacked_r2; (void)stacked_r3;
	(void)stacked_r12; (void)stacked_lr; (void)stacked_pc; (void)stacked_psr;
	(void)vectornum;

	LOG("FAULT VECTOR %d!!!",vectornum);
	LOG("r[0..3]   = %08x %08x %08x %08x", stacked_r0, stacked_r1, stacked_r2, stacked_r3);
	LOG("r12 lr pc = %08x %08x %08x %08x", stacked_r12, stacked_lr, stacked_pc);
	LOG("psr       = %08x", stacked_psr);

	LOG("Halting forever");
	while (1) {
	}
}

void HardFault_Handler(void) __attribute__((naked));
void HardFault_Handler(void) {
	__asm(".syntax unified\n"
		  "B Generic_Fault_Handler\n");
}

void UsageFault_Handler(void) __attribute__((naked));
void UsageFault_Handler(void) {
	__asm(".syntax unified\n"
		  "B Generic_Fault_Handler\n");
}

void BusFault_Handler(void) __attribute__((naked));
void BusFault_Handler(void) {
	__asm(".syntax unified\n"
		  "B Generic_Fault_Handler\n");
}

/* The prototype shows it is a naked function -
 * in effect this is just an assembly function.
 */
void Generic_Fault_Handler(void) __attribute__((naked));
void Generic_Fault_Handler(void)
{
	/* This snippet will fetch the vector# from the NVIC,
	   and figure out if the PSP or MSP is the stack we
	   should use. */
	__asm(".syntax unified\n"
		"LDR    R3,=#0xe000ed04 \n"  // Store IRQ vector in R2.
		"LDR    R2,[R3,#0]\n"
		"MOVS   R0, #4  \n"
		"MOV    R1, LR  \n"
		"TST    R0, R1  \n"
		"BEQ    _MSP    \n"
		"MRS    R0, PSP \n"  // Fault occured in userspace, so use PSP
		"B      Fault_HandlerC      \n"
		"_MSP:  \n"
		"MRS    R0, MSP \n"  // Fault occured in kernelspace, so use MSP
		"B      Fault_HandlerC      \n"
		".syntax divided\n");
}
