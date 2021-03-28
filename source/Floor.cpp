#include "Floor.h"
using namespace cugl;

Floor::Floor() {}

void Floor::dispose() {
	ConstructionElement::dispose();
}

bool Floor::init(int x, float ang, Vec2 scale, int level, Color4 color, int rows, int columns, std::shared_ptr<Texture> texture)
{
	setSceneNode(scene2::AnimationNode::alloc(texture, rows, columns));
	setTexture(texture);
	setPos(Vec2(x, level * FLOOR_HEIGHT + FLOOR_OFFSET+30));
	setAngle(ang);
	setScale(scale);
	setLevel(level);
	setColor(color);
	return true;
}