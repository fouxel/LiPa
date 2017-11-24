/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
//#include "stm32f10x_exti.h"
//#include "diag/Trace.h"

#include <stdint.h>
#include "stm32f10x.h"


#include "Timer.h"
//#include "BlinkLed.h"

// ----------------------------------------------------------------------------
//
// Standalone STM32F1 led blink sample (trace via DEBUG).
//
// In debug configurations, demonstrate how to print a greeting message
// on the trace device. In release configurations the message is
// simply discarded.
//
// Then demonstrates how to blink a led with 1 Hz, using a
// continuous loop and SysTick delays.
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//
// The external clock frequency is specified as a preprocessor definition
// passed to the compiler via a command line option (see the 'C/C++ General' ->
// 'Paths and Symbols' -> the 'Symbols' tab, if you want to change it).
// The value selected during project creation was HSE_VALUE=8000000.
//
// Note: The default clock settings take the user defined HSE_VALUE and try
// to reach the maximum possible system clock. For the default 8 MHz input
// the result is guaranteed, but for other values it might not be possible,
// so please adjust the PLL settings in system/src/cmsis/system_stm32f10x.c
//

// ----- Timing definitions -------------------------------------------------

// Keep the LED on for 2/3 of a second.
#define BLINK_ON_TICKS  (TIMER_FREQUENCY_HZ * 3 / 4)
#define BLINK_OFF_TICKS (TIMER_FREQUENCY_HZ - BLINK_ON_TICKS)

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"


uint16_t _ports[] = { GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3 };


void send_char(char c) {
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	USART_SendData(USART2, c);
}

void send_string(char *s) {
	while(*s) {
		send_char(*s++);
	}
}

void send_string_len(char *s, int len) {
	for (int i = 0; i < len; i++) {
		send_char(*s++);
	}
}

void turnIdx(int idx) {
	for (int i = 0; i < 4; ++i) {
		 GPIO_ResetBits(GPIOC, _ports[i]);
	}
	GPIO_SetBits(GPIOC, _ports[idx]);

}

volatile uint32_t _timerUs = 0; //deklaracja zmiennej 32 bitowej bez znaku
volatile int _timerOn = 0;

static void timer_startTimer() {
	_timerUs = 0;
	_timerOn = 1;
}

static uint32_t timer_stopTimer() {
	_timerOn = 0;
	return _timerUs;
}

void TIM2_IRQHandler()
{
 if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
 {
	 TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		if(_timerOn) {
			_timerUs++;
		}
 }
}
float calc_pwm(float val)
{
 const float k = 0.1f;
 const float x0 = 60.0f;
 return 300.0f / (1.0f + exp(-k * (val - x0)));
}

static void generatePulse() {
	timer_startTimer();

	GPIO_SetBits(GPIOC, GPIO_Pin_5);
	while(_timerUs < 20) {
	}
	timer_stopTimer();
	GPIO_ResetBits(GPIOC, GPIO_Pin_5);
}

static int waitMillis(int millis)  {
	int waitTime = 2000 * millis;
	timer_startTimer();
	while(_timerUs < waitTime) {
	}
	timer_stopTimer();
}

static int getDistance() {
	int delay = 0;
	generatePulse();
	while(1) {
		if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) == 1) {
			timer_startTimer();
			while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) == 1) {
				if(_timerUs > 10000) {
					return -1;
				}
			}
			delay = timer_stopTimer();
			break;
		}
	}
	return delay;
}

int main(void)
{
	//PC5 jako wyjscie TRIG
	//PC6 jako wejscie ECHO

	TIM_TimeBaseInitTypeDef tim;
	NVIC_InitTypeDef nvic;

	GPIO_InitTypeDef gpio;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	// Konfiguracja do silnika (PC0, PC1) oraz TRIG do czujnika odległości(PC5)
	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_5;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &gpio);

	gpio.GPIO_Pin = GPIO_Pin_6;
	gpio.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOC, &gpio);


	TIM_TimeBaseStructInit(&tim);
	tim.TIM_CounterMode = TIM_CounterMode_Up;
	tim.TIM_Prescaler = 32 - 1;
	tim.TIM_Period = 2;
	TIM_TimeBaseInit(TIM2, &tim);

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);

	nvic.NVIC_IRQChannel = TIM2_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	timer_start();

	while(1) {
		int i = getDistance();
		trace_printf("distance: %d\n", i);
		if(i > 200) {
			int count = 100;
			while (count--) {
				GPIO_SetBits(GPIOC, 3);
				timer_sleep(1);
				GPIO_ResetBits(GPIOC, 3);
				timer_sleep(20);
			}
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
	//blink_led_off();
/* int counter = 0;
 GPIO_InitTypeDef gpio;
 TIM_TimeBaseInitTypeDef tim;
 TIM_OCInitTypeDef  channel;
 NVIC_InitTypeDef nvic;

 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
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
