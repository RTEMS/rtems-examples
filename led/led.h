/*
 *  LED -- simple LED support
 */

#ifndef __LED_h
#define __LED_h

#define __LONG_LED

#include <stdio.h>

#if defined(mbx860)

#define MBX_LED_REG *((volatile unsigned char *)0xfa100001)

#define LED_ON()  MBX_LED_REG = 0x0F
#define LED_OFF() MBX_LED_REG = 0x00

#elif defined(icecube)

#include <bsp/mpc5200.h>

#define LED_INIT() \
  do { \
    mpc5200.gpiosen |= 0x30000000; \
    mpc5200.gpiosdd |= 0x30000000; \
  } while (0)

#define LED_ON() \
  do { \
    mpc5200.gpiosdo |= 0x30000000; \
  } while (0)

#define LED_OFF() \
  do { \
    mpc5200.gpiosdo &= ~0x30000000; \
  } while (0)

#elif defined(MPC8313ERDB)

/* This rotates a single LED off around the back of 8. */
volatile uint8_t *MPC8313_LED_REG;
uint8_t MPC8313_LED_Values[] =
    { 0x01, 0x02, 0x04, 0x08, 0x80, 0x40, 0x20, 0x10};
uint8_t MPC8313_LED_Count;

#define LED_INIT()  \
  do { \
    MPC8313_LED_REG = (uint8_t *) 0xfa000000; \
    MPC8313_LED_Count = 0; \
  } while (0)

#define LED_ROTATE()  \
  do { \
    *MPC8313_LED_REG = MPC8313_LED_Values[MPC8313_LED_Count]; \
    /* printk( "v=0x%02x ", MPC8313_LED_Values[MPC8313_LED_Count] ); */ \
    if ( ++MPC8313_LED_Count == 9 ) \
      MPC8313_LED_Count = 0; \
  } while (0)

#define LED_ON() LED_ROTATE()
#define LED_OFF() LED_ROTATE()

#elif defined(BCM2835_GPIO_REGS_BASE)
// Raspberry Pi
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3)) 
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3)) 
#define GPIO_SET *(gpio+7)   // sets   bits which are 1 ignores bits which are 0 
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0
// For GPIO# >= 32 (RPi B+)
#define GPIO_SET_EXT *(gpio+8)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR_EXT *(gpio+11) // clears bits which are 1 ignores bits which are 0

// RPi B
//#define LED_INIT()  do { unsigned int *gpio = (unsigned int *)BCM2835_GPIO_REGS_BASE; OUT_GPIO(16);} while(0)
//#define LED_ON()  do { unsigned int *gpio = (unsigned int *)BCM2835_GPIO_REGS_BASE; GPIO_CLR = 1 << 16;} while(0)
//#define LED_OFF()  do { unsigned int *gpio = (unsigned int *)BCM2835_GPIO_REGS_BASE; GPIO_SET = 1 << 16;} while(0)
// RPi B+ => led 47
#define LED_INIT()  do { unsigned int *gpio = (unsigned int *)BCM2835_GPIO_REGS_BASE; OUT_GPIO(47);} while(0)
#define LED_ON()  do { unsigned int *gpio = (unsigned int *)BCM2835_GPIO_REGS_BASE; GPIO_CLR_EXT = 1 << (47 % 32);} while(0)
#define LED_OFF()  do { unsigned int *gpio = (unsigned int *)BCM2835_GPIO_REGS_BASE; GPIO_SET_EXT = 1 << (47 % 32);} while(0)

#elif defined(STM32F4_FAMILY_F4XXXX)

#include <bsp/io.h> //Everything we need is in io.h

//STM32F4 Discovery Board, LED4: PORTD, 12
stm32f4_gpio_config led3config =
{
	.fields={
		.pin_first = STM32F4_GPIO_PIN(3, 12),
	.pin_last = STM32F4_GPIO_PIN(3, 12),
	.mode = STM32F4_GPIO_MODE_OUTPUT,
	.otype = STM32F4_GPIO_OTYPE_PUSH_PULL,
	.ospeed = STM32F4_GPIO_OSPEED_2_MHZ,
	.pupd = STM32F4_GPIO_NO_PULL,
	.output = 1,
	.af = 0
   }
};

#define LED_INIT() stm32f4_gpio_set_config(&led3config)
#define LED_ON() stm32f4_gpio_set_output(STM32F4_GPIO_PIN(3,12), 1)
#define LED_OFF() stm32f4_gpio_set_output(STM32F4_GPIO_PIN(3,12), 0)

#else
/* default case is to print */

#define __LED_PRINTING 1
#define LED_ON()  fputs( "LED ON\n", stderr )
#define LED_OFF() fputs( "LED OFF\n", stderr )
#endif

#ifndef LED_INIT
#define LED_INIT() 
#endif

#endif
