BL_SRCS := \
	bootloader/main.c \
	SDK-13.0.0/components/libraries/log/src/nrf_log_backend_serial.c \
	SDK-13.0.0/components/libraries/log/src/nrf_log_frontend.c \
	SDK-13.0.0/components/drivers_nrf/common/nrf_drv_common.c \
	SDK-13.0.0/components/drivers_nrf/uart/nrf_drv_uart.c \

BL_SRCS += \
	$(SDK_SRCS) \
	$(SDK_SRCS_NRF52)

BL_CFLAGS := \
	-DUSE_APP_CONFIG \
	-DSVC_INTERFACE_CALL_AS_NORMAL_FUNCTION \
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
	-Ibootloader/config_nrf52 \
	-Ibootloader \
	-ISDK-13.0.0/config \
	-DSWI_DISABLE0 \
	-DBLE_STACK_SUPPORT_REQD

BL_LDFLAGS := \
	-Lscripts/linker -Tbootloader_nrf52840.ld -LSDK-13.0.0/components/toolchain/gcc
