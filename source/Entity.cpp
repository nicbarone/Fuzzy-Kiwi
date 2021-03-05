#include "Entity.h"

Entity::Entity() :
	_angle(0)
{
	_pos = Vec2::ZERO;
	_size = Vec2::ZERO;
	_color = Color4::WHITE;
}


void Entity::dispose() {
	_pos = Vec2::ZERO;
	_size = Vec2::ZERO;
	_angle = 0;
	_color = Color4::WHITE;
}