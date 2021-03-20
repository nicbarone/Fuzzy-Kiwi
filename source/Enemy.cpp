#include "Enemy.h"

using namespace cugl;

/** temp def for enemy movement, will be more variable later*/
#define PATROL_START 50
#define PATROL_END 500

Enemy::Enemy() :

	_patrolStart(PATROL_START),
	_patrolEnd(PATROL_END),
	_visionRange(DEFAULT_VISION),
	_speed(ENEMY_SPEED),
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

bool Enemy::init(float x, int level, float ang, std::shared_ptr<Texture> enemy, std::shared_ptr<Texture> alt) {
	Entity::setPos(x);
	Entity::setAngle(0);
	Entity::setLevel(level);
	_texture = enemy;
	_altTexture = alt;
	_isActive = true;
	_sceneNode = scene2::AnimationNode::alloc(_texture, 1, 1);
	_sceneNode->setPosition(Vec2(x, level * FLOOR_HEIGHT + FLOOR_OFFSET));
	return true;
}

void Enemy::move(float direction) {
	float original = Entity::getPos();
	if (_isActive) {
		if (_movingRight) {
			Entity::setVelocity(Vec2(_speed,0));
		}
		else {
			Entity::setVelocity(Vec2(-_speed, 0));
		}
		Entity::setPos(original + Entity::getVelocity().x);
		_sceneNode->setPositionX(original + Entity::getVelocity().x);
		if (Entity::getPos() >= _patrolEnd) {
			_movingRight = false;
		}
		else if (Entity::getPos() <= _patrolStart) {
			_movingRight = true;
		}
	}
	else if (_isPossessed) {
		manualMove(direction, ENEMY_SPEED);
		_sceneNode->setPositionX(original + getVelocity().x);
	}
}

void Enemy::setPossessed() {
	_isPossessed = true;
	_isActive = false;
	_sceneNode->setTexture(_altTexture);
}

void Enemy::setLevel(int level) {
	Entity::setLevel(level);
	_sceneNode->setPositionY(Entity::getLevel() * FLOOR_HEIGHT + FLOOR_OFFSET);
}

