/*
 * Expander.cpp
 *
 *  Created on: Jan 7, 2018
 *      Author: jakub
 */

#include "expander.hpp"
#include "gpio.hpp"

namespace {

#define MCP_IODIR 0x00
#define MCP_IPOL 0x01
#define MCP_GPINTEN 0x02
#define MCP_DEFVAL 0x03
#define MCP_INTCON 0x04
#define MCP_IOCON 0x05
#define MCP_GPPU 0x06
#define MCP_INTF 0x07
#define MCP_INTCAP 0x08
#define MCP_GPIO 0x09
#define MCP_OLAT 0x0a

uint8_t spi_sendrecv(uint8_t byte) {
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
    ;
  SPI_I2S_SendData(SPI1, byte);

  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
    ;
  return SPI_I2S_ReceiveData(SPI1);
}

} // namespace

Expander::Expander(uint8_t dir) : gpioCS(GPIO_Pin_0, GPIOC, GPIO_Mode_Out_PP) {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

  GPIO gpioSCK_MOSI(GPIO_Pin_5 | GPIO_Pin_7, GPIOA, GPIO_Mode_AF_PP);
  gpioSCK_MOSI.init();

  GPIO gpioMISO(GPIO_Pin_6, GPIOA, GPIO_Mode_IN_FLOATING);
  gpioMISO.init();

  gpioCS.init();
  gpioCS.set();

  SPI_InitTypeDef spi;
  SPI_StructInit(&spi);
  spi.SPI_Mode = SPI_Mode_Master;
  spi.SPI_NSS = SPI_NSS_Soft;
  spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
  SPI_Init(SPI1, &spi);
  SPI_Cmd(SPI1, ENABLE);

  //writeReg(MCP_IODIR, ~0x01);
  writeReg(MCP_IODIR, dir); //~0x03);
  m_pin = 0;
}

Expander::~Expander() {}

void Expander::init() {}

void Expander::setPin(pin_t pin) {
	m_pin |= 1 << pin;
	writeReg(MCP_OLAT, m_pin);
}

void Expander::resetPin(pin_t pin) {
	m_pin &= ~(1 << pin);
	writeReg(MCP_OLAT, m_pin);
}

bool Expander::readPin(pin_t pin) { return (readReg(MCP_GPIO) & pin); }

void Expander::writeReg(uint8_t addr, uint8_t value) {
  gpioCS.reset();
  GPIO_ResetBits(GPIOC, GPIO_Pin_0);
  spi_sendrecv(0x40);
  spi_sendrecv(addr);
  spi_sendrecv(value);
  gpioCS.set();
  GPIO_SetBits(GPIOC, GPIO_Pin_0);
}

uint8_t Expander::readReg(uint8_t addr) {
  gpioCS.reset();
  spi_sendrecv(0x41);
  spi_sendrecv(addr);
  uint8_t value = spi_sendrecv(0xff);
  gpioCS.set();
  return value;
}
