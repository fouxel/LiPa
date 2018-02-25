/*
 * ITimer.h
 *
 *  Created on: Feb 24, 2018
 *      Author: jakub
 */

#ifndef ITIMER_H_
#define ITIMER_H_

#include <cstddef>

class ITimer {
public:
	virtual ~ITimer() {}

	virtual size_t time() = 0;

	virtual void start() = 0;

	virtual size_t end() = 0;

	/**
	 * @brief This function is called when timer interrupt occurs.
	 * 		  It shall be called from the layer that directly receives
	 * 		  an interrupt. Implementation must not use any blocking
	 * 		  calls.
	 *
	 **/
	virtual void interrupt() = 0;
};

#endif /* ITIMER_H_ */
