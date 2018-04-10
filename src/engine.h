/*
 * Engine.h
 *
 *  Created on: Mar 20, 2018
 *      Author: jakub
 */

#ifndef ENGINE_H_
#define ENGINE_H_

#include "iengine.h"

class Engine: public IEngine {
public:
	Engine();
	virtual ~Engine();

	void startLeftForward() override;
	void startRightForward() override;
	void stopLeft() override;
	void stopRight() override;
};

#endif /* ENGINE_H_ */
