#include "Enemy.h"

using namespace cugl;

/** temp def for enemy movement, will be more variable later*/
#define PATROL_START 50
#define PATROL_END 600

Enemy::Enemy() :

	_patrolStart(0),
	_patrolEnd(0),
	_visionRange(DEFAULT_VISION),
	_speed(ENEMY_SPEED),
	_movingRight(true),
	_isStuck(false),
	_isPossessed(false),
	_isActive(false),
	_frame(0),
	_frameCounter(15)
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
	_frame = 0;
	_frameCounter = 0;
	Entity::dispose();
}

bool Enemy::init(float x, int level, float ang, float patrolStart, float patrolEnd,  std::shared_ptr<Texture> enemy, std::shared_ptr<Texture> alt) {
	Entity::setPos(x);
	Entity::setAngle(0);
	Entity::setLevel(level);
	_patrolStart = patrolStart;
	_patrolEnd = patrolEnd;
	_texture = enemy;
	_altTexture = alt;
	_isActive = true;
	_sceneNode = scene2::AnimationNode::alloc(_texture, 1, 5);
	_sceneNode->setPosition(Vec2(x, level * FLOOR_HEIGHT + FLOOR_OFFSET));
	_frame = 0;
	_sceneNode->setScale(0.05, 0.05);
	return true;
}

void Enemy::move(float direction) {
	float original = Entity::getPos();
	if (_isActive) {
		if (_patrolStart == _patrolEnd) {
			return;
		}
		if (_frameCounter > 0) {
			_frameCounter--;
		}
		else {
			_frameCounter = 7;
			_frame++;
			_frame = _frame % 5;
		}
		
		if (_movingRight) {
			Entity::setVelocity(Vec2(_speed,0));
			_sceneNode->flipHorizontal(false);
		}
		else {
			Entity::setVelocity(Vec2(-_speed, 0));
			_sceneNode->flipHorizontal(true);
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
	else if (_isPossessed && _sceneNode->isVisible()) {
		manualMove(direction, ENEMY_SPEED);
		_sceneNode->setPositionX(original + getVelocity().x);
		if (direction == 1) {
			_movingRight = true;
			_sceneNode->flipHorizontal(false);
		}
		else if (direction == -1) {
			_movingRight = false;
			_sceneNode->flipHorizontal(true);
		}
		if (direction != 0)
		{
			if (_frameCounter > 0) {
				_frameCounter--;
			}
			else {
				_frameCounter = 7;
				_frame++;
				_frame = _frame % 5;
			}
		}
	}
	if (_isActive || isPossessed()) {
		//_sceneNode->setFrame(_frame);
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

