#include "Door.h"
using namespace cugl;

Door::Door() {}

void Door::dispose() {
	ConstructionElement::dispose();
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
	return true;
}