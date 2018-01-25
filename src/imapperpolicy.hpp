/*
 * IMapperPolicy.hpp
 *
 *  Created on: Jan 25, 2018
 *      Author: jakub
 */

#ifndef IMAPPERPOLICY_HPP_
#define IMAPPERPOLICY_HPP_

class IMapperPolicy {
public:
	virtual ~IMapperPolicy() {}

	virtual int getState() = 0;
};

#endif /* IMAPPERPOLICY_HPP_ */
