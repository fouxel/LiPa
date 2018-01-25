/*
 * IExpander.hpp
 *
 *  Created on: Jan 25, 2018
 *      Author: jakub
 */

#ifndef IEXPANDER_HPP_
#define IEXPANDER_HPP_

typedef unsigned char pin_t;

class IExpander {
public:
	virtual void init() = 0;
	virtual void setPin(pin_t pin) = 0;
	virtual void resetPin(pin_t pin) = 0;
	virtual bool readPin(pin_t pin) = 0;

	virtual ~IExpander() {};
};

#endif /* IEXPANDER_HPP_ */
