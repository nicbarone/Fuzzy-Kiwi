#include "ConstructionElement.h"
using namespace cugl;

ConstructionElement::ConstructionElement():
	_angle(0),
	_level(-1)
	{
		_pos = cugl::Vec2::ZERO;
		_size = cugl::Vec2::ZERO;
		_color = cugl::Color4::WHITE;
		_texture = nullptr;
		_vertices.resize(1000);

	}



void ConstructionElement::dispose() {
	_pos = cugl::Vec2::ZERO;
	_size = cugl::Vec2::ZERO;
	_angle = 0;
	_color = cugl::Color4::WHITE;
	_texture = nullptr;
	_vertices.clear();
}

