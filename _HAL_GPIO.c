#include "_HAL_GPIO.h"
#include <stdint.h>

const uint32_t PIN_POS[16] = {
	(0x00),
	(0x04),
	(0x08),
	(0x0C),
	(0x10),
	(0x14),
	(0x18),
	(0x1C),
	(0x00),
	(0x04),
	(0x08),
	(0x0C),
	(0x10),
	(0x14),
	(0x18),
	(0x1C)
};

static void config_pin (GPIO_TypeDef *port, uint32_t pinNumber, uint32_t modeType)
{
	if(pinNumber >= 8) // control high register
	{
		switch(modeType)
		{
			case OUTPUT_GEN_PP | INPUT_ANALOG:
				port->CRH &= ~( ( 1 << CNF_POS_BIT1 ) | ( 1 << CNF_POS_BIT2) );
			break;
			
			case OUTPUT_GEN_PP | INPUT_FLOATING:
				port->CRH &= ~( 1 << CNF_POS_BIT1 );
				port->CRH |= ( 1 << CNF_POS_BIT1 );
			break;
			
			case OUTPUT_FUNC_PP | INPUT_PULLUD:
				port->CRH |= OUTPUT_FUNC_PP<<(CNF_POS_BIT1);
			break;
			
			case OUTPUT_FUNC_OD:
				port->CRH |= OUTPUT_FUNC_OD<<(CNF_POS_BIT1);
			break;
		}/*end switch*/
	}
	else	// control low register
	{
		switch(modeType)
		{
			case OUTPUT_GEN_PP | INPUT_ANALOG:
				port->CRL &= ~( ( 1 << CNF_POS_BIT1 ) | ( 1 << CNF_POS_BIT2) );
			break;
			
			case OUTPUT_GEN_PP | INPUT_FLOATING:
				port->CRL &= ~( 1 << CNF_POS_BIT1 );
				port->CRL |= ( 1 << CNF_POS_BIT1 );
			break;
			
			case OUTPUT_FUNC_PP | INPUT_PULLUD:
				port->CRL |= OUTPUT_FUNC_PP<<(CNF_POS_BIT1);
			break;
			
			case OUTPUT_FUNC_OD:
				port->CRL |= OUTPUT_FUNC_OD<<(CNF_POS_BIT1);
			break;
		}/*end switch*/
	}
}

static void config_pin_speed (GPIO_TypeDef *port, uint32_t pinNumber, uint32_t pinSpeed, uint32_t mode)
{
	if(pinNumber >= 8)
	{
		if(mode == MODE_INPUT)
			port->CRH &= ~(1<<(PIN_POS[pinNumber]) | 1<<(PIN_POS[pinNumber] + 1));
		else
			port->CRH |= (pinSpeed<<(PIN_POS[pinNumber]));
	} 
	else
	{
		if(mode == MODE_INPUT)
			port->CRL &= ~(1<<(PIN_POS[pinNumber]) | 1<<(PIN_POS[pinNumber] + 1));
		else
			port->CRL |= (pinSpeed<<(pinNumber));
	}
}

void gpio_write(GPIO_TypeDef *port, uint32_t pinNumber, uint8_t state)
{
	if(state)
		port->BSRR = (1<<pinNumber);
	else
		port->BSRR = (1<<(pinNumber + 16));
}

void gpio_toggle(GPIO_TypeDef *port, uint32_t pinNumber)
{
	port->ODR ^= (1<<pinNumber);
}

void gpio_init(GPIO_TYPE gpioType)
{
	if(gpioType.port == PORTA)
		GPIO_CLK_ENABLE_PORTA;
	
	if(gpioType.port == PORTB)
		GPIO_CLK_ENABLE_PORTB;
	
	if(gpioType.port == PORTC)
		GPIO_CLK_ENABLE_PORTC;
	
	if(gpioType.port == PORTD)
		GPIO_CLK_ENABLE_PORTD;
	
	config_pin(gpioType.port, gpioType.pin, gpioType.mode_type);
	config_pin_speed(gpioType.port, gpioType.pin, gpioType.speed, gpioType.mode);
}

