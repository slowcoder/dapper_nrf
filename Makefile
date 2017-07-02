CROSS_COMPILE ?= arm-none-eabi
CC := $(CROSS_COMPILE)-gcc
OBJCOPY := $(CROSS_COMPILE)-objcopy

all: out/bootloader out/app

include Makefile.toolchain
include Makefile.sdk
include bootloader/build.mk
include app/build.mk

BL_OBJS := \
	$(patsubst %.c,out/bootloader/obj/%.o, $(filter %.c, $(BL_SRCS))) \
	$(patsubst %.S,out/bootloader/obj/%.o, $(filter %.S, $(BL_SRCS)))

APP_OBJS := \
	$(patsubst %.c,out/app/obj/%.o, $(filter %.c, $(APP_SRCS))) \
	$(patsubst %.S,out/app/obj/%.o, $(filter %.S, $(APP_SRCS)))

ALL_DEPS := $(patsubst %.o,%.d,$(BL_OBJS) $(APP_OBJS))

out/app/obj/%.o:%.c
	@echo "CC $<"
	@mkdir -p $(dir $@)
	@$(CC) -MM -MF $(subst .o,.d,$@) -MP -MT "$@ $(subst .o,.d,$@)" $(NRF52840_CFLAGS) $(APP_CFLAGS) $<
	@$(CC) $(NRF52840_CFLAGS) $(APP_CFLAGS) -c -o $@ $<

out/app/obj/%.o:%.S
	@echo "CC $<"
	@mkdir -p $(dir $@)
	@$(CC) -MM -MF $(subst .o,.d,$@) -MP -MT "$@ $(subst .o,.d,$@)" $(NRF52840_CFLAGS) $(APP_CFLAGS) $<
	@$(CC) $(NRF52840_CFLAGS) $(APP_CFLAGS) -c -o $@ $<

out/app: $(APP_OBJS) $(APP_LIBS)
	@echo "LD $@"
	@$(CC) $(NRF52840_CFLAGS) $(APP_CFLAGS) -o $@.elf $^ $(NRF52840_LDFLAGS) $(APP_LDFLAGS) -Xlinker -Map=$@.map
	@$(OBJCOPY) -O ihex $@.elf $@.hex

out/bootloader/obj/%.o:%.c
	@echo "CC $<"
	@mkdir -p $(dir $@)
	@$(CC) -MM -MF $(subst .o,.d,$@) -MP -MT "$@ $(subst .o,.d,$@)" $(NRF52840_CFLAGS) $(BL_CFLAGS) $<
	@$(CC) $(NRF52840_CFLAGS) $(BL_CFLAGS) -c -o $@ $<

out/bootloader/obj/%.o:%.S
	@echo "CC $<"
	@mkdir -p $(dir $@)
	@$(CC) -MM -MF $(subst .o,.d,$@) -MP -MT "$@ $(subst .o,.d,$@)" $(NRF52840_CFLAGS) $(BL_CFLAGS) $<
	@$(CC) $(NRF52840_CFLAGS) $(BL_CFLAGS) -c -o $@ $<

out/bootloader: $(BL_OBJS) $(BL_LIBS)
	@echo "LD $@"
	@$(CC) $(NRF52840_CFLAGS) $(BL_CFLAGS) -o $@.elf $^ $(NRF52840_LDFLAGS) $(BL_LDFLAGS) -Xlinker -Map=$@.map
	@$(OBJCOPY) -O ihex $@.elf $@.hex

clean:
	@echo "Cleaning"
	@rm -rf $(BL_OBJS) $(APP_OBJS) $(ALL_DEPS) out/

ifneq ("$(MAKECMDGOALS)","clean")
-include $(ALL_DEPS)
endif
