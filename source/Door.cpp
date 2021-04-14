#include "Door.h"
using namespace cugl;

#define DOOR_OFFSET 66
Door::Door() :
	_frame(0),
	_frameCounter(8)
{
	_keys = { 0 }; 
	_blockedEnemy = nullptr;
	_isOpen = false;

}

void Door::dispose() {
	ConstructionElement::dispose();
	_blockedEnemy = nullptr;
	_isOpen = false;
	_keys = { 0 };
}

bool Door::init(int x, float ang, Vec2 scale, int level, Color4 color, std::vector<int> keys, int rows, int columns, std::shared_ptr<Texture> texture)
{
	setSceneNode(scene2::AnimationNode::alloc(texture, rows, columns));
	setTexture(texture);
	setPos(Vec2(x, level * FLOOR_HEIGHT + FLOOR_OFFSET + DOOR_OFFSET));
	setAngle(ang);
	setScale(scale);
	setLevel(level);
	setColor(color);
	_keys = keys;
	_frame = 0;
	_blockedEnemy = nullptr;
	_isOpen = false;
	return true;
}

void Door::setDoor(bool open) {
	_isOpen = open;
	std::function<bool()> frame0 = [this]() {
		std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(_frameCounter);
		_frameCounter++;
		return false;
	};
	std::function<bool()> frame1 = [this](){
		std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(_frameCounter);
		_frameCounter--;
		return false;
	};
	if (open) {
		int timeDiff = 25;
		_frameCounter = 1;
		for (int i = 1; i <= 6; i = i++) {
			cugl::Application::get()->schedule(frame0, 50 + timeDiff * i);
		}
	}
	else
	{
		int timeDiff = 25;
		_frameCounter = 6;
		for (int i = 6; i >= 0; i = i--) {
			cugl::Application::get()->schedule(frame1, 50 + timeDiff * i);
			CULog("frame counter %d", _frameCounter);
		}
	}
	//std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(_frame);
	if (_blockedEnemy != nullptr) {
		//CULog("door's old patrol: %d", _blockedEnemy->getOldPatrol().y);
		Vec2 temp = Vec2(_blockedEnemy->getPatrol().x, _blockedEnemy->getPatrol().y);
		if (_blockedEnemy->isActive()) {
			_blockedEnemy->setPatrol(_blockedEnemy->getOldPatrol().x, _blockedEnemy->getOldPatrol().y);
			_blockedEnemy->setOldPatrol(temp);
		}
		_blockedEnemy = nullptr;
		//setBlockedEnemy(nullptr);
		CULog("%d", _blockedEnemy == nullptr);

	}
	
}

void  Door::doorOpenAnimation() {
	int i = 0;
	std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(i);
}