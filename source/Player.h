#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__
#include <cugl/cugl.h>
#include "Entity.h"
using namespace cugl;
class Player : Entity {

private:
	/** number of possessions remaining*/
	int _n_possessions;
	/** whether the player is possessing an enemy*/
	bool _is_possessing;
	/** whether the player is hidden, i.e. in staircase or in den*/
	bool _is_hidden;

	std::shared_ptr<scene2::AnimationNode> _scene_node;
	std::shared_ptr<Texture> _texture;


public:

	Player();

	~Player() { dispose(); }

	void dispose();
	
	bool init(float x, float y, float ang, std::shared_ptr<Texture> cat);

	static std::shared_ptr<Player> alloc(float x, float y, float ang, std::shared_ptr<Texture> cat) {
		std::shared_ptr<Player> result = std::make_shared<Player>();
		return (result->init(x, y, ang, cat) ? result : nullptr);
	}

	/** true if the player has possessions remaining and is in cat form*/
	bool can_possess() {
		return _n_possessions > 0 && !_is_possessing;
	}

	/** sets possessions remaining*/
	void set_n_possess(int count) {
		_n_possessions = count;
	}

	/** sets the possession state of the cat*/
	void set_possess(bool value) {
		_is_possessing = value;
	}
	
	/** sets the hiding state of the cat*/
	void set_hidden(bool value) {
		_is_hidden;
	}


	/** returns the AnimationNode associated with the player*/
	std::shared_ptr<scene2::AnimationNode> get_scene_node() {
		return _scene_node;
	}

};

#endif