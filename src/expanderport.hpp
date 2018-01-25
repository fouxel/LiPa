/*
 * expanderport.hpp
 *
 *  Created on: Jan 25, 2018
 *      Author: jakub
 */

#ifndef EXPANDERPORT_HPP_
#define EXPANDERPORT_HPP_

#include "port.hpp"
#include "expander.hpp"

class ExpanderPort : public Port {
public:
	ExpanderPort(IExpander &expander, pin_t pin);
	virtual ~ExpanderPort() {}

	bool init();

	void set();
	void reset();

	bool read();

private:
	IExpander& m_expander;
	pin_t m_pin;
};


#endif /* EXPANDERPORT_HPP_ */
