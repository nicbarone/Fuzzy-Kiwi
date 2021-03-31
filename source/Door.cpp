#include "Door.h"
using namespace cugl;

#define DOOR_OFFSET 10
Door::Door() :
	_frame(0),
	_frameCounter(9)
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
	setPos(Vec2(x, level * FLOOR_HEIGHT + FLOOR_OFFSET- DOOR_OFFSET));
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

	if (open) {
		
		for (int i = 0; i < 10; i++) {
			std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(i);
		}
		/*cugl::Application::schedule(Door::doorOpenAnimation, 3);*/
	}
	else
	{
		for (int i = 9; i > -1; i--) {
			std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(i);
		}
	}
	//std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(_frame);
	if (_blockedEnemy != nullptr) {
		//CULog("door's old patrol: %d", _blockedEnemy->getOldPatrol().y);
		Vec2 temp = Vec2(_blockedEnemy->getPatrol().x, _blockedEnemy->getPatrol().y);
		_blockedEnemy->setPatrol(_blockedEnemy->getOldPatrol().x, _blockedEnemy->getOldPatrol().y);
		_blockedEnemy->setOldPatrol(temp);
		_blockedEnemy = nullptr;
		//setBlockedEnemy(nullptr);
		CULog("%d", _blockedEnemy == nullptr);

	}
	
}

void  Door::doorOpenAnimation() {
	int i = 0;
	std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(i);
}