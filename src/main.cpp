#include <cmath>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

#include "stm32f10x.h"
#include <stdint.h>

#include "Timer.h"
#include "expander.hpp"
#include "gpio.hpp"


// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

volatile uint32_t _timerUs = 0;
volatile int _timerOn = 0;

static void timer_startTimer() {
  _timerUs = 0;
  _timerOn = 1;
}

static uint32_t timer_stopTimer() {
  _timerOn = 0;
  return _timerUs;
}

extern "C" {
void TIM2_IRQHandler() {
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    if (_timerOn) {
      _timerUs++;
    }
  }
}
}

static void generatePulse() {
  timer_startTimer();

  GPIO_SetBits(GPIOC, GPIO_Pin_5);
  while (_timerUs < 20) {
  }
  timer_stopTimer();
  GPIO_ResetBits(GPIOC, GPIO_Pin_5);
}

static int waitMillis(int millis) {
  int waitTime = 2000 * millis;
  timer_startTimer();
  while (_timerUs < waitTime) {
  }
  timer_stopTimer();
}

static int getDistance() {
  int delay = 0;
  generatePulse();
  while (1) {
    if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) == 1) {
      timer_startTimer();
      while (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) == 1) {
        if (_timerUs > 10000) {
          return -1;
        }
      }
      delay = timer_stopTimer();
      break;
    }
  }
  return delay;
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

int main(void) {
  // PC5 jako wyjscie TRIG
  // PC6 jako wejscie ECHO

  TIM_TimeBaseInitTypeDef tim;
  NVIC_InitTypeDef nvic;

  GPIO_InitTypeDef gpio;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                             RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD,
                         ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM4, ENABLE);

  // EKSPANDER
  Expander expander;

  //   TRIG do czujnika odległości(PC5) oraz ECHO (PC6)
  GPIO_StructInit(&gpio);
  gpio.GPIO_Pin = GPIO_Pin_5;
  gpio.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &gpio);

  gpio.GPIO_Pin = GPIO_Pin_6;
  gpio.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(GPIOC, &gpio);

  // Ustawienia silników PC6, PC7
  gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  gpio.GPIO_Speed = GPIO_Speed_50MHz;
  gpio.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &gpio);

  // Konfiguracja timera do obsługi czujnika odległości
  TIM_TimeBaseStructInit(&tim);
  tim.TIM_CounterMode = TIM_CounterMode_Up;
  tim.TIM_Prescaler = 32 - 1;
  tim.TIM_Period = 2;
  TIM_TimeBaseInit(TIM2, &tim);

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
  //timer_start();

  while (1) {
    int i = getDistance();
    trace_printf("distance: %d\n", i);
    if (i < 400) {
      leftStop();
      rightStop();
      expander.resetPin(0); // mcp_write_reg(MCP_OLAT, 0x00);
      //			timer_sleep(10000);
      //			rightForward();
      //			timer_sleep(1000);
      //			rightStop();
    } else {
      leftForward();
      rightForward();
      expander.setPin(0);
      // mcp_write_reg(MCP_OLAT, 0x01);
    }
  }

  /*
          timer_start();
          blink_led_init();
          int count = 25;
          while(count--) {
                  GPIO_SetBits(GPIOC, 1);
                  timer_sleep(1);
                  GPIO_ResetBits(GPIOC, 1);
                  timer_sleep(20);
          }

          count = 25;
          while(count--) {
                  GPIO_SetBits(GPIOC, 2);
                  timer_sleep(1);
                  GPIO_ResetBits(GPIOC, 2);
                  timer_sleep(20);
          }

          count = 100;
          while(count--) {
                  GPIO_SetBits(GPIOC, 3);
                  timer_sleep(1);
                  GPIO_ResetBits(GPIOC, 3);
                  timer_sleep(20);
          }
          */
  // blink_led_off();
  /* int counter = 0;
   GPIO_InitTypeDef gpio;
   TIM_TimeBaseInitTypeDef tim;
   TIM_OCInitTypeDef  channel;
   NVIC_InitTypeDef nvic;

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
  RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

   GPIO_StructInit(&gpio);
   gpio.GPIO_Pin = GPIO_Pin_5;
   gpio.GPIO_Speed = GPIO_Speed_2MHz;
   gpio.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_Init(GPIOA, &gpio);

   gpio.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
   gpio.GPIO_Mode = GPIO_Mode_AF_PP;
   GPIO_Init(GPIOB, &gpio);
   GPIO_SetBits(GPIOB, GPIO_Pin_6);
   GPIO_SetBits(GPIOB, GPIO_Pin_7);
   GPIO_SetBits(GPIOB, GPIO_Pin_8);

   TIM_TimeBaseStructInit(&tim);
   tim.TIM_CounterMode = TIM_CounterMode_Up;
   tim.TIM_Prescaler = 64 - 1;
   tim.TIM_Period = 1000 - 1;
   TIM_TimeBaseInit(TIM4, &tim);

   TIM_OCStructInit(&channel);
   channel.TIM_OCMode = TIM_OCMode_PWM2;
   channel.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OC1Init(TIM4, &channel);
   TIM_OC2Init(TIM4, &channel);
   TIM_OC3Init(TIM4, &channel);

   TIM_Cmd(TIM4, ENABLE);

   nvic.NVIC_IRQChannel = TIM4_IRQn;
   nvic.NVIC_IRQChannelPreemptionPriority = 0;
   nvic.NVIC_IRQChannelSubPriority = 0;
   nvic.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&nvic);

   SysTick_Config(SystemCoreClock / 1000);

  // float values[3][3] = {{0, 0, 0},
  //  	  	  	  	  	   {50000, 50000, 0},
  // 	 	 	 	 	   {0, 50000, 0}};

   while (1) {

   float r = 50 * (1.0f + sin(counter / 100.0f));
   float g = 50 * (1.0f + sin(1.5f * counter / 100.0f));
   float b = 50 * (1.0f + sin(2.0f * counter / 100.0f));
   TIM_SetCompare1(TIM4, calc_pwm(r));
    TIM_SetCompare2(TIM4, calc_pwm(g));
    TIM_SetCompare3(TIM4, calc_pwm(b));
   timer_sleep(20);
   counter++;
  //counter++;
  // if(counter >= 3) {
  //	 counter = 0;
  // }
   }
   */
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
