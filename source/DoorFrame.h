#pragma once
#ifndef __DOORFRAME_H__
#define __DOORFRAME_H__
#include "ConstructionElement.h"
#include "Enemy.h"
#include <cugl/cugl.h>
#include "Constants.h"
using namespace cugl;

class DoorFrame : public ConstructionElement {
private:
	std::shared_ptr<Enemy> _blockedEnemy;
	int _frame;
	int _frameCounter;
	bool _isOpen;
	/*list of keys that the door.*/
	std::vector<int> _keys;
public:

	DoorFrame();

	~DoorFrame() { dispose(); }

	void dispose();

	bool init(int x, float ang, Vec2 scale, int level, Color4 color, std::vector<int> keys, int rows, int columns,
		std::shared_ptr<Texture> texture);

	static std::shared_ptr<DoorFrame> alloc(int x, float ang, Vec2 scale, int level, Color4 color, std::vector<int> keys, int rows, int columns,
		std::shared_ptr<Texture> texture) {
		std::shared_ptr<DoorFrame> result = std::make_shared<DoorFrame>();
		return (result->init(x, ang, scale, level, color, keys, rows, columns, texture) ? result : nullptr);
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

	void setKeys(std::vector<int> keys) {
		_keys = keys;
	}

	std::vector<int> getKeys() {
		return _keys;
	}
	//sets the door to play either the open or close door animation based on the open parameter.
	void setDoor(bool open);


	/*void setDoorFrame(shared_ptr<scene2::AnimationNode> node, int frame) {
		node->setFrame(frame);

	}*/

};

#endif/* __DOORFRAME_H__ */