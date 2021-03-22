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
#define DOOR_WIDTH 130

void collisions::checkForDoorCollision(const std::shared_ptr<Enemy>& possessedEnemy,
	const vector<std::shared_ptr<Enemy>>& enemies, const std::shared_ptr<Player>& player,
	const std::vector<shared_ptr<Door>>& doors)
{

	
	for (shared_ptr<Enemy> enemy : enemies) {
		for (shared_ptr<Door> door : doors) {
			/*if (door->getLevel() == 2&&!enemy->getPossessed()) {
				CULog("%d", door->getBlockedEnemy() == nullptr);
			}*/
			if (door->getSceneNode()->isVisible() &&
				door->getPos().x - enemy->getPos() <= DOOR_WIDTH / 2 &&
				door->getPos().x - enemy->getPos() >= 0 &&
				door->getLevel() == enemy->getLevel()) {
				if (door->getBlockedEnemy() == nullptr) {
					enemy->setOldPatrol(enemy->getPatrol());
					enemy->setPatrol(enemy->getPatrol().x, door->getPos().x - DOOR_WIDTH / 2);
					door->setBlockedEnemy(enemy);
				}
				
			}
			else if (door->getSceneNode()->isVisible() &&
				enemy->getPos() - door->getPos().x <= DOOR_WIDTH / 2 &&
				enemy->getPos() - door->getPos().x >= 0 &&
				door->getLevel() == enemy->getLevel()) {
				if (door->getBlockedEnemy() == nullptr) {
					enemy->setOldPatrol(enemy->getPatrol());
					enemy->setPatrol(door->getPos().x + DOOR_WIDTH / 2, enemy->getPatrol().y);
					door->setBlockedEnemy(enemy);
					
				}
			}

		}

	}

	std::shared_ptr<Entity> currentPlayer;
	if (possessedEnemy != nullptr) {
		currentPlayer = possessedEnemy;
	}
	else {
		currentPlayer = player;
	}
			for (shared_ptr<Door> door : doors) {
				if (door->getSceneNode()->isVisible() &&
					door->getPos().x - currentPlayer->getPos() <= DOOR_WIDTH/2 &&
					door->getPos().x - currentPlayer->getPos() >= 0 &&
					door->getLevel() == currentPlayer->getLevel()) {
					currentPlayer->setPos(door->getPos().x - DOOR_WIDTH / 2);
				}
				else if (door->getSceneNode()->isVisible() &&
					currentPlayer->getPos() - door->getPos().x <= DOOR_WIDTH/2 &&
					currentPlayer->getPos() - door->getPos().x > 0 &&
					door->getLevel() == currentPlayer->getLevel()) {
					currentPlayer->setPos(door->getPos().x + DOOR_WIDTH / 2);
				}
			}


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
void collisions::checkInBounds(const std::shared_ptr<Player>& player, const cugl::Rect bounds)
{

    //Vec2 vel = player->getVelocity();
    float pos = player->getPos();

    //Ensure player doesn't go out of view. Stop by walls
    if (pos <= bounds.origin.x) {
        //vel.x = 0;
        pos = bounds.origin.x;
        //player->setVelocity(vel);
        player->setPos(pos);
    }
    else if (pos >= bounds.size.width + bounds.origin.x) {
        //vel.x = 0;
        pos = bounds.size.width + bounds.origin.x - 1.0f;
        //player->setVelocity(vel);
        player->setPos(pos);
    }

    //if (pos.y <= bounds.origin.y) {
    //    //vel.y = 0;
    //    pos.y = bounds.origin.y;
    //    //player->setVelocity(vel);
    //    player->setPos(pos);
    //}
    //else if (pos.y >= bounds.size.height + bounds.origin.y) {
    //    //vel.y = 0;
    //    pos.y = bounds.size.height + bounds.origin.y - 1.0f;
    //    //player->setVelocity(vel);
    //    player->setPos(pos);
    //}

}
