#include "Door.h"
using namespace cugl;

Door::Door() {
	_blockedEnemy = nullptr;
}

void Door::dispose() {
	ConstructionElement::dispose();
	_blockedEnemy = nullptr;
}

bool Door::init(Vec2 pos, float ang, Vec2 scale, int level, Color4 color, int rows, int columns, std::shared_ptr<Texture> texture)
{
	setSceneNode(scene2::AnimationNode::alloc(texture, rows, columns));
	//std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(0);
	setTexture(texture);
	setPos(pos);
	setAngle(ang);
	setScale(scale);
	setLevel(level);
	setColor(color);
	_blockedEnemy = nullptr;
	return true;
}

void Door::setVisibility(bool visibility) {
	getSceneNode()->setVisible(visibility);
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