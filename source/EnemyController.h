#pragma once
#ifndef __ENEMYCONTROLLER_H__
#define __ENEMYCONTROLLER_H__
#include <cugl/cugl.h>
#include <Enemy.h>
using namespace cugl;
class EnemyController {
private:
	/** reference to vector of enemies managed by this controller*/
	vector<std::shared_ptr<Enemy>> _enemies;
	/** reference to the closest enemy to the player*/
	std::shared_ptr<Enemy> _closestEnemy;
	/** reference to the currently possessed enemy*/
	std::shared_ptr<Enemy> _possessedEnemy;

public:

	EnemyController();

	~EnemyController() { dispose(); }

	void dispose();

	/** creates and adds an enemy to this controller*/
	void addEnemy(float x, float y, float ang, std::shared_ptr<Texture> enemy, std::shared_ptr<Texture> alt);

	/** returns the vector of enemies managed by this controller*/
	vector<std::shared_ptr<Enemy>> getEnemies() {
		return _enemies;
	}

	/** updates the controller's reference _closestEnemy*/
	void findClosest(Vec2 pos);

	/** returns the closest enemy*/
	std::shared_ptr<Enemy> closestEnemy();

	/** moves active enemies*/
	void moveEnemies(float direction);

	/** setter for _possessedEnemy*/
	void updatePossessed(std::shared_ptr<Enemy> enemy) {
		_possessedEnemy = enemy;
	}
	/** getter for _possessedEnemy*/
	std::shared_ptr<Enemy> getPossessed() {
		return _possessedEnemy;
	}

};
#endif