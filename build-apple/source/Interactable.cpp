#include "Interactable.h"
using namespace cugl;

Interactable::Interactable()
{
	_pos = cugl::Vec2::ZERO;
	_angle = 0;
	_scale = cugl::Vec2::ZERO;
	_level = 0;
	_color = cugl::Color4::WHITE;
	_texture = nullptr;
	_sceneNode = nullptr;

}



void Interactable::dispose() {
	_pos = cugl::Vec2::ZERO;
	_scale = cugl::Vec2::ZERO;
	_angle = 0;
	_scale = cugl::Vec2::ZERO;
	_level = 0;
	_color = cugl::Color4::WHITE;
	_texture = nullptr;
	_sceneNode = nullptr;


}

