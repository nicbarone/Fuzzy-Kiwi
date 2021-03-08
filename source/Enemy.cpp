#include "Enemy.h"
using namespace cugl;

/** temp def for enemy movement, will be more variable later*/
#define SPEED 5
#define PATROL_START 50
#define PATROL_END 500

Enemy::Enemy() :

	_patrolStart(PATROL_START),
	_patrolEnd(PATROL_END),
	_visionRange(0),
	_speed(SPEED),
	_movingRight(true),
	_isStuck(false),
	_isPossessed(false),
	_isActive(false)
{

	_sceneNode = nullptr;
	_texture = nullptr;
}


void Enemy::dispose() {
	_patrolStart = 0;
	_patrolEnd = 0;
	_visionRange = 0;
	_isStuck = false;
	_movingRight = false;
	_isPossessed = false;
	_isActive = false;
	_texture = nullptr;
	_sceneNode = nullptr;
	Entity::dispose();
}

bool Enemy::init(float x, float y, float ang, std::shared_ptr<Texture> enemy)
{
	Entity::setPos(Vec2(x, y));
	Entity::setAngle(0);
	_texture = enemy;
	_sceneNode = scene2::AnimationNode::alloc(_texture, 1, 1);
	_sceneNode->setPosition(Vec2(x, y));
	return true;
}

void Enemy::move() {
	if (_movingRight) {
		Entity::setVelocity(_speed);
	}
	else {
		Entity::setVelocity(-_speed);
	}
	Vec2 original = Entity::getPos();
	Entity::setPos(Vec2(original.x + Entity::getVelocity(), original.y));
	_sceneNode->setPositionX(original.x + Entity::getVelocity());
	if (Entity::getPos().x > _patrolEnd) {
		_movingRight = false;
	}
	else if (Entity::getPos().x < _patrolStart) {
		_movingRight = true;
	}
}


