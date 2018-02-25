/*
 * sensor.h
 *
 *  Created on: Jan 2, 2018
 *      Author: jakub
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#include "isensor.hpp"
#include "itimer.h"
#include "gpio.hpp"

class Sensor : public ISensor {
public:
	//TODO: This should not be a reference, but a shared_ptr.
	Sensor(ITimer &timer, Port& echo, Port& trig);
	virtual ~Sensor();

	int getDistance();

private:
	ITimer &m_timer;
	Port &m_echo;
	Port &m_trig;
	void generatePulse();
};



#endif /* SENSOR_H_ */
