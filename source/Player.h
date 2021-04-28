#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "cugl/cugl.h"
#include "Entity.h"
#include "Enemy.h"
using namespace cugl;

extern const float PLAYER_SPEED;

class Player : public Entity {

private:
	/** number of possessions remaining*/
	int _nPossessions;
	/** whether the player is possessing an enemy*/
	bool _isPossessing;
	/** whether the player is hidden, i.e. in staircase or in den*/
	bool _isHidden;

	int _currentDoor;
	int _currentDen;
	bool _movingRight;

	std::shared_ptr<scene2::AnimationNode> _sceneNode;
	std::shared_ptr<Texture> _texture;
	std::shared_ptr<Enemy> _possessingEnemy;

	int _frame;
	int _frameCounter;

public:

	Player();

	~Player() { dispose(); }

	void dispose();
	

	bool init(float x, int level, float ang, int num_frames, const std::shared_ptr<Texture> cat);

	static std::shared_ptr<Player> alloc(float x, int level, float ang, int num_frames, const std::shared_ptr<Texture> cat) {
		std::shared_ptr<Player> result = std::make_shared<Player>();
		return (result->init(x, level, ang, num_frames, cat) ? result : nullptr);

	}

	/** true if the player has possessions remaining and is in cat form*/
	bool canPossess() {
		return _nPossessions > 0 && !_isPossessing;
	}

	/** sets possessions remaining*/
	void set_nPossess(int count) {
		_nPossessions = count;
	}
	/**gets possessions remaining*/
	int get_nPossess() {
		return _nPossessions;
	}

	/** sets possess enemy */
	void set_possessEnemy(std::shared_ptr<Enemy> enemy) {
		_nPossessions--;
		_possessingEnemy = enemy;
	}

	/** gets possess enemy */
	std::shared_ptr<Enemy> get_possessEnemy() {
		return _possessingEnemy;
	}

	/** sets the possession state of the cat*/
	void setPossess(bool value) {
		_isPossessing = value;
	}

	void SetSceneNode(std::shared_ptr<scene2::AnimationNode> newNode) {
		_sceneNode = newNode;
	}

	/** gets the possession state of the cat*/
	bool getPossess() {
		return _isPossessing;
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

	/** setter for level, overloaded for this class to also change scene node position, deprecates changeFloor()*/
	void setLevel(int level);

	int getCurrentDoor() {
		return _currentDoor;
	}
	void setCurrentDoor(int currDoor) {
		_currentDoor = currDoor;
	}
	int getCurrentDen() {
		return _currentDen;
	}
	void setCurrentDen(int currDen) {
		_currentDen = currDen;
	}
	void PossessAnimation(int possessing);

	void EnemyDying();

	/** sets player's movingRight state*/
	void setMovingRight(bool movingRight) {
		_movingRight = movingRight;
	}
	/**gets player's movingRight state*/
	int getMovingRight() {
		return _movingRight;
	}

};

#endif