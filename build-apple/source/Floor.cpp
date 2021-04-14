#include "Floor.h"
using namespace cugl;

Floor::Floor() {}

void Floor::dispose() {
	ConstructionElement::dispose();
}

bool Floor::init(Vec2 pos, float ang, Vec2 scale, int level, Color4 color, std::vector<Vec2> vertices, std::shared_ptr<Texture> texture)
{
	ConstructionElement::setSceneNode(scene2::PolygonNode::allocWithTexture(texture, vertices));
	ConstructionElement::setTexture(texture);
	ConstructionElement::setPos(pos);
	ConstructionElement::setAngle(ang);
	ConstructionElement::setScale(scale);
	ConstructionElement::setLevel(level);
	ConstructionElement::setColor(color);
	return true;
}