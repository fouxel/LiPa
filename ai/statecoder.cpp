/*
 * Copyright 2018 <copyright holder> <email>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 */

#include "statecoder.h"

StateCoder::StateCoder(int maxValue): m_maxValue(maxValue) {}

state StateCoder::encode(cdistvec &vec, int maxValue) {
  std::size_t state = 0;
  unsigned multiplier = 1;
  for (int d : vec) {
    state += multiplier * d;
    multiplier *= (maxValue + 1);
  }
  return state;
}