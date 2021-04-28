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
	int _turnFrame;
	/*list of keys the enemies has. Keys can open specific doors.*/
	std::vector<int> _keys;


	std::shared_ptr<scene2::AnimationNode> _sceneNode;
	std::shared_ptr<Texture> _texture;
	std::shared_ptr<Texture> _altTexture;
	std::shared_ptr<Texture> _glowTexture;
	std::shared_ptr<Texture> _tableTexture;
	std::shared_ptr<scene2::WireNode> _patrolNode;
	std::shared_ptr<scene2::PolygonNode> _startTableNode;
	std::shared_ptr<scene2::PolygonNode> _endTableNode;
	std::shared_ptr<scene2::WireNode> _visionNode;


public:

	Enemy();

	~Enemy() { dispose(); }

	void dispose();

	bool init(float x, int level, float ang, std::vector<int> keys, float patrolStart, float patrolEnd, std::shared_ptr<Texture> enemy, std::shared_ptr<Texture> alt, std::shared_ptr<Texture> glow);
	
	bool init(float x, int level, float ang, std::vector<int> keys, float patrolStart, float patrolEnd, int num_frames,
		std::shared_ptr<Texture> enemy, std::shared_ptr<Texture> alt, std::shared_ptr<Texture> glow, std::shared_ptr<Texture> table);

	static std::shared_ptr<Enemy> alloc(float x, int level, float ang, std::vector<int> keys, float patrolStart, float patrolEnd, int num_frames, std::shared_ptr<Texture> enemy, std::shared_ptr<Texture> alt, std::shared_ptr<Texture> glow) {
		std::shared_ptr<Enemy> result = std::make_shared<Enemy>();
		return (result->init(x, level, ang, keys, patrolStart, patrolEnd, enemy, alt, glow) ? result : nullptr);
	}

	static std::shared_ptr<Enemy> alloc(float x, int level, float ang, std::vector<int> keys, float patrolStart, float patrolEnd, int num_frames,
		std::shared_ptr<Texture> enemy, std::shared_ptr<Texture> alt, std::shared_ptr<Texture> glow, std::shared_ptr<Texture> table) {
		std::shared_ptr<Enemy> result = std::make_shared<Enemy>();
		return (result->init(x, level, ang, keys, patrolStart, patrolEnd, num_frames, enemy, alt, glow, table) ? result : nullptr);
	}

	/** returns the AnimationNode associated with the player*/
	std::shared_ptr<scene2::AnimationNode> getSceneNode() {
		return _sceneNode;
	}

	/** OBSOLETE: returns the WireNode associated with the patrol path*/
	std::shared_ptr<scene2::WireNode> getPatrolNode() {
		return _patrolNode;
	}

	/** returns the WireNode associated with the vision path*/
	std::shared_ptr<scene2::WireNode> getVisionNode() {
		return _visionNode;
	}

	/** returns the node associated with the start table node*/
	std::shared_ptr<scene2::PolygonNode> getStartTableNode() {
		return _startTableNode;
	}

	/** returns the node associated with the end table node*/
	std::shared_ptr<scene2::PolygonNode> getEndTableNode() {
		return _endTableNode;
	}

	/** changes the x position of the enemy based on its patrol path and current position*/
	void move(float direction);

	/** changes the possessed state of the enemy*/
	void setPossessed(bool isPossessed) {
		_isPossessed = isPossessed;
	}
	/** changes possessed state of the enemy*/
	bool getPossessed() {
		return _isPossessed;
	}
	/** changes the possessed state of the enemy*/
	void setAsPossessed();

	/** returns the possessed state of the enemy*/
	bool isPossessed() { return _isPossessed; }

	/** sets the _patrolStart and _patrolEnd of the enemy and updates the _patrolNode*/
	void setPatrol(float x1, float x2) {
		_patrolStart = x1;
		_patrolEnd = x2;
		//_patrolNode->setPolygon(Rect(0, 0, x1 - x2, 2));
		//_patrolNode->setPositionX((x1 + x2) / 2);
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

	/** setter mainly used to highlight enemies, also updates the scenenode's texture*/
	void setGlow(bool flag) {
		_sceneNode->setTexture(flag ? _glowTexture : _texture);
	}

	void setKeys(std::vector<int> keys) {
		_keys = keys;
	}

	std::vector<int> getKeys() {
		return _keys;
	}

	void enemyOpeningDoorAnimation();

	void SetSceneNode(std::shared_ptr<scene2::AnimationNode> newNode) {
		_sceneNode = newNode;
	}


	/** changes the moving state of the enemy*/
	void setMovingRight(bool movingRight) {
		_movingRight = movingRight;
	}
	/** gets the moving right state of the enemy*/
	bool getMovingRight() {
		return _movingRight;
	}
};



#endif