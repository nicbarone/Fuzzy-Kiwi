#include "Enemy.h"

using namespace cugl;

/** temp def for enemy movement, will be more variable later*/
#define PATROL_START 50
#define PATROL_END 800

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
	_frameCounter(0),
	_turnFrame(0)
{
	_keys = { 0 };
	_sceneNode = nullptr;
	_texture = nullptr;
	_altTexture = nullptr;
	_glowTexture = nullptr;
	_patrolNode = nullptr;
}


void Enemy::dispose() {
	_patrolStart = 0;
	_patrolEnd = 0;
	_visionRange = 0;
	_turnFrame = 0;
	_isStuck = false;
	_movingRight = false;
	_isPossessed = false;
	_isActive = false;
	_texture = nullptr;
	_sceneNode = nullptr;
	_altTexture = nullptr;
	_glowTexture = nullptr;
	_patrolNode = nullptr;
	_frame = 0;
	_keys = { 0 };
	_frameCounter = 0;
	Entity::dispose();
}

bool Enemy::init(float x, int level, float ang, std::vector<int> keys, float patrolStart, float patrolEnd,  int num_frames, std::shared_ptr<Texture> enemy, std::shared_ptr<Texture> alt, std::shared_ptr<Texture> glow) {
	Entity::setPos(x);
	Entity::setAngle(0);
	Entity::setLevel(level);
	_keys = keys;
	_patrolStart = patrolStart;
	_patrolEnd = patrolEnd;
	_texture = enemy;
	_altTexture = alt;
	_glowTexture = glow;
	_isActive = true;
	_frameCounter = 7;
	_sceneNode = scene2::AnimationNode::alloc(_texture, 1, num_frames);
	_sceneNode->setPosition(Vec2(x, level * FLOOR_HEIGHT + FLOOR_OFFSET));
	_patrolNode = scene2::WireNode::alloc(Rect(0, 0, patrolEnd - patrolStart, 2));
	_patrolNode->setPosition(Vec2((patrolStart+patrolEnd)/2, level * FLOOR_HEIGHT + FLOOR_OFFSET - 75));
	_patrolNode->setColor(Color4::RED);
	if (patrolStart == patrolEnd && patrolStart < x) {
		_movingRight = false;
		_sceneNode->setScale(-0.05, 0.05);
	}
	else {
		_movingRight = true;
		_sceneNode->setScale(0.05, 0.05);
	}
	_frame = 0;
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
		
		if (_movingRight && original < _patrolEnd) {
			Entity::setVelocity(Vec2(_speed, 0));
		}
		else if (!_movingRight && original > _patrolStart){
			Entity::setVelocity(Vec2(-_speed, 0));
		}
		else {
			Entity::setVelocity(Vec2(0, 0));
		}
		Entity::setPos(original + Entity::getVelocity().x);
		_sceneNode->setPositionX(original + Entity::getVelocity().x);
		if (Entity::getPos() >= _patrolEnd) {
			if (_turnFrame < ENEMY_TURNING_FRAMES) {
				_turnFrame++;
				_frame = 0;
			}
			else {
				_movingRight = false;
				_sceneNode->setScale(-0.05, 0.05);
				_turnFrame = 0;
			}
		}
		else if (Entity::getPos() <= _patrolStart) {
			if (_turnFrame < ENEMY_TURNING_FRAMES) {
				_turnFrame++;
				_frame = 0;
			}
			else {
				_movingRight = true;
				_sceneNode->setScale(0.05, 0.05);
				_turnFrame = 0;
			}
		}
		
	}
	else if (_isPossessed && _sceneNode->isVisible()) {
		manualMove(direction, ENEMY_SPEED);
		_sceneNode->setPositionX(original + getVelocity().x);
		if (direction == 1) {
			_movingRight = true;
			_sceneNode->setScale(0.05, 0.05);
		}
		else if (direction == -1) {
			_movingRight = false;
			_sceneNode->setScale(-0.05, 0.05);
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
	if ((_isActive || _isPossessed) && _frameCounter == 7) {
		_sceneNode->setFrame(_frame);
	}
}

void Enemy::setAsPossessed() {
	_isPossessed = true;
	_isActive = false;
	_sceneNode->setTexture(_altTexture);
}

void Enemy::setLevel(int level) {
	Entity::setLevel(level);
	_sceneNode->setPositionY(Entity::getLevel() * FLOOR_HEIGHT + FLOOR_OFFSET);
}


void Enemy::enemyDyingAnimation() {
	//use this field in brackets now you can reference any field defined in player or class
	std::function<bool()> frame0 = [&]() {
		std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(0);
		return false;
	};
	std::function<bool()> frame1 = [&]() {
		std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(1);
		return false;
	};
	std::function<bool()> frame2 = [&]() {
		std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(2);
		return false;
	};
	std::function<bool()> frame3 = [&]() {
		std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(3);
		return false;
	};
	std::function<bool()> frame4 = [&]() {
		std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(4);
		return false;
	};
	std::function<bool()> frame5 = [&]() {
		std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(5);
		return false;
	};
	std::function<bool()> frame6 = [&]() {
		std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(6);
		return false;
	};
	std::function<bool()> frame7 = [&]() {
		std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(7);
		return false;
	};
	int timeDiff = 75;
	CULog("h");
	cugl::Application::get()->schedule(frame0, 50 + timeDiff * 1);
	cugl::Application::get()->schedule(frame1, 50 + timeDiff * 2);
	/*cugl::Application::get()->schedule(frame2, 50 + timeDiff * 3);
	cugl::Application::get()->schedule(frame3, 50 + timeDiff * 4);
	cugl::Application::get()->schedule(frame4, 50 + timeDiff * 5);
	cugl::Application::get()->schedule(frame5, 50 + timeDiff * 6);
	cugl::Application::get()->schedule(frame6, 50 + timeDiff * 7);
	cugl::Application::get()->schedule(frame7, 50 + timeDiff * 8);*/
}




