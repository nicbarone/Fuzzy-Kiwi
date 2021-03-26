#include "Door.h"
using namespace cugl;

Door::Door() :
	_frame(0),
	_frameCounter(9)
{
	_blockedEnemy = nullptr;
	_isOpen = false;
}

void Door::dispose() {
	ConstructionElement::dispose();
	_blockedEnemy = nullptr;
	_isOpen = false;
}

bool Door::init(Vec2 pos, float ang, Vec2 scale, int level, Color4 color, int rows, int columns, std::shared_ptr<Texture> texture)
{
	setSceneNode(scene2::AnimationNode::alloc(texture, rows, columns));
	setTexture(texture);
	setPos(pos);
	setAngle(ang);
	setScale(scale);
	setLevel(level);
	setColor(color);
	_frame = 0;
	_blockedEnemy = nullptr;
	_isOpen = false;
	return true;
}

void Door::setDoor(bool open) {
	_isOpen = open;

	if (open) {
		for (int i = 0; i <10; i++) {
			std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(i);
			//cugl::Application::schedule()

		}
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