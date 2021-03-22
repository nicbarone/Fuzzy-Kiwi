#include "Player.h"
using namespace cugl;

/** how fast the player moves*/

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


bool Player::init(float x, int level, float ang, std::shared_ptr<Texture> cat)

{
	Entity::setPos(x);
	Entity::setLevel(level);
	Entity::setAngle(0);
	Entity::setLevel(level);
	_texture = cat;
	_sceneNode = scene2::AnimationNode::alloc(_texture, 1, 1);
	_sceneNode->setPosition(Vec2(x, level*FLOOR_HEIGHT+FLOOR_OFFSET));
	return true;
}

void Player::move(float direction) {
	float original = getPos();
	manualMove(direction, PLAYER_SPEED);
	_sceneNode->setPositionX(original + getVelocity().x);
}

void Player::setLevel(int level) {
	Entity::setLevel(level);
	_sceneNode->setPositionY(Entity::getLevel() * FLOOR_HEIGHT + FLOOR_OFFSET);
}