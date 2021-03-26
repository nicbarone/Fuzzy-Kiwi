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

void EnemyController::addEnemy(float x, int level, float patrolStart, float patrolEnd, float ang, std::shared_ptr<Texture> enemy, std::shared_ptr<Texture> alt) {
	_enemies.push_back(Enemy::alloc(x, level, ang, patrolStart, patrolEnd, enemy, alt));
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

bool EnemyController::detectedPlayer(float x, int level, vector<Vec2> vision_blockers) {
	for (auto it = begin(_enemies); it != end(_enemies); ++it) {
		auto enemy = it->get();

		float min = enemy->facingRight() ? enemy->getPos() : enemy->getPos() - enemy->getVision();
		float max = enemy->facingRight() ? enemy->getPos() + enemy->getVision() : enemy->getPos();
		//check if vision obstructed by door
		for (auto it2 = begin(vision_blockers); it2 != end(vision_blockers); it2++) {

			auto pair = it2;
			if (pair->y == enemy->getLevel()) {
				max = enemy->facingRight() && pair->x > enemy->getPos() ? std::min(max, pair->x) : max;
				min = !enemy->facingRight() && pair->x < enemy->getPos() ? std::max(min, pair->x) : min;
			}
		}
			if (_possessedEnemy == nullptr) { //if player is not possessing
			//if on the same floor, is active and not possessed
				if (level == enemy->getLevel() && enemy->isActive() && !enemy->isPossessed()) {
					if (min < x && x < max) {
						//if ((enemy->facingRight() && enemy->getPos() + enemy->getVision() > x && enemy->getPos() < x)
						//	|| (!enemy->facingRight() && enemy->getPos() - enemy->getVision() < x) && enemy->getPos() > x) {
						CULog("detected");
						return true;
					}
				}
			}
			else { //if player possessing, compare with possessed and also checks possessed facing
				//if on the same floor, is active and not possessed
				if (_possessedEnemy->getLevel() == enemy->getLevel() && enemy->isActive() && !enemy->isPossessed()) {
					if ((enemy->facingRight() && min < x && x < max && _possessedEnemy->facingRight())
						|| (!enemy->facingRight() && min < x && x < max && !_possessedEnemy->facingRight())) {
						//if ((enemy->facingRight() && enemy->getPos() + enemy->getVision() > _possessedEnemy->getPos()
						//	&& enemy->getPos() < _possessedEnemy->getPos() && _possessedEnemy->facingRight())
						//	|| (!enemy->facingRight() && enemy->getPos() - enemy->getVision() < _possessedEnemy->getPos())
						//	&& enemy->getPos() > _possessedEnemy->getPos() && !_possessedEnemy->facingRight()) {
						CULog("detected");
						return true;
					}
				}
			
		}
	}
	return false;
}

