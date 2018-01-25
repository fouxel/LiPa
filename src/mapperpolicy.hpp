/*
 * expandermapperpolicy.hpp
 *
 *  Created on: Jan 25, 2018
 *      Author: jakub
 */

#ifndef MAPPERPOLICY_HPP_
#define MAPPERPOLICY_HPP_

#include "imapperpolicy.hpp"
#include "isensor.hpp"
#include <algorithm>

class MapperPolicy : public IMapperPolicy {
public:
	typedef std::iterator<std::bidirectional_iterator_tag, ISensor> mapperIterator_t;

	MapperPolicy(mapperIterator_t begin, mapperIterator_t end);

	int getState() override;

private:
	mapperIterator_t m_begin;
	mapperIterator_t m_end;
};


#endif /* MAPPERPOLICY_HPP_ */
