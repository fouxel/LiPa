/*
 * port.hpp
 *
 *  Created on: Jan 8, 2018
 *      Author: jakub
 */

#ifndef PORT_HPP_
#define PORT_HPP_

#include "periph.hpp"

class Port : public Periph{
public:
	virtual bool init() = 0;

	virtual void set() = 0;
	virtual void reset() = 0;

	virtual bool read() = 0;

	virtual ~Port() {}
};



#endif /* PORT_HPP_ */
