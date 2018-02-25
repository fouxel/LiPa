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

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

std::unique_ptr<Timer> _timer;
std::unique_ptr<Sensor> _sensor;

extern "C" {
void TIM2_IRQHandler() {
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    _timer->interrupt();
  }
}
}

void leftForward() {
  TIM_OCInitTypeDef channel;

  TIM_OCStructInit(&channel);
  channel.TIM_OCMode = TIM_OCMode_PWM1;
  channel.TIM_OutputState = TIM_OutputState_Enable;
  channel.TIM_Pulse = 1000;
  TIM_OC1Init(TIM4, &channel);
}

void leftReverse();
void leftStop() {
  TIM_OCInitTypeDef channel;

  TIM_OCStructInit(&channel);
  channel.TIM_OCMode = TIM_OCMode_PWM1;
  channel.TIM_OutputState = TIM_OutputState_Enable;
  channel.TIM_Pulse = 0;
  TIM_OC1Init(TIM4, &channel);
}

void rightForward() {
  TIM_OCInitTypeDef channel;

  TIM_OCStructInit(&channel);
  channel.TIM_OCMode = TIM_OCMode_PWM1;
  channel.TIM_OutputState = TIM_OutputState_Enable;
  channel.TIM_Pulse = 1000;
  TIM_OC2Init(TIM4, &channel);
}

void rightReverse();

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


  // 1 - wejscie, 0 - wyjscie
  Expander expander(0b11111010);
  ExpanderPort expPortTrig(expander, 0);
  ExpanderPort expPortEcho(expander, 1);
  ExpanderPort expPortTrig2(expander, 2);
  ExpanderPort expPortEcho2(expander, 4);

  _timer.reset(new Timer());
  _sensor.reset(new Sensor(*(_timer.get()), expPortEcho, expPortTrig));
  Sensor sensor2(*(_timer.get()), expPortEcho2, expPortTrig2);

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

  while (1) {
    int i = _sensor->getDistance();
    int i2 = sensor2.getDistance();
    trace_printf("distance: %d\n", i);
    trace_printf("distance2: %d\n", i2);
    if (i < 400) {
      leftStop();
      rightStop();
    } else {
      leftForward();
      rightForward();
    }
  }
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
