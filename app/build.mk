
APP_SRCS := \
	app/main.c \
	app/errorhandler.c

APP_SRCS += \
	$(SDK_SRCS) \
	$(SDK_SRCS_NRF52) \
	$(SDK_SRCS_BLE) \
	$(SDK_SRCS_FREERTOS) \
	$(SDK_SRCS_FREERTOS_NRF52) \
	SDK-13.0.0/components/drivers_nrf/uart/nrf_drv_uart.c \
	SDK-13.0.0/components/libraries/log/src/nrf_log_backend_serial.c \
	SDK-13.0.0/components/libraries/log/src/nrf_log_frontend.c \

APP_CFLAGS := \
	-DUSE_APP_CONFIG \
	-D__STARTUP_CLEAR_BSS \
	-DBLE_STACK_SUPPORT_REQD \
	-ISDK-13.0.0/components/device \
	-ISDK-13.0.0/components/drivers_nrf/common \
	-ISDK-13.0.0/components/drivers_nrf/hal \
	-ISDK-13.0.0/components/drivers_nrf/uart \
	-ISDK-13.0.0/components/drivers_nrf/clock \
	-ISDK-13.0.0/components/libraries/bootloader \
	-ISDK-13.0.0/components/libraries/log \
	-ISDK-13.0.0/components/libraries/log/src \
	-ISDK-13.0.0/components/libraries/strerror \
	-ISDK-13.0.0/components/libraries/util \
	-ISDK-13.0.0/components/toolchain \
	-ISDK-13.0.0/components/toolchain/cmsis/include \
	-ISDK-13.0.0/components/softdevice/s132/headers/nrf52 \
	-ISDK-13.0.0/components/softdevice/s132/headers \
	-Iapp/config_nrf52 \
	-Iapp \
	-ISDK-13.0.0/config \
	-DSWI_DISABLE0

APP_CFLAGS += \
	$(SDK_CFLAGS) \
	$(SDK_CFLAGS_NRF52)

APP_LDFLAGS := \
	-Lscripts/linker -Tapp_nrf52840.ld -LSDK-13.0.0/components/toolchain/gcc
