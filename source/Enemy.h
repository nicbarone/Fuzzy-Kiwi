#pragma once
#pragma once
#ifndef __ENEMY_H__
#define __ENEMY_H__
#include <cugl/cugl.h>
#include "Entity.h"
using namespace cugl;
class Enemy : public Entity {

private:

	/** the x position of the start enemy's patrol path*/
	float _patrol_start;
	/** the x position of the end enemy's patrol path*/
	float _patrol_end;
	/** ray cast distance of the enemy*/
	float _vision_range;
	/** whether the enemy is blocked by a closed door*/
	bool _is_stuck;
	/** whether the enemy has been possessed*/
	bool _is_possessed;
	/** whether the enemy is patrolling*/
	bool _is_active;


	std::shared_ptr<scene2::AnimationNode> _scene_node;
	std::shared_ptr<Texture> _texture;


public:

	Enemy();

	~Enemy() { dispose(); }

	void dispose();

	bool init(float x, float y, float ang, std::shared_ptr<Texture> cat);

	static std::shared_ptr<Enemy> alloc(float x, float y, float ang, std::shared_ptr<Texture> enemy) {
		std::shared_ptr<Enemy> result = std::make_shared<Enemy>();
		return (result->init(x, y, ang, enemy) ? result : nullptr);
	}


	/** returns the AnimationNode associated with the player*/
	std::shared_ptr<scene2::AnimationNode> get_scene_node() {
		return _scene_node;
	}

};

#endif