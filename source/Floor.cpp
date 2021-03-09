#include "Floor.h"
using namespace cugl;

Floor::Floor()
{

	_sceneNode = nullptr;

}


void Floor::dispose() {
	_sceneNode = nullptr;
	ConstructionElement::dispose();
}

bool Floor::init(float x, float y, float ang, int level, Color4 color, std::vector<Vec2> vertices, std::shared_ptr<Texture> cat)
{
	ConstructionElement::setTexture(cat);
	ConstructionElement::setVertices(vertices);
	_sceneNode = scene2::PolygonNode::allocWithTexture(cat, vertices);
	ConstructionElement::setPos(Vec2(x, y));
	ConstructionElement::setAngle(ang);
	ConstructionElement::setLevel(level);
	ConstructionElement::setColor(color);
	return true;
}