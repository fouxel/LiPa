/*
 * gpio.cpp
 *
 *  Created on: Jan 8, 2018
 *      Author: jakub
 */
#include "gpio.hpp"

GPIO::GPIO(uint16_t pin, GPIO_TypeDef* gpiox, GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed):
m_pin(pin),
m_mode(mode),
m_speed(speed),
m_gpiox(gpiox) {

}

bool GPIO::init() {
	GPIO_InitTypeDef gpio;
	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = m_pin; //SCK, MOSI
	gpio.GPIO_Mode = m_mode;
	gpio.GPIO_Speed = m_speed;
	GPIO_Init(m_gpiox, &gpio);
	return true;
}

void GPIO::set() {
	GPIO_SetBits(m_gpiox, m_pin);
}

void GPIO::reset() {
	GPIO_ResetBits(m_gpiox, m_pin);
}
