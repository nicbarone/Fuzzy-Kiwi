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
	float _patrolStart;
	/** the x position of the end enemy's patrol path*/
	float _patrolEnd;
	/** ray cast distance of the enemy*/
	float _visionRange;
	/** movement speed of the enemy*/
	float _speed;
	/** keeps track of which direction the enemy is moving*/
	bool _movingRight;
	/** whether the enemy is blocked by a closed door*/
	bool _isStuck;
	/** whether the enemy has been possessed*/
	bool _isPossessed;
	/** whether the enemy is patrolling*/
	bool _isActive;



	std::shared_ptr<scene2::AnimationNode> _sceneNode;
	std::shared_ptr<Texture> _texture;
	std::shared_ptr<Texture> _altTexture;


public:

	Enemy();

	~Enemy() { dispose(); }

	void dispose();

	bool init(float x, float y, float ang, std::shared_ptr<Texture> enemy, std::shared_ptr<Texture> alt);

	static std::shared_ptr<Enemy> alloc(float x, float y, float ang, std::shared_ptr<Texture> enemy, std::shared_ptr<Texture> alt) {
		std::shared_ptr<Enemy> result = std::make_shared<Enemy>();
		return (result->init(x, y, ang, enemy, alt) ? result : nullptr);
	}


	/** returns the AnimationNode associated with the player*/
	std::shared_ptr<scene2::AnimationNode> getSceneNode() {
		return _sceneNode;
	}

	/** changes the x position of the enemy based on its patrol path and current position*/
	void move(float direction);

	/** changes possessed state of the enemy*/
	void setPossessed();

	/** sets the _patrolStart and _patrolEnd of the enemy*/
	void setPatrol(float x1, float x2) {
		_patrolStart = x1;
		_patrolEnd = x2;
	}

	bool isActive() {return _isActive; }

	/** returns a Vec2 containing _patrolStart and _patrolEnd of the enemy*/
	// Vec2 without a y might be bad practice? can change if needed
	Vec2 getPatrol() {
		return Vec2(_patrolStart, _patrolEnd);
	}

};

#endif