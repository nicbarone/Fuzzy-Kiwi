#pragma once
#ifndef __STAIRCASEDOOR_H__
#define __STAIRCASEDOOR_H__
#include "ConstructionElement.h"
#include "Enemy.h"
#include <cugl/cugl.h>
#include "Constants.h"
using namespace cugl;

class StaircaseDoor : public ConstructionElement {
private:
	int _frame;
	int _frameCounter;
	bool _isOpen;
public:

	StaircaseDoor();

	~StaircaseDoor() { dispose(); }

	void dispose();

	bool init(int x, float ang, Vec2 scale, int level, Color4 color, int rows, int columns,
		std::shared_ptr<Texture> texture);

	static std::shared_ptr<StaircaseDoor> alloc(int x, float ang, Vec2 scale, int level, Color4 color, int rows, int columns,
		std::shared_ptr<Texture> texture) {
		std::shared_ptr<StaircaseDoor> result = std::make_shared<StaircaseDoor>();
		return (result->init(x, ang, scale, level, color, rows, columns, texture) ? result : nullptr);
	}

	void setIsOpen(bool open) {
		_isOpen = open;
	}

	bool getIsOpen() {
		return _isOpen;
	}

	void setDoor(bool open);

	void doorOpenAnimation();
};

#endif/* __STAIRCASEDOOR_H__ */