/*
 * Periph.h
 *
 *  Created on: Jan 7, 2018
 *      Author: jakub
 */

#ifndef PERIPH_HPP_
#define PERIPH_HPP_

class Periph {
public:
	Periph();
	virtual ~Periph();

	virtual bool init() = 0;
};



#endif /* PERIPH_HPP_ */
