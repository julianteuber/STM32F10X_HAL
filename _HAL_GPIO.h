#ifndef _HAL_GPIO
#define _HAL_GPIO

#include "stm32f10x.h"

#define LOW		0
#define HIGH	1

// Port Names
#define PORTA GPIOA
#define PORTB GPIOB
#define PORTC GPIOC
#define PORTD GPIOD
#define PORTE GPIOE
#define PORTF GPIOF
#define PORTG GPIOG

// Pin Modes
#define MODE_OUTPUT 		((uint32_t)0x01)
#define MODE_INPUT			((uint32_t)0x02)

// Input Mode Types
#define INPUT_ANALOG		((uint32_t)0x00)	// Analog mode
#define INPUT_FLOATING	((uint32_t)0x01)	// Floating input (reset state)
#define INPUT_PULLUD		((uint32_t)0x02)	// Input with pull-up / pull-down
#define INPUT_RESERVED	((uint32_t)0x04)	// Reserved

// Output Mode Types
#define OUTPUT_GEN_PP		((uint32_t)0x00)	// General purpose output push-pull
#define OUTPUT_GEN_OD		((uint32_t)0x01)	// General purpose output Open-drain
#define OUTPUT_FUNC_PP	((uint32_t)0x02)	// Alternate function output Push-pull
#define OUTPUT_FUNC_OD	((uint32_t)0x04)	// Alternate function output Open-drain

// pin speeds/slew rates
#define SPEED_2MHZ			((uint32_t)0x02)	// Output mode, max speed 2 MHz
#define SPEED_10MHZ			((uint32_t)0x01)	// Output mode, max speed 10 MHz
#define SPEED_50MHZ			((uint32_t)0x03)	// Output mode, max speed 50 MHz

// Clock Enable
#define GPIO_CLK_ENABLE_ALT_FUNC	(RCC->APB2ENR |= (1<<0))
#define GPIO_CLK_ENABLE_PORTA 		(RCC->APB2ENR |= (1<<2))
#define GPIO_CLK_ENABLE_PORTB 		(RCC->APB2ENR |= (1<<3))
#define GPIO_CLK_ENABLE_PORTC 		(RCC->APB2ENR |= (1<<4))
#define GPIO_CLK_ENABLE_PORTD 		(RCC->APB2ENR |= (1<<5))

// CNF pin configuration bits and
// MODE pin mode bits pointer offset
#define CNF_POS_BIT1		(PIN_POS[pinNumber] + 2)
#define CNF_POS_BIT2		(PIN_POS[pinNumber] + 3)

// Configuration structure
typedef struct
{
	GPIO_TypeDef *port;
	uint32_t	pin;
	uint32_t	mode;
	uint32_t	mode_type;
	uint32_t	pull;
	uint32_t	speed;
	uint32_t	alt_func;
}GPIO_TYPE;

typedef enum{
	RISING_EDGE,
	FALLING_EDGE,
	RISING_FALLING_EDGE
}edge_select;

// Function Prototypes
// ****************************************************
//                   GPIO Configuration
void gpio_init(GPIO_TYPE gpioType);
static void config_pin (GPIO_TypeDef *port, uint32_t pinNumber, uint32_t modeType);
static void config_pin_speed (GPIO_TypeDef *port, uint32_t pinNumber, uint32_t pinSpeed, uint32_t mode);

// ****************************************************
//                   GPIO Functions
void gpio_write(GPIO_TypeDef *port, uint32_t pinNumber, uint8_t state);
void gpio_toggle(GPIO_TypeDef *port, uint32_t pinNumber);

// ****************************************************
//                   Interrupt Functions
// Nested vector interrupt controller (NVIC)
// External interrupt/event controller (EXTI)
void configure_gpio_interrupt(GPIO_TypeDef *port, uint32_t pinNumber, edge_select edge);
void enable_gpio_interrupt(uint32_t pinNumber, IRQn_Type irqNummber);
void clear_gpio_interrupt(uint32_t pinNumber);

#endif /*_HAL_GPIO*/
