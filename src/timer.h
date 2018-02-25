/*
 * Timer.h
 *
 *  Created on: Feb 24, 2018
 *      Author: jakub
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "itimer.h"

class Timer : public ITimer {
public:
	Timer();
	virtual ~Timer();

	size_t time() override;
	void start() override;
	size_t end() override;
	void interrupt() override;

private:
	volatile size_t m_count;
	volatile bool m_enabled;
};

#endif /* TIMER_H_ */
