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
	/*list of keys that the door.*/
	std::vector<int> _keys;
public:

	Door();

	~Door() { dispose(); }

	void dispose();

	bool init(int x, float ang, Vec2 scale, int level, Color4 color, std::vector<int> keys, int rows, int columns,
		std::shared_ptr<Texture> unlockedDoor, std::shared_ptr<Texture> greenLockedDoor, std::shared_ptr<Texture> pinkLockedDoor, 
		std::shared_ptr<Texture> redLockedDoor, std::shared_ptr<Texture> blueLockedDoor);

	static std::shared_ptr<Door> alloc(int x, float ang, Vec2 scale, int level, Color4 color, std::vector<int> keys, int rows, int columns,
		std::shared_ptr<Texture> unlockedDoor,std::shared_ptr<Texture> greenLockedDoor, std::shared_ptr<Texture> pinkLockedDoor,
		std::shared_ptr<Texture> redLockedDoor, std::shared_ptr<Texture> blueLockedDoor) {
		std::shared_ptr<Door> result = std::make_shared<Door>();
		return (result->init(x, ang, scale, level, color, keys, rows,  columns, unlockedDoor,
			greenLockedDoor, pinkLockedDoor, redLockedDoor, blueLockedDoor) ? result : nullptr);
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

	void doorOpenAnimation();
};

#endif/* __DOOR_H__ */