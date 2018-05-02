#include <cmath>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include "diag/Trace.h"

#include "stm32f10x.h"
#include <stdint.h>

#include "expander.hpp"
#include "expanderport.hpp"
#include "gpio.hpp"

#include "timer.h"
#include "sensor.h"

std::unique_ptr<Timer> _timer;

extern "C" {
void TIM2_IRQHandler() {
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    _timer->interrupt();
  }
}
}

void leftForward(uint16_t pulse) {
  TIM_OCInitTypeDef channel;

  TIM_OCStructInit(&channel);
  channel.TIM_OCMode = TIM_OCMode_PWM1;
  channel.TIM_OutputState = TIM_OutputState_Enable;
  channel.TIM_Pulse = pulse;
  TIM_OC1Init(TIM4, &channel);
}

void leftStop() {
  TIM_OCInitTypeDef channel;

  TIM_OCStructInit(&channel);
  channel.TIM_OCMode = TIM_OCMode_PWM1;
  channel.TIM_OutputState = TIM_OutputState_Enable;
  channel.TIM_Pulse = 0;
  TIM_OC1Init(TIM4, &channel);
}

void rightForward(uint16_t pulse) {
  TIM_OCInitTypeDef channel;

  TIM_OCStructInit(&channel);
  channel.TIM_OCMode = TIM_OCMode_PWM1;
  channel.TIM_OutputState = TIM_OutputState_Enable;
  channel.TIM_Pulse = pulse;
  TIM_OC2Init(TIM4, &channel);
}

void rightStop() {
  TIM_OCInitTypeDef channel;

  TIM_OCStructInit(&channel);
  channel.TIM_OCMode = TIM_OCMode_PWM1;
  channel.TIM_OutputState = TIM_OutputState_Enable;
  channel.TIM_Pulse = 0;
  TIM_OC2Init(TIM4, &channel);
}

/*
 *
  // TRIG do czujnika odległości(PC5) oraz ECHO (PC6)
  GPIO echo(GPIO_Pin_6, GPIOC, GPIO_Mode_IPD);
  GPIO trig(GPIO_Pin_5, GPIOC, GPIO_Mode_Out_PP);
 * */

void send_char(uint16_t c)
{
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    USART_SendData(USART2, c);
}

uint16_t read_char()
{
	while (!USART_GetFlagStatus(USART2, USART_FLAG_RXNE));
	return USART_ReceiveData(USART2);
}

int main(void) {
  TIM_TimeBaseInitTypeDef tim;
  NVIC_InitTypeDef nvic;

  GPIO_InitTypeDef gpio;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                             RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD,
                         ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM4, ENABLE);

  // usart
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

  GPIO_StructInit(&gpio);
  gpio.GPIO_Pin = GPIO_Pin_2;
  gpio.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &gpio);

  gpio.GPIO_Pin = GPIO_Pin_3;
  gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &gpio);

  USART_InitTypeDef uart;

  USART_StructInit(&uart);
  uart.USART_BaudRate = 115200;
  USART_Init(USART2, &uart);

  USART_Cmd(USART2, ENABLE);

  // 1 - wejscie, 0 - wyjscie
  Expander expander(0b11101010);
  ExpanderPort expPortTrig(expander, 0);
  ExpanderPort expPortEcho(expander, 1);
  ExpanderPort expPortTrig2(expander, 2);
  ExpanderPort expPortEcho2(expander, 3);

  ExpanderPort expPortTrig3(expander, 4);
  ExpanderPort expPortEcho3(expander, 5);

  GPIO portEcho(GPIO_Pin_0, GPIOA, GPIO_Mode_IN_FLOATING);
  GPIO portTrig(GPIO_Pin_1, GPIOA, GPIO_Mode_Out_PP);

  GPIO portEcho2(GPIO_Pin_2, GPIOC, GPIO_Mode_IN_FLOATING);
  GPIO portTrig2(GPIO_Pin_3, GPIOC, GPIO_Mode_Out_PP);

  GPIO portEcho3(GPIO_Pin_10, GPIOC, GPIO_Mode_IN_FLOATING);
  GPIO portTrig3(GPIO_Pin_11, GPIOC, GPIO_Mode_Out_PP);

  constexpr auto SENSOR_COUNT = 3;
  _timer.reset(new Timer());
  std::array<Sensor, SENSOR_COUNT> sensors = { Sensor(*(_timer.get()), portEcho, portTrig),
		  	  	  	  	  	  	   	   	   	   Sensor(*(_timer.get()), portEcho2, portTrig2),
											   Sensor(*(_timer.get()), portEcho3, portTrig3)};

  // Ustawienia silników PC6, PC7
  gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  gpio.GPIO_Speed = GPIO_Speed_50MHz;
  gpio.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &gpio);

  //Konfiguracja timera do pwm silnika
  TIM_TimeBaseStructInit(&tim);
  tim.TIM_CounterMode = TIM_CounterMode_Up;
  tim.TIM_Prescaler = 64 - 1;
  tim.TIM_Period = 20000;
  TIM_TimeBaseInit(TIM4, &tim);

  TIM_OCInitTypeDef channel;

  TIM_OCStructInit(&channel);
  channel.TIM_OCMode = TIM_OCMode_PWM1;
  channel.TIM_OutputState = TIM_OutputState_Enable;
  channel.TIM_Pulse = 1000;
  TIM_OC1Init(TIM4, &channel);
  channel.TIM_Pulse = 1000;
  TIM_OC2Init(TIM4, &channel);

  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM2, ENABLE);
  TIM_Cmd(TIM4, ENABLE);

  nvic.NVIC_IRQChannel = TIM2_IRQn;
  nvic.NVIC_IRQChannelPreemptionPriority = 0;
  nvic.NVIC_IRQChannelSubPriority = 0;
  nvic.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic);

  leftStop();
  rightStop();
  while (1) {
	for (int i = 0; i < 3; ++i) {
		for (auto &sensor : sensors) {
			auto dist = sensor.getDistance();
			trace_printf("distance original: %d\n", dist);
			dist /= 2;
			if (dist > 200) {
				dist = 200;
			}
			trace_printf("distance send: %d\n", dist);
			send_char(dist);
	    	_timer->sleep(1500);

			auto ack = read_char();
			if (ack != dist) {
				trace_printf("wrong ack! %d\n", ack);
			}
		}
    trace_printf("###########\n");
    _timer->sleep();
	}

	auto action = read_char();
    trace_printf("action: %d\n", action);

    /*
        ACTION_FORWARD, // Move forward.
        ACTION_LEFT, // Turn left.
        ACTION_RIGHT, // Turn right.
        ACTION_TERMINATE // Agent reache
     * */

    if (action == 0) {
    	leftForward(1420);
    	rightForward(1380);
    } else if (action == 1) {
    	leftForward(1250);
    	rightStop();
    } else if (action == 2) {
    	rightForward(1250);
    	leftStop();
    } else {
    	rightStop();
    	leftStop();
    }

    if (action == 0) {
    	_timer->sleep(150000);
    }
    _timer->sleep(150000);
    rightStop();
    leftStop();
  }
}
