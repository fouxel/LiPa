/*
 * Expander.h
 *
 *  Created on: Jan 7, 2018
 *      Author: jakub
 */

#ifndef EXPANDER_HPP_
#define EXPANDER_HPP_

#include "periph.hpp"

#define MCP_IODIR		0x00
#define MCP_IPOL		0x01
#define MCP_GPINTEN		0x02
#define MCP_DEFVAL		0x03
#define MCP_INTCON		0x04
#define MCP_IOCON		0x05
#define MCP_GPPU		0x06
#define MCP_INTF		0x07
#define MCP_INTCAP		0x08
#define MCP_GPIO		0x09
#define MCP_OLAT		0x0a

class Expander {
public:
	Expander();
	virtual ~Expander();

	void init();
};

#endif /* EXPANDER_HPP_ */
