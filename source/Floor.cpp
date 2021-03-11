#include "Floor.h"
using namespace cugl;

Floor::Floor() {}

void Floor::dispose() {
	ConstructionElement::dispose();
}

bool Floor::init(Vec2 pos, float ang, Vec2 scale, int level, Color4 color, std::vector<Vec2> vertices, std::shared_ptr<Texture> cat)
{
	ConstructionElement::setSceneNode(scene2::PolygonNode::allocWithTexture(cat, vertices));
	ConstructionElement::setTexture(cat);
	ConstructionElement::setPos(pos);
	ConstructionElement::setAngle(ang);
	ConstructionElement::setScale(scale);
	ConstructionElement::setLevel(level);
	ConstructionElement::setColor(color);
	return true;
}