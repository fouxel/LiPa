/*
 * Timer.cpp
 *
 *  Created on: Feb 24, 2018
 *      Author: jakub
 */

#include "timer.h"
#include "stm32f10x.h"


Timer::Timer():
m_count(0),
m_enabled(false) {
	  TIM_TimeBaseInitTypeDef tim;
	  TIM_TimeBaseStructInit(&tim);
	  tim.TIM_CounterMode = TIM_CounterMode_Up;
	  tim.TIM_Prescaler = 32 - 1;
	  tim.TIM_Period = 2;
	  TIM_TimeBaseInit(TIM2, &tim);
}

Timer::~Timer() { }

size_t Timer::time() {
	return m_count;
}

void Timer::start() {
	m_enabled = true;
	m_count = 0;
}

size_t Timer::end() {
	m_enabled = false;
	return m_count;
};

void Timer::interrupt() {
	if (m_enabled) {
		++m_count;
	}
}
