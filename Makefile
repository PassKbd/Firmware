TARGET=main.hex
EXECUTABLE=main.elf

CC=arm-none-eabi-gcc
LD=arm-none-eabi-gcc
AR=arm-none-eabi-ar
AS=arm-none-eabi-as
CP=arm-none-eabi-objcopy
OD=arm-none-eabi-objdump

BIN=$(CP) -O ihex 

# Change HSE_VALUE according to your external clock.
DEFS = -DUSE_STDPERIPH_DRIVER -DSTM32F4XX -DHSE_VALUE=8000000
STARTUP = Libs/CMSIS/startup_stm32f40xx.s

MCU = cortex-m4
MCFLAGS = -mcpu=$(MCU) -mthumb -mlittle-endian -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb-interwork -std=c99
STM32_INCLUDES = \
	-ILibs/CMSIS/Include/ \
	-ILibs/STM32F4xx_StdPeriph_Driver/inc/

OPTIMIZE       = -Os

CFLAGS	= $(MCFLAGS)  $(OPTIMIZE)  $(DEFS) -I. -I./ $(STM32_INCLUDES)  -Wl,-T,Libs/stm32_flash.ld
AFLAGS	= $(MCFLAGS) 
#-mapcs-float use float regs. small increase in code size

SRC = main.c \
	stm32f4xx_it.c \
	Libs/CMSIS/system_stm32f4xx.c \
	Libs/STM32F4xx_StdPeriph_Driver/src/misc.c \
	Libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_adc.c \
	Libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_can.c \
	Libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_crc.c \
	Libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_cryp_aes.c \
	Libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_cryp.c \
	Libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_cryp_des.c \
	Libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_cryp_tdes.c \
	Libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dac.c \
	Libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dbgmcu.c \
	Libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dcmi.c \
	Libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dma.c \
	Libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_exti.c \
	Libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_flash.c \
	Libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_fsmc.c \
	Libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c \
	Libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_hash.c \
	Libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_hash_md5.c \
	Libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_hash_sha1.c \
	Libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c \
	Libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_iwdg.c \
	Libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_pwr.c \
	Libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c \
	Libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rng.c \
	Libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rtc.c \
	Libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_sdio.c \
	Libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_spi.c \
	Libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_syscfg.c \
	Libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_tim.c \
	Libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_usart.c \
	Libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_wwdg.c 

OBJDIR = .
OBJ = $(SRC:%.c=$(OBJDIR)/%.o) 
OBJ += Startup.o

all: $(TARGET)

$(TARGET): $(EXECUTABLE)
	$(CP) -O ihex $^ $@

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -lm -lc -lnosys -o $@

clean:
	rm -f Startup.lst  $(TARGET)  $(TARGET).lst $(OBJ) $(AUTOGEN)  $(TARGET) $(EXECUTABLE)

Startup.o: $(STARTUP)
	$(CC) $(CFLAGS) -c $^ -lm -lc -lnosys -o $*.o
	 
%.o: %.c
	$(CC) $(CFLAGS) -c $^ -lm -lc -lnosys -o $*.o