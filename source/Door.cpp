#include "Door.h"
using namespace cugl;

Door::Door() {}

void Door::dispose() {
	Interactable::dispose();
}

bool Door::init(Vec2 pos, float ang, Vec2 scale, int level, Color4 color, std::shared_ptr<Texture> texture)
{
	Interactable::setSceneNode(scene2::AnimationNode::alloc(texture, 1, 1));
	Interactable::setTexture(texture);
	Interactable::setPos(pos);
	Interactable::setAngle(ang);
	Interactable::setScale(scale);
	Interactable::setLevel(level);
	Interactable::setColor(color);
	return true;
}