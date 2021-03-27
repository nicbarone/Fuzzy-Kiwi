#include "Player.h"
using namespace cugl;

/** how fast the player moves*/

Player::Player() :

	_nPossessions(1),
	_isPossessing(false),
	_isHidden(false),
	_frame(0),
	_frameCounter(12)
{

	_sceneNode = nullptr;
	_texture = nullptr;
}


void Player::dispose() {
	_nPossessions = 0;
	_isPossessing = false;
	_isHidden = false;
	_texture = nullptr;
	_sceneNode = nullptr;
	_frame = 0;
	_frameCounter = 7;
	Entity::dispose();
}


bool Player::init(float x, int level, float ang, std::shared_ptr<Texture> cat)

{
	Entity::setPos(x);
	Entity::setLevel(level);
	Entity::setAngle(0);
	Entity::setLevel(level);
	_texture = cat;
	_sceneNode = scene2::AnimationNode::alloc(_texture, 1, 8);
	_sceneNode->setScale(0.15, 0.15);
	_sceneNode->setPosition(Vec2(x, level*FLOOR_HEIGHT+FLOOR_OFFSET));
	return true;
}

void Player::move(float direction) {
	float original = getPos();
	manualMove(direction, PLAYER_SPEED);
	_sceneNode->setPositionX(original + getVelocity().x);
	if (direction == 1) {
		_sceneNode->setScale(0.15, 0.15);
	}
	else if (direction == -1) {
		_sceneNode->setScale(-0.15, 0.15);
	}
	if (direction != 0)
	{
		if (_frameCounter > 0) {
			_frameCounter--;
		}
		else {
			_frameCounter = 7;
			_frame++;
			_frame = _frame % 8;
		}
	}
	_sceneNode->setFrame(_frame);
}

void Player::setLevel(int level) {
	Entity::setLevel(level);
	_sceneNode->setPositionY(Entity::getLevel() * FLOOR_HEIGHT + FLOOR_OFFSET);
}