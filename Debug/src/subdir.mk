################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ext_irq_setup.c \
../src/hal_entry.c \
../src/pm2008.c \
../src/timer_setup.c 

C_DEPS += \
./src/ext_irq_setup.d \
./src/hal_entry.d \
./src/pm2008.d \
./src/timer_setup.d 

OBJS += \
./src/ext_irq_setup.o \
./src/hal_entry.o \
./src/pm2008.o \
./src/timer_setup.o 

SREC += \
ui_project.srec 

MAP += \
ui_project.map 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	$(file > $@.in,-mcpu=cortex-m23 -mthumb -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g -D_RENESAS_RA_ -D_RA_CORE=CM23 -I"C:/Users/Tarcin/e2_studio/workspace/ui_project/src" -I"C:/Users/Tarcin/e2_studio/workspace/ui_project/ra/fsp/inc" -I"C:/Users/Tarcin/e2_studio/workspace/ui_project/ra/fsp/inc/api" -I"C:/Users/Tarcin/e2_studio/workspace/ui_project/ra/fsp/inc/instances" -I"C:/Users/Tarcin/e2_studio/workspace/ui_project/ra/arm/CMSIS_5/CMSIS/Core/Include" -I"C:/Users/Tarcin/e2_studio/workspace/ui_project/ra_gen" -I"C:/Users/Tarcin/e2_studio/workspace/ui_project/ra_cfg/fsp_cfg/bsp" -I"C:/Users/Tarcin/e2_studio/workspace/ui_project/ra_cfg/fsp_cfg" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

