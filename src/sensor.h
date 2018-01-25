/*
 * sensor.h
 *
 *  Created on: Jan 2, 2018
 *      Author: jakub
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#include "isensor.hpp"

class Sensor : public ISensor {
public:
	Sensor();
	virtual ~Sensor();

	int getDistance();
};



#endif /* SENSOR_H_ */
