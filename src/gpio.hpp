/*
 * gpio.hpp
 *
 *  Created on: Jan 8, 2018
 *      Author: jakub
 */

#ifndef GPIO_HPP_
#define GPIO_HPP_

#include "periph.hpp"
#include "port.hpp"
#include "stm32f10x.h"

class GPIO : public Port{
public:
	GPIO(uint16_t pin, GPIO_TypeDef* gpiox, GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed = GPIO_Speed_50MHz);

	bool init();

	void set();
	void reset();

	bool read();

private:
	GPIO_InitTypeDef m_gpio;
	uint16_t m_pin;
	GPIOMode_TypeDef m_mode;
	GPIOSpeed_TypeDef m_speed;
	GPIO_TypeDef *m_gpiox;

};


#endif /* GPIO_HPP_ */
