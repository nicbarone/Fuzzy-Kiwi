#include "Wall.h"
using namespace cugl;

Wall::Wall() {}

#define WALL_OFFSET 45
void Wall::dispose() {
	ConstructionElement::dispose();
}

bool Wall::init(int x, float ang, Vec2 scale, int level, Color4 color, int rows, int columns, std::shared_ptr<Texture> texture)
{
	setSceneNode(scene2::AnimationNode::alloc(texture, rows, columns));
	setTexture(texture);
	setPos(Vec2(x, level * FLOOR_HEIGHT + FLOOR_OFFSET + WALL_OFFSET));
	setAngle(ang);
	setScale(scale);
	setLevel(level);
	setColor(color);
	return true;
}