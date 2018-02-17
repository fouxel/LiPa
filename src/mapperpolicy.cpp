/*
 * mapperpolicy.cpp
 *
 *  Created on: Jan 25, 2018
 *      Author: jakub
 */

#include "mapperpolicy.hpp"

MapperPolicy::MapperPolicy(mapperIterator_t begin, mapperIterator_t end)
    : m_begin(begin), m_end(end) {}

int MapperPolicy::getState() {
  // TODO: algorithm polling every sensor for the distance and calculating
  // unique state for collection.
  return 0;
}
