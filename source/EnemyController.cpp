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

void EnemyController::addEnemy(float x, int level, float ang, vector<int> keys, float patrolStart, float patrolEnd, int num_frames,
	std::shared_ptr<Texture> enemy, std::shared_ptr<Texture> alt, std::shared_ptr<Texture> glow, std::shared_ptr<Texture> table) {
	_enemies.push_back(Enemy::alloc(x, level, ang, keys, patrolStart, patrolEnd, num_frames, enemy, alt, glow, table));
}

std::shared_ptr<Enemy> EnemyController::closestEnemy() {
	return _closestEnemy;
}

void EnemyController::findClosest(float pos, int level, vector<Vec2> vision_blockers) {
	float dist = POSSESS_RANGE;
	int index = -1;
	bool blocked = false;
	for (int i = 0; i < _enemies.size(); i++) {
		if (_enemies[i]->getLevel() == level && abs(_enemies[i]->getPos() - pos) < dist && _enemies[i]->isActive()) {
			for (auto it = begin(vision_blockers); it != end(vision_blockers); it++) {
				auto pair = it;
				if (pair->y == level && (pair->x > pos && pair->x < _enemies[i]->getPos())
					|| (pair->x < pos && pair->x > _enemies[i]->getPos())) {
					blocked = true;
				}
			}
			if (!blocked) {
				dist = abs(_enemies[i]->getPos() - pos);
				index = i;
			}
			blocked = false;
		}
	}
	// enemies out of range will be reset
	if (index == -1 && _closestEnemy != nullptr && !_closestEnemy->isPossessed() && _closestEnemy->isActive()) {
		_closestEnemy->setGlow(false);
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

bool EnemyController::colorDetectingPlayer(float x, int level, vector<Vec2> vision_blockers) {
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
					enemy->getSceneNode()->setColor(Color4::YELLOW);
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
					enemy->getSceneNode()->setColor(Color4::RED);
					return true;
				}
			}

		}
	}
	return false;
}

void EnemyController::removeEnemy(std::shared_ptr<Enemy> enemy) {
	_enemies.erase(std::remove(_enemies.begin(), _enemies.end(), enemy), _enemies.end());
}
