#include "Entity.h"

Entity::Entity() :
	_angle(0),
	_level(-1),
	_pos(0)
{
	_size = cugl::Vec2::ZERO;
	_color = cugl::Color4::WHITE;

}

void Entity::dispose() {
	_pos = 0;
	_size = cugl::Vec2::ZERO;
	_angle = 0;
	_color = cugl::Color4::WHITE;
}

void Entity::manualMove(float direction, float speed) 
	{
		setVelocity(Vec2(direction * speed, 0));
		setPos(getPos() + getVelocity().x);

	}