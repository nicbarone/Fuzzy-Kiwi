#include "Entity.h"

Entity::Entity() :
	_angle(0),
	_level(-1)
{
	_pos = cugl::Vec2::ZERO;
	_size = cugl::Vec2::ZERO;
	_color = cugl::Color4::WHITE;

}


void Entity::dispose() {
	_pos = cugl::Vec2::ZERO;
	_size = cugl::Vec2::ZERO;
	_angle = 0;
	_color = cugl::Color4::WHITE;
}