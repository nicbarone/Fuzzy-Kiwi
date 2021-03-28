#pragma once
#ifndef __DOOR_H__
#define __DOOR_H__
#include "ConstructionElement.h"
#include "Enemy.h"
#include <cugl/cugl.h>
#include "Constants.h"
using namespace cugl;

class Door : public ConstructionElement {
private:
	std::shared_ptr<Enemy> _blockedEnemy;
	int _frame;
	int _frameCounter;
	bool _isOpen;
public:

	Door();

	~Door() { dispose(); }

	void dispose();

	bool init(int x, float ang, Vec2 scale, int level, Color4 color, int rows, int columns,
		std::shared_ptr<Texture> texture);

	static std::shared_ptr<Door> alloc(int x, float ang, Vec2 scale, int level, Color4 color, int rows, int columns,
		std::shared_ptr<Texture> texture) {
		std::shared_ptr<Door> result = std::make_shared<Door>();
		return (result->init(x, ang, scale, level, color,  rows,  columns, texture) ? result : nullptr);
	}
	
	void setBlockedEnemy(shared_ptr<Enemy> blockedEnemy) {
		_blockedEnemy = blockedEnemy;
	}
	shared_ptr<Enemy> getBlockedEnemy() {
		return _blockedEnemy;
	}

	void setIsOpen(bool open) {
		_isOpen = open;
	}

	bool getIsOpen() {
		return _isOpen;
	}

	void setDoor(bool open);

	void delay(int time);

	/*void setDoorFrame(shared_ptr<scene2::AnimationNode> node, int frame) {
		node->setFrame(frame);

	}*/


};

#endif/* __DOOR_H__ */