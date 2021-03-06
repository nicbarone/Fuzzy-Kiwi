#pragma once
#pragma once
#ifndef __ENEMY_H__
#define __ENEMY_H__
#include "cugl/cugl.h"
#include "Entity.h"
#include "Constants.h"
using namespace cugl;

extern const float DEFAULT_VISION;
extern const float ENEMY_SPEED;


class Enemy : public Entity {

private:

	/** the x position of the start enemy's patrol path*/
	float _patrolStart;
	/** the x position of the end enemy's patrol path*/
	float _patrolEnd;
	/**the old patrol coordinates that have been adjusted due to
	a door*/
	Vec2 _oldPatrol;
	/** ray cast distance of the enemy*/
	float _visionRange;
	/** movement speed of the enemy*/
	float _speed;
	/** keeps track of which direction the enemy is moving/facing*/
	bool _movingRight;
	/** whether the enemy is blocked by a closed door*/
	bool _isStuck;
	/** whether the enemy has been possessed*/
	bool _isPossessed;
	/** whether the enemy is patrolling*/
	bool _isActive;
	int _frame;
	int _frameCounter;


	std::shared_ptr<scene2::AnimationNode> _sceneNode;
	std::shared_ptr<Texture> _texture;
	std::shared_ptr<Texture> _altTexture;


public:

	Enemy();

	~Enemy() { dispose(); }

	void dispose();

	bool init(float x, int level, float ang, float patrolStart, float patrolEnd, std::shared_ptr<Texture> enemy, std::shared_ptr<Texture> alt);

	static std::shared_ptr<Enemy> alloc(float x, int level, float ang, float patrolStart, float patrolEnd, std::shared_ptr<Texture> enemy, std::shared_ptr<Texture> alt) {
		std::shared_ptr<Enemy> result = std::make_shared<Enemy>();
		return (result->init(x, level, ang, patrolStart, patrolEnd, enemy, alt) ? result : nullptr);
	}


	/** returns the AnimationNode associated with the player*/
	std::shared_ptr<scene2::AnimationNode> getSceneNode() {
		return _sceneNode;
	}

	/** changes the x position of the enemy based on its patrol path and current position*/
	void move(float direction);

	/** changes possessed state of the enemy*/
	bool getPossessed() {
		return _isPossessed;
	}
	/** changes possessed state of the enemy*/
	void setPossessed();

	/** changes possessed state of the enemy*/
	bool isPossessed() { return _isPossessed; }

	/** sets the _patrolStart and _patrolEnd of the enemy*/
	void setPatrol(float x1, float x2) {
		_patrolStart = x1;
		_patrolEnd = x2;
	}
	/** sets the _patrolStart and _patrolEnd of the old patrol of the enemy that
	is changed due to hitting a door*/
	void setOldPatrol(Vec2 patrol) {
		_oldPatrol = patrol;
	}

	/** returns a Vec2 containing _patrolStart and _patrolEnd of the old patrol of the enemy*/
	Vec2 getOldPatrol() {
		return _oldPatrol;
	}

	bool isActive() { return _isActive; }

	/** returns a Vec2 containing _patrolStart and _patrolEnd of the enemy*/
	// Vec2 without a y might be bad practice? can change if needed
	Vec2 getPatrol() {
		return Vec2(_patrolStart, _patrolEnd);
	}

	/** getter for enemy facing direction*/
	bool facingRight() {
		return _movingRight;
	}

	/** getter for enemy vision range*/
	float getVision() {
		return _visionRange;
	}

	/** setter for level, overloaded for this class to also change scene node position, deprecates changeFloor()*/
	void setLevel(int level);

};

#endif