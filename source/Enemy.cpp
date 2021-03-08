#include "Enemy.h"
using namespace cugl;
Enemy::Enemy() :

	_patrol_start(0),
	_patrol_end(0),
	_vision_range(0),
	_is_stuck(false),
	_is_possessed(false),
	_is_active(false)
{

	_scene_node = nullptr;
	_texture = nullptr;
}


void Enemy::dispose() {
	_patrol_start = 0;
	_patrol_end = 0;
	_vision_range = 0;
	_is_stuck = false;
	_is_possessed = false;
	_is_active = false;
	_texture = nullptr;
	_scene_node = nullptr;
	Entity::dispose();
}

bool Enemy::init(float x, float y, float ang, std::shared_ptr<Texture> enemy)
{
	Entity::set_pos(Vec2(x, y));
	Entity::set_angle(0);
	_texture = enemy;
	_scene_node = scene2::AnimationNode::alloc(_texture, 1, 1);
	return true;
}


