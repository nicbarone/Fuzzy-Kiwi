#include "EnemyController.h"
using namespace cugl;
/** how far the player can possess an enemy from, also determines highlighting distance*/


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

void EnemyController::addEnemy(float x, int level, float ang, std::shared_ptr<Texture> enemy, std::shared_ptr<Texture> alt) {
	_enemies.push_back(Enemy::alloc(x,level,ang,enemy,alt));
}

std::shared_ptr<Enemy> EnemyController::closestEnemy() {
	return _closestEnemy;
}

void EnemyController::findClosest(float pos, int level) {
	float dist = POSSESS_RANGE;
	int index = -1;
	for (int i = 0; i < _enemies.size(); i++) {
		//eventually the y condition should be checking levels, not the actual y
		if (_enemies[i]->getLevel() == level && abs(_enemies[i]->getPos() - pos) < dist && _enemies[i]->isActive()) {
			dist = abs(_enemies[i]->getPos() - pos);
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

bool EnemyController::detectedPlayer(float x, int level) {
	for (auto it = begin(_enemies); it != end(_enemies); ++it) {
		auto enemy = it->get();
		if (level == enemy->getLevel()) {
			if ((enemy->facingRight() && enemy->getPos() + enemy->getVision() > x && enemy->getPos() < x)
				|| (!enemy->facingRight() && enemy->getPos() - enemy->getVision() < x) && enemy->getPos() > x) {
				return true;
			}
		}
	}
	
	return false;
}