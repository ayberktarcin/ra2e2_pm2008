################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra/fsp/src/r_wdt/r_wdt.c 

C_DEPS += \
./ra/fsp/src/r_wdt/r_wdt.d 

OBJS += \
./ra/fsp/src/r_wdt/r_wdt.o 

SREC += \
ui_project.srec 

MAP += \
ui_project.map 


# Each subdirectory must supply rules for building sources it contributes
ra/fsp/src/r_wdt/%.o: ../ra/fsp/src/r_wdt/%.c
	$(file > $@.in,-mcpu=cortex-m23 -mthumb -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g -D_RENESAS_RA_ -D_RA_CORE=CM23 -I"C:/Users/Tarcin/e2_studio/workspace/ui_project/src" -I"C:/Users/Tarcin/e2_studio/workspace/ui_project/ra/fsp/inc" -I"C:/Users/Tarcin/e2_studio/workspace/ui_project/ra/fsp/inc/api" -I"C:/Users/Tarcin/e2_studio/workspace/ui_project/ra/fsp/inc/instances" -I"C:/Users/Tarcin/e2_studio/workspace/ui_project/ra/arm/CMSIS_5/CMSIS/Core/Include" -I"C:/Users/Tarcin/e2_studio/workspace/ui_project/ra_gen" -I"C:/Users/Tarcin/e2_studio/workspace/ui_project/ra_cfg/fsp_cfg/bsp" -I"C:/Users/Tarcin/e2_studio/workspace/ui_project/ra_cfg/fsp_cfg" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

