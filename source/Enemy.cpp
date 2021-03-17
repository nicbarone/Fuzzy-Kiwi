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
	_altTexture = nullptr;
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
	_altTexture = nullptr;
	Entity::dispose();
}

bool Enemy::init(float x, float y, int level, float ang, std::shared_ptr<Texture> enemy, std::shared_ptr<Texture> alt) {
	Entity::setPos(Vec2(x, y));
	Entity::setAngle(0);
	Entity::setLevel(level);
	_texture = enemy;
	_altTexture = alt;
	_isActive = true;
	_sceneNode = scene2::AnimationNode::alloc(_texture, 1, 1);
	_sceneNode->setPosition(Vec2(x, y));
	return true;
}

void Enemy::move(float direction) {
	Vec2 original = Entity::getPos();
	if (_isActive) {
		if (_movingRight) {
			Entity::setVelocity(Vec2(_speed,0));
		}
		else {
			Entity::setVelocity(Vec2(-_speed, 0));
		}
		Entity::setPos(Vec2(original + Entity::getVelocity()));
		_sceneNode->setPosition(original + Entity::getVelocity());
		if (Entity::getPos().x > _patrolEnd) {
			_movingRight = false;
		}
		else if (Entity::getPos().x < _patrolStart) {
			_movingRight = true;
		}
	}
	else if (_isPossessed) {
		manualMove(direction, SPEED);
		_sceneNode->setPosition(original + getVelocity());
	}
}

void Enemy::setPossessed() {
	_isPossessed = true;
	_isActive = false;
	_sceneNode->setTexture(_altTexture);
}

void Enemy::changeFloor() {
	_sceneNode->setPositionY(Entity::getPos().y);
}

