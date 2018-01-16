/*
 * port.hpp
 *
 *  Created on: Jan 8, 2018
 *      Author: jakub
 */

#ifndef PORT_HPP_
#define PORT_HPP_

class Port : public Periph{
public:
	Port();

	virtual bool init() = 0;
};



#endif /* PORT_HPP_ */