void configure_gpio_interrupt(GPIO_TypeDef *port, uint32_t pinNumber, edge_select edge)
{
	/* APB2 peripheral clock enable register */
	/* Alternate function I/O clock enable */
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	
	if(port == PORTA)
	{
		/* select the source input for EXTIx external interrupt on PORTA */
		switch(pinNumber)
		{			
			/* External interrupt configuration register 1 */
			case 0:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI0_PA; break;
			case 1:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI1_PA; break;
			case 2:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI2_PA; break;
			case 3:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI3_PA; break;
			
			/* External interrupt configuration register 2 */
			case 4:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI4_PA; break;
			case 5:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI5_PA; break;
			case 6:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI6_PA; break;
			case 7:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI7_PA; break;
			
			/* External interrupt configuration register 3 */
			case 8:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI8_PA; break;
			case 9:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI9_PA; break;
			case 10:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI10_PA; break;
			case 11:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI11_PA; break;
			
			/* External interrupt configuration register 4 */
			case 12:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI12_PA; break;
			case 13:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI13_PA; break;
			case 14:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI14_PA; break;
			case 15:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI15_PA; break;
		}
	}/* endif(port == PORTA) */
	
	if(port == PORTB)
	{
		/* select the source input for EXTIx external interrupt on PORTB */
		switch(pinNumber)
		{
			/* External interrupt configuration register 1 */
			case 0:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI0_PB; break;
			case 1:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI1_PB; break;
			case 2:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI2_PB; break;
			case 3:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI3_PB; break;
			
			/* External interrupt configuration register 2 */
			case 4:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI4_PB; break;
			case 5:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI5_PB; break;
			case 6:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI6_PB; break;
			case 7:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI7_PB; break;
			
			/* External interrupt configuration register 3 */
			case 8:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI8_PB; break;
			case 9:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI9_PB; break;
			case 10:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI10_PB; break;
			case 11:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI11_PB; break;
			
			/* External interrupt configuration register 4 */
			case 12:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI12_PB; break;
			case 13:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI13_PB; break;
			case 14:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI14_PB; break;
			case 15:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI15_PB; break;
		}
	}/* endif(port == PORTB) */
	
	if(port == PORTC)
	{
		/* select the source input for EXTIx external interrupt on PORTC */
		switch(pinNumber)
		{
			/* External interrupt configuration register 1 */
			case 0:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI0_PC; break;
			case 1:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI1_PC; break;
			case 2:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI2_PC; break;
			case 3:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI3_PC; break;
			
			/* External interrupt configuration register 2 */
			case 4:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI4_PC; break;
			case 5:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI5_PC; break;
			case 6:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI6_PC; break;
			case 7:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI7_PC; break;
			
			/* External interrupt configuration register 3 */
			case 8:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI8_PC; break;
			case 9:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI9_PC; break;
			case 10:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI10_PC; break;
			case 11:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI11_PC; break;
			
			/* External interrupt configuration register 4 */
			case 12:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI12_PC; break;
			case 13:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI13_PC; break;
			case 14:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI14_PC; break;
			case 15:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI15_PC; break;
		}
	}/* endif(port == PORTC) */
	
	if(port == PORTD)
	{
		/* select the source input for EXTIx external interrupt on PORTD */
		switch(pinNumber)
		{
			/* External interrupt configuration register 1 */
			case 0:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI0_PD; break;
			case 1:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI1_PD; break;
			case 2:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI2_PD; break;
			case 3:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI3_PD; break;
			
			/* External interrupt configuration register 2 */
			case 4:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI4_PD; break;
			case 5:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI5_PD; break;
			case 6:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI6_PD; break;
			case 7:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI7_PD; break;
			
			/* External interrupt configuration register 3 */
			case 8:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI8_PD; break;
			case 9:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI9_PD; break;
			case 10:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI10_PD; break;
			case 11:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI11_PD; break;
			
			/* External interrupt configuration register 4 */
			case 12:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI12_PD; break;
			case 13:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI13_PD; break;
			case 14:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI14_PD; break;
			case 15:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI15_PD; break;
		}
	}/* endif(port == PORTD) */
	
	if(port == PORTE)
	{
		/* select the source input for EXTIx external interrupt on PORTE */
		switch(pinNumber)
		{
			/* External interrupt configuration register 1 */
			case 0:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI0_PE; break;
			case 1:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI1_PE; break;
			case 2:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI2_PE; break;
			case 3:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI3_PE; break;
			
			/* External interrupt configuration register 2 */
			case 4:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI4_PE; break;
			case 5:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI5_PE; break;
			case 6:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI6_PE; break;
			case 7:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI7_PE; break;
			
			/* External interrupt configuration register 3 */
			case 8:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI8_PE; break;
			case 9:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI9_PE; break;
			case 10:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI10_PE; break;
			case 11:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI11_PE; break;
			
			/* External interrupt configuration register 4 */
			case 12:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI12_PE; break;
			case 13:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI13_PE; break;
			case 14:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI14_PE; break;
			case 15:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI15_PE; break;
		}
	}/* endif(port == PORTE) */
	
	if(port == PORTF)
	{
		/* select the source input for EXTIx external interrupt on PORTF */
		switch(pinNumber)
		{
			/* External interrupt configuration register 1 */
			case 0:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI0_PF; break;
			case 1:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI1_PF; break;
			case 2:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI2_PF; break;
			case 3:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI3_PF; break;
			
			/* External interrupt configuration register 2 */
			case 4:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI4_PF; break;
			case 5:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI5_PF; break;
			case 6:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI6_PF; break;
			case 7:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI7_PF; break;
			
			/* External interrupt configuration register 3 */
			case 8:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI8_PF; break;
			case 9:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI9_PF; break;
			case 10:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI10_PF; break;
			case 11:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI11_PF; break;
			
			/* External interrupt configuration register 4 */
			case 12:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI12_PF; break;
			case 13:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI13_PF; break;
			case 14:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI14_PF; break;
			case 15:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI15_PF; break;
		}
	}/* endif(port == PORTF) */
	
	if(port == PORTG)
	{
		/* select the source input for EXTIx external interrupt on PORTG */
		switch(pinNumber)
		{
			/* External interrupt configuration register 1 */
			case 0:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI0_PG; break;
			case 1:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI1_PG; break;
			case 2:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI2_PG; break;
			case 3:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI3_PG; break;
			
			/* External interrupt configuration register 2 */
			case 4:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI4_PG; break;
			case 5:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI5_PG; break;
			case 6:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI6_PG; break;
			case 7:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI7_PG; break;
			
			/* External interrupt configuration register 3 */
			case 8:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI8_PG; break;
			case 9:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI9_PG; break;
			case 10:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI10_PG; break;
			case 11:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI11_PG; break;
			
			/* External interrupt configuration register 4 */
			case 12:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI12_PG; break;
			case 13:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI13_PG; break;
			case 14:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI14_PG; break;
			case 15:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI15_PG; break;
		}
	}/* endif(port == PORTG) */
	
	/* configure Rising trigger selection register (EXTI_RTSR) */
	/* and Falling trigger selection register (EXTI_FTSR) */
	/* according to 'edge' */
	if(edge == RISING_EDGE)
		EXTI->RTSR |= 1<<pinNumber;
	if(edge == FALLING_EDGE)
		EXTI->FTSR |= 1<<pinNumber;
	if(edge == RISING_FALLING_EDGE)
	{
		EXTI->FTSR |= 1<<pinNumber;
		EXTI->RTSR |= 1<<pinNumber;
	}
}

void enable_gpio_interrupt(uint32_t pinNumber, IRQn_Type irqNumber)
{
	/* enabled interrupt request from Line x in interrupt mask register */
	EXTI->IMR |= 1<<pinNumber;
	/* enable interrupt in Nested vectored interrupt controller */
	NVIC_EnableIRQ(irqNumber);
}

void clear_gpio_interrupt(uint32_t pinNumber)
{
	/* clear interrupt trigger request in Pending register (EXTI_PR) */
	EXTI->PR |= 1<<pinNumber;
}