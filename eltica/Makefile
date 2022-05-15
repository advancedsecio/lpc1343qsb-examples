# -*- coding: utf-8 -*-

PRGNAME := eltica
DEBUG := 0
ROOT := ..
VPATH := $(ROOT)/common/src
BLDDIR := build

ARCH = arm-none-eabi

AS = $(ARCH)-as
CC = $(ARCH)-gcc
LD = $(ARCH)-ld
OBJCOPY = $(ARCH)-objcopy
STRIP = $(ARCH)-strip
SIZE = $(ARCH)-size
CSUM = lpcrc

CFLAGS = -Wall -march=armv7-m -mthumb -ffreestanding
CFLAGS += -I. -I$(ROOT)/common/include
ifeq ($(DEBUG),1)
	CFLAGS += -g3 -O0
endif

ifeq ($(MAKECMDGOALS),preproc)
	CFLAGS += -E
else
	CFLAGS += -MMD -MP
endif

LFLAGS = -nostartfiles -nostdlib
LFLAGS += -Wl,-Map=$(TARGET).map,--gc-sections,-T$(PRGNAME).ld

TARGET := $(BLDDIR)/$(PRGNAME)
OBJDIR := $(BLDDIR)/obj
SRCDIRS := $(shell find $(ROOT) -type d)
SRCS := $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.c))
OBJS = $(addprefix $(OBJDIR)/,$(notdir $(SRCS)))
OBJS := $(patsubst %.c,%.o,$(OBJS))
DEPS := $(OBJS:.o=.d)

PPDIR := $(BLDDIR)/preproc
PPS = $(addprefix $(PPDIR)/,$(notdir $(SRCS)))
PPS := $(patsubst %.c,%.p,$(PPS))

#$(info SRCS = $(SRCS))
#$(info OBJS = $(OBJS))
#$(info DEPS = $(DEPS))
#$(info PPS = $(PPS))

.PHONY: all preproc clean

$(TARGET): $(OBJS)
	$(CC) $(LFLAGS) -o $@ $^
	cp $(TARGET) $(TARGET).elf
	$(SIZE) $(TARGET).elf
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin
	$(CSUM) $(TARGET).bin

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $<

preproc: $(PPS)

$(PPDIR)/%.p: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $<

doc:
	@( cat $(ROOT)/doxyfile; echo 'PROJECT_NAME = "$(PRGNAME)"' ) | doxygen -

all: clean $(TARGET)

clean:
	rm -rf $(BLDDIR) html

-include $(DEPS)
