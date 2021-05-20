#include "CollisionManager.h"

using namespace cugl;

/**
*  Handles collisions the player and wall, causing the player to stop further movement.
*
*  This method updates the velocities of the cat.
*
*  @param player    Player in candidate collision
*  @param entity    Entity in candidate collision
*/
#define DOOR_WIDTH 220
#define DOOR_OFFSET 20
#define LEFT_LEVEL_BOUND -300
#define RIGHT_LEVEL_BOUND 1440

void collisions::checkForDoorCollision(const std::shared_ptr<Enemy>& possessedEnemy,
	const vector<std::shared_ptr<Enemy>>& enemies, const std::shared_ptr<Player>& player,
	const std::vector<shared_ptr<Door>>& doors)
{
	std::shared_ptr<Entity> currentPlayer;
	if (possessedEnemy != nullptr) {
		currentPlayer = possessedEnemy;
	}
	else {
		currentPlayer = player;
	}
	Vec2 pos;
	for (shared_ptr<Door> door : doors) {
		if (!door->getIsOpen() &&
			door->getPos().x - currentPlayer->getPos() - DOOR_OFFSET <= DOOR_WIDTH / 2 &&
			door->getPos().x - currentPlayer->getPos() >= 0 &&
			door->getLevel() == currentPlayer->getLevel()) {
			currentPlayer->setPos(door->getPos().x - DOOR_OFFSET - DOOR_WIDTH / 2);
		}
		else if (!door->getIsOpen() &&
			currentPlayer->getPos() - door->getPos().x - DOOR_OFFSET <= DOOR_WIDTH / 2 &&
			currentPlayer->getPos() - door->getPos().x > 0 &&
			door->getLevel() == currentPlayer->getLevel()) {
			currentPlayer->setPos(door->getPos().x + DOOR_OFFSET + DOOR_WIDTH / 2);
		}
	}

	
	for (shared_ptr<Enemy> enemy : enemies) {
		for (shared_ptr<Door> door : doors) {
			/*if (door->getLevel() == 2&&!enemy->getPossessed()) {
				CULog("%d", door->getBlockedEnemy() == nullptr);
			}*/
			if (!door->getIsOpen() &&
				door->getPos().x - enemy->getPos() <= DOOR_WIDTH / 2 &&
				door->getPos().x - enemy->getPos() >= 0 &&
				door->getLevel() == enemy->getLevel() &&
				enemy->isActive()) {
				if (!(std::count(door->getBlockedEnemy().begin(), door->getBlockedEnemy().end(), enemy))) {
				}
				else{
					enemy->setOldPatrol(enemy->getPatrol());
					enemy->setPatrol(enemy->getPatrol().x, door->getPos().x - DOOR_WIDTH / 2);
					door->addBlockedEnemy(enemy);
				}
				
			}
			else if (!door->getIsOpen() &&
				enemy->getPos() - door->getPos().x <= DOOR_WIDTH / 2 &&
				enemy->getPos() - door->getPos().x >= 0 &&
				door->getLevel() == enemy->getLevel()&&
				enemy->isActive()) {
				if (!(std::count(door->getBlockedEnemy().begin(), door->getBlockedEnemy().end(), enemy))) {
				}
				else {
					enemy->setOldPatrol(enemy->getPatrol());
					enemy->setPatrol(door->getPos().x + DOOR_WIDTH / 2, enemy->getPatrol().y);
					door->addBlockedEnemy(enemy);
					
				}
			}

		}

	}

	
}

int collisions::checkForCagedAnimalCollision(const std::shared_ptr<Player>& player,
	const std::shared_ptr<Player>& cagedAnimal) {
	if (cagedAnimal->getPos() - player->getPos() <= DOOR_WIDTH / 2 &&
		cagedAnimal->getPos() - player->getPos() >= 0 &&
		cagedAnimal->getLevel() == player->getLevel() && !player->getPossess()) {
		return 1;

	}
	else if (cagedAnimal->getSceneNode()->isVisible() &&
		player->getPos() - cagedAnimal->getPos() <= DOOR_WIDTH / 2 &&
		player->getPos() - cagedAnimal->getPos() >= 0 &&
		cagedAnimal->getLevel() == player->getLevel() && !player->getPossess()) {
		return 2;
	}
	return 0;
}


/**
 * Nudge the player to ensure it does not do out of view.
 *
 * This code bounces the ship off walls.  You will replace it as part of
 * the lab.
 *
 * @param player      They player which may have collided
 * @param bounds    The rectangular bounds of the playing field
 */
void collisions::checkInBounds(const std::shared_ptr<Enemy>& possessedEnemy, const std::shared_ptr<Player>& player)
{
	std::shared_ptr<Entity> currentPlayer;
	if (possessedEnemy != nullptr) {
		currentPlayer = possessedEnemy;
	}
	else {
		currentPlayer = player;
	}
	Vec2 pos;

	if (currentPlayer->getPos() <= LEFT_LEVEL_BOUND) {
		currentPlayer->setPos(LEFT_LEVEL_BOUND);
	}
	if (currentPlayer->getPos() >= RIGHT_LEVEL_BOUND) {
		currentPlayer->setPos(RIGHT_LEVEL_BOUND);
	}
}
