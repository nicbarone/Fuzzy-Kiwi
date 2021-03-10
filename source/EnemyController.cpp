#include "EnemyController.h"
using namespace cugl;
/** how far the player can possess an enemy from, also determines highlighting distance*/
#define POSSESS_RANGE 150

EnemyController::EnemyController() 
{
	_enemies = {};
	_closestEnemy = nullptr;
	_possessedEnemy = nullptr;
}

void EnemyController::dispose() {
	_enemies = {};
	_closestEnemy = nullptr;
	_possessedEnemy = nullptr;
}

void EnemyController::addEnemy(float x, float y, float ang, std::shared_ptr<Texture> enemy, std::shared_ptr<Texture> alt) {
	_enemies.push_back(Enemy::alloc(x,y,ang,enemy,alt));
}

std::shared_ptr<Enemy> EnemyController::closestEnemy() {
	return _closestEnemy;
}

void EnemyController::findClosest(Vec2 pos) {
	float dist = POSSESS_RANGE;
	int index = -1;
	for (int i = 0; i < _enemies.size(); i++) {
		//eventually the y condition should be checking levels, not the actual y
		if (_enemies[i]->getPos().y == pos.y && abs(_enemies[i]->getPos().x - pos.x) < dist && _enemies[i]->isActive()) {
			dist = abs(_enemies[i]->getPos().x - pos.x);
			index = i;
		}
	}
	// enemies out of range will be reset
	if (index == -1 && _closestEnemy != nullptr) {
		_closestEnemy->getSceneNode()->setAngle(0);
	}
	_closestEnemy = index == -1 ? nullptr : _enemies[index];
}

void EnemyController::moveEnemies(float direction) {
	for (auto it = begin(_enemies); it != end(_enemies); ++it) {
		it->get()->move(direction);
	}
}