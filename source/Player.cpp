#include "Player.h"
using namespace cugl;
Player::Player() :

	_n_possessions(0),
	_is_possessing(true),
	_is_hidden(false)
{

	_scene_node = nullptr;
	
}


void Player::dispose() {
	_n_possessions = 0;
	_is_possessing = false;
	_is_hidden = false;
	_texture = nullptr;
	_scene_node = nullptr;
	Entity::dispose();
}

bool Player::init(float x, float y, float ang, std::shared_ptr<Texture> cat)
{
	Entity::set_pos(Vec2(x, y));
	Entity::set_angle(0);
	_texture = cat;
	_scene_node = scene2::AnimationNode::alloc(_texture, 1, 1);
	return true;
}


