#include "Door.h"
using namespace cugl;

Door::Door() {}

void Door::dispose() {
	ConstructionElement::dispose();
	_blockedEnemy = nullptr;
}

bool Door::init(Vec2 pos, float ang, Vec2 scale, int level, Color4 color, std::shared_ptr<Texture> texture)
{
	ConstructionElement::setSceneNode(scene2::AnimationNode::alloc(texture, 1, 1));
	ConstructionElement::setTexture(texture);
	ConstructionElement::setPos(pos);
	ConstructionElement::setAngle(ang);
	ConstructionElement::setScale(scale);
	ConstructionElement::setLevel(level);
	ConstructionElement::setColor(color);
	_blockedEnemy = nullptr;
	return true;
}

void Door::setVisibility(bool visibility) {
	if (_blockedEnemy != nullptr) {
		//CULog("door's old patrol: %d", _blockedEnemy->getOldPatrol().y);
		_blockedEnemy->setPatrol(_blockedEnemy->getOldPatrol().x, _blockedEnemy->getOldPatrol().y);
		_blockedEnemy->setOldPatrol(Vec2(0, 0));
		setBlockedEnemy(nullptr);
	}
	ConstructionElement::getSceneNode()->setVisible(visibility);
}