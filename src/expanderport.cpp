/*
 * expanderport.cpp
 *
 *  Created on: Jan 25, 2018
 *      Author: jakub
 */

#include "expanderport.hpp"


ExpanderPort::ExpanderPort(IExpander &expander, pin_t pin):
m_expander(expander),
m_pin(pin)
{}

bool ExpanderPort::init() {
	return true;
}

void ExpanderPort::set() {
	m_expander.setPin(m_pin);
}

void ExpanderPort::reset() {
	m_expander.resetPin(m_pin);
}

bool ExpanderPort::read() {
	return m_expander.readPin(m_pin);
}
