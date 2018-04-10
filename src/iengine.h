/*
 * iengine.h
 *
 *  Created on: Mar 20, 2018
 *      Author: jakub
 */

#ifndef IENGINE_H_
#define IENGINE_H_

template <class Engine>
void startForward(Engine &engine) {
	engine.startLeftForward();
	engine.startRightForward();
}

template <class Engine>
void stop(Engine &engine) {
	engine.stopLeft();
	engine.stopRight();
}

class IEngine {
public:
	virtual ~IEngine() {}

	virtual void startLeftForward() = 0;
	virtual void startRightForward() = 0;
	virtual void stopLeft() = 0;
	virtual void stopRight() = 0;
};

#endif /* IENGINE_H_ */
