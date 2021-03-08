#include "Floor.h"
using namespace cugl;

Floor::Floor()
{

	_sceneNode = nullptr;

}


void Floor::dispose() {
	_texture = nullptr;
	_sceneNode = nullptr;
	Entity::dispose();
}

bool Floor::init(float x, float y, float ang, std::vector<Vec2> vertices, std::shared_ptr<Texture> cat)
{
	_texture = cat;
	_vertices = vertices;
	_sceneNode = scene2::PolygonNode::allocWithTexture(cat, vertices);
	_sceneNode->setPosition(Vec2(x, y));
	return true;
}