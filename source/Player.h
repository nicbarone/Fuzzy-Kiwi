#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__
#include <cugl/cugl.h>
#include "Entity.h"
using namespace cugl;
class Player : public Entity {

private:
	/** number of possessions remaining*/
	int _nPossessions;
	/** whether the player is possessing an enemy*/
	bool _isPossessing;
	/** whether the player is hidden, i.e. in staircase or in den*/
	bool _isHidden;

	std::shared_ptr<scene2::AnimationNode> _sceneNode;
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
	bool canPossess() {
		return _nPossessions > 0 && !_isPossessing;
	}

	/** sets possessions remaining*/
	void set_nPossess(int count) {
		_nPossessions = count;
	}

	/** sets the possession state of the cat*/
	void setPossess(bool value) {
		_isPossessing = value;
	}
	
	/** sets the hiding state of the cat*/
	void setHidden(bool value) {
		_isHidden;
	}

	/** changes the x position of the player based on the direction*/
	void move(float direction);

	/** returns the AnimationNode associated with the player*/
	std::shared_ptr<scene2::AnimationNode> getSceneNode() {
		return _sceneNode;
	}

};

#endif