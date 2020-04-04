TARGET = cap_touch
C_SRCS += cap_touch.c
C_SRCS += $(BSP_BASE)/drivers/plic/plic_driver.c
CFLAGS += -O2 -fno-builtin-printf -DUSE_PLIC -DUSE_M_TIME

BSP_BASE = ../../bsp
include $(BSP_BASE)/env/common.mk
