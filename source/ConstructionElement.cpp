#include "ConstructionElement.h"
using namespace cugl;

ConstructionElement::ConstructionElement()
	{
		_pos = cugl::Vec2::ZERO;
		_angle = 0;
		_scale = cugl::Vec2::ZERO;
		_level = 0;
		_color = cugl::Color4::WHITE;
		_vertices.resize(1000);
		_texture = nullptr;
		_sceneNode = nullptr;

	}



void ConstructionElement::dispose() {
	_pos = cugl::Vec2::ZERO;
	_scale = cugl::Vec2::ZERO;
	_angle = 0;
	_scale = cugl::Vec2::ZERO;
	_level = 0;
	_color = cugl::Color4::WHITE;
	_vertices.clear();
	_texture = nullptr;
	_sceneNode = nullptr;
	

}

