/*
 * isensor.hpp
 *
 *  Created on: Jan 25, 2018
 *      Author: jakub
 */

#ifndef ISENSOR_HPP_
#define ISENSOR_HPP_

class ISensor {
public:
	virtual ~ISensor() {}

	virtual int getDistance() = 0;
};

#endif /* ISENSOR_HPP_ */
