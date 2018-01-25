/*
 * Expander.h
 *
 *  Created on: Jan 7, 2018
 *      Author: jakub
 */

#ifndef EXPANDER_HPP_
#define EXPANDER_HPP_

#include "periph.hpp"
#include "gpio.hpp"
#include "iexpander.hpp"

class Expander: public IExpander{
public:
	Expander();
	virtual ~Expander();

	void init();

	void setPin(pin_t pin);
	void resetPin(pin_t pin);

	bool readPin(pin_t pin);

private:
	GPIO gpioCS;

	void writeReg(uint8_t addr, uint8_t value);
	uint8_t readReg(uint8_t addr);
};

#endif /* EXPANDER_HPP_ */
