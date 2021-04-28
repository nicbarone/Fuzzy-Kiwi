#include "CagedAnimal.h"
using namespace cugl;

#define CAGED_ANIMAL_OFFSET -10
CagedAnimal::CagedAnimal() :
	_frame(0),
	_frameCounter(8)
{}

void CagedAnimal::dispose() {
	ConstructionElement::dispose();
}

bool CagedAnimal::init(int x, float ang, Vec2 scale, int level, Color4 color, std::vector<int> keys, int rows, int columns, std::shared_ptr<Texture> texture)
{
	setSceneNode(scene2::AnimationNode::alloc(texture, rows, columns));
	setTexture(texture);
	setPos(Vec2(x, level * FLOOR_HEIGHT + FLOOR_OFFSET + CAGED_ANIMAL_OFFSET));
	setAngle(ang);
	setScale(scale);
	setLevel(level);
	setColor(color);
	_keys = keys;
	_frame = 0;
	_isOpen = false;
	getSceneNode()->setPriority(level);
	return true;
}
