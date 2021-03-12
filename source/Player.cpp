#include "Player.h"
using namespace cugl;

/** how fast the player moves*/
#define SPEED 10
Player::Player() :

	_nPossessions(1),
	_isPossessing(false),
	_isHidden(false)
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
	Entity::dispose();
}

bool Player::init(float x, float y, float ang, std::shared_ptr<Texture> cat)
{
	Entity::setPos(Vec2(x, y));
	Entity::setAngle(0);
	_texture = cat;
	_sceneNode = scene2::AnimationNode::alloc(_texture, 1, 1);
	_sceneNode->setPosition(Vec2(x, y));
	return true;
}

void Player::move(float direction) {
	Vec2 original = getPos();
	manualMove(direction, SPEED);
	_sceneNode->setPosition(original + getVelocity());
}