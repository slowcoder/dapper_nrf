#include <stdint.h>
#include <nrf_mbr.h>
#include <nrf_log.h>
#include <nrf_log_ctrl.h>
#include <app_error.h>
#include <app_error_weak.h>
#include <nrf_bootloader_info.h>

extern uint32_t * __isr_vector;

volatile uint32_t m_uicr_bootloader_start_address  __attribute__ ((section(".uicrBootStartAddress")))
                                            = (uint32_t)&__isr_vector;

void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info) {
	NRF_LOG_ERROR("received a fault! id: 0x%08x, pc: 0x%08x\r\n", id, pc);
	NVIC_SystemReset();
}

void app_error_handler_bare(uint32_t error_code) {
    (void)error_code;

	NRF_LOG_ERROR("received an error: 0x%08x!\r\n", error_code);
	NVIC_SystemReset();
}

static void __attribute__ ((noinline)) app_start_impl(uint32_t start_addr) {
	__ASM volatile(
		"ldr   r0, [%0]\t\n"            // Get App initial MSP for bootloader.
		"msr   msp, r0\t\n"             // Set the main stack pointer to the applications MSP.
		"ldr   r0, [%0, #0x04]\t\n"     // Load Reset handler into R0.

		"movs  r4, #0xFF\t\n"           // Move ones to R4.
		"sxtb  r4, r4\t\n"              // Sign extend R4 to obtain 0xFFFFFFFF instead of 0xFF.

		"mrs   r5, IPSR\t\n"            // Load IPSR to R5 to check for handler or thread mode.
		"cmp   r5, #0x00\t\n"           // Compare, if 0 then we are in thread mode and can continue to reset handler of bootloader.
		"bne   isr_abort\t\n"           // If not zero we need to exit current ISR and jump to reset handler of bootloader.

		"mov   lr, r4\t\n"              // Clear the link register and set to ones to ensure no return.
		"bx    r0\t\n"                  // Branch to reset handler of bootloader.

		"isr_abort:  \t\n"

		"mov   r5, r4\t\n"              // Fill with ones before jumping to reset handling. Will be popped as LR when exiting ISR. Ensures no return to application.
		"mov   r6, r0\t\n"              // Move address of reset handler to R6. Will be popped as PC when exiting ISR. Ensures the reset handler will be executed when exist ISR.
		"movs  r7, #0x21\t\n"           // Move MSB reset value of xPSR to R7. Will be popped as xPSR when exiting ISR. xPSR is 0x21000000 thus MSB is 0x21.
		"rev   r7, r7\t\n"              // Reverse byte order to put 0x21 as MSB.
		"push  {r4-r7}\t\n"             // Push everything to new stack to allow interrupt handler to fetch it on exiting the ISR.

		"movs  r4, #0x00\t\n"           // Fill with zeros before jumping to reset handling. We be popped as R0 when exiting ISR (Cleaning up of the registers).
		"movs  r5, #0x00\t\n"           // Fill with zeros before jumping to reset handling. We be popped as R1 when exiting ISR (Cleaning up of the registers).
		"movs  r6, #0x00\t\n"           // Fill with zeros before jumping to reset handling. We be popped as R2 when exiting ISR (Cleaning up of the registers).
		"movs  r7, #0x00\t\n"           // Fill with zeros before jumping to reset handling. We be popped as R3 when exiting ISR (Cleaning up of the registers).
		"push  {r4-r7}\t\n"             // Push zeros (R4-R7) to stack to prepare for exiting the interrupt routine.

		"movs  r0, #0xF9\t\n"           // Move the execution return command into register, 0xFFFFFFF9.
		"sxtb  r0, r0\t\n"              // Sign extend R0 to obtain 0xFFFFFFF9 instead of 0xF9.
		"bx    r0\t\n"                  // No return - Handler mode will be exited. Stack will be popped and execution will continue in reset handler initializing other application.
		".align\t\n"
		:: "r" (start_addr)             // Argument list for the gcc assembly. start_addr is %0.
		:  "r0", "r4", "r5", "r6", "r7" // List of register maintained manually.
	);
}

#ifdef BLE_STACK_SUPPORT_REQD
static uint32_t nrf_dfu_mbr_init_sd(void) {
	uint32_t ret_val;

	sd_mbr_command_t command = {
		.command = SD_MBR_COMMAND_INIT_SD
	};

	ret_val = sd_mbr_command(&command);

	return ret_val;
}
#endif

static void app_start(uint32_t start_addr) {

#ifdef BLE_STACK_SUPPORT_REQD
	if(nrf_dfu_mbr_init_sd() != NRF_SUCCESS) {
		NRF_LOG_INFO("Failed: nrf_dfu_mbr_init_sd\r\n");
		return;
	}
#endif

	// Disable interrupts
	NVIC->ICER[0]=0xFFFFFFFF;
#if defined(__NRF_NVIC_ISER_COUNT) && __NRF_NVIC_ISER_COUNT == 2
	NVIC->ICER[1]=0xFFFFFFFF;
#endif

#ifdef BLE_STACK_SUPPORT_REQD
	// Set the sd softdevice vector table base address
	if(sd_softdevice_vector_table_base_set(start_addr) != NRF_SUCCESS) {
		NRF_LOG_INFO("Failed running sd_softdevice_vector_table_base_set\r\n");
		return;
	}
#endif

	// Run application
	app_start_impl(start_addr);
}

int main(void) {

	(void) NRF_LOG_INIT(NULL);

	NRF_LOG_INFO("Entering bootloader\r\n");

	app_start(MAIN_APPLICATION_START_ADDR);

	// If the app returns, something magically wrong happened..
	app_error_handler_bare(~0);

	return 0;
}
