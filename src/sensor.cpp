/*
 * sensor.cpp
 *
 *  Created on: Jan 2, 2018
 *      Author: jakub
 */

#include "sensor.h"
#include "stm32f10x.h"

Sensor::Sensor(ITimer &timer, Port& echo, Port& trig):
m_timer(timer),
m_echo(echo),
m_trig(trig)
 {
  m_echo.init();
  m_trig.init();
}

Sensor::~Sensor() {}

int Sensor::getDistance() {
  int delay = 0;
  generatePulse();
  while (1) {
	if (m_echo.read() == 1) {
	  m_timer.start();
	  while (m_echo.read() == 1) {
		if (m_timer.time() > 10000) {
		  return 1;
		}
	  }
	  delay = m_timer.end();
	  break;
	}
  }
  return delay;
}

void Sensor::generatePulse() {
  m_timer.start();

  m_trig.set();
  while (m_timer.time() < 20) {
  }
  m_timer.end();
  m_trig.reset();
}
