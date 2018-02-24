/*
 * ITimer.h
 *
 *  Created on: Feb 24, 2018
 *      Author: jakub
 */

#ifndef ITIMER_H_
#define ITIMER_H_

class ITimer {
public:
	virtual ~ITimer();

	virtual size_t getTime() = 0;

	virtual void start() = 0;

	virtual void end() = 0;
};

#endif /* ITIMER_H_ */
