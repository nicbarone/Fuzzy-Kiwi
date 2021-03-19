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
void collisions::checkForDoorCollision(const std::shared_ptr<Enemy>& possessedEnemy,
	const std::shared_ptr<Player>& player, const  std::vector<shared_ptr<Door>>& doors)
{
	

	if (possessedEnemy != nullptr) {
		std::vector<shared_ptr<Entity>> possiblePlayer;
		possiblePlayer = { player, possessedEnemy };

		Vec2 pos = possessedEnemy->getPos();
		for (shared_ptr<Entity> currentPlayer : possiblePlayer) {
			for (shared_ptr<Door> door : doors) {
				if (door->getSceneNode()->isVisible() &&
					door->getPos().x - currentPlayer->getPos().x <= 70 &&
					door->getPos().x - currentPlayer->getPos().x >= 0) {
					pos = Vec2(door->getPos().x - 70, currentPlayer->getPos().y);
					currentPlayer->setPos(pos);
					CULog("1");
				}
				else if (door->getSceneNode()->isVisible() &&
					currentPlayer->getPos().x - door->getPos().x <= 70 &&
					currentPlayer->getPos().x - door->getPos().x > 0) {
					pos = Vec2(door->getPos().x + 70, currentPlayer->getPos().y);
					currentPlayer->setPos(pos);
					CULog("2");
				}

			}
		}
	else {
		Vec2 pos = player->getPos();
		for (shared_ptr<Door> door : doors) {
			if (door->getSceneNode()->isVisible() &&
				door->getPos().x - player->getPos().x <= 70 &&
				door->getPos().x - player->getPos().x >= 0) {
				pos = Vec2(door->getPos().x - 70, player->getPos().y);
				player->setPos(pos);
				CULog("1");
			}
			else if (door->getSceneNode()->isVisible() &&
				player->getPos().x - door->getPos().x <= 70 &&
				player->getPos().x - door->getPos().x > 0) {
				pos = Vec2(door->getPos().x + 70, player->getPos().y);
				player->setPos(pos);
				CULog("2");
			}
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
	Vec2 pos = player->getPos();

	//Ensure player doesn't go out of view. Stop by walls
	if (pos.x <= bounds.origin.x) {
		//vel.x = 0;
		pos.x = bounds.origin.x;
		//player->setVelocity(vel);
		player->setPos(pos);
	}
	else if (pos.x >= bounds.size.width + bounds.origin.x) {
		//vel.x = 0;
		pos.x = bounds.size.width + bounds.origin.x - 1.0f;
		//player->setVelocity(vel);
		player->setPos(pos);
	}

	if (pos.y <= bounds.origin.y) {
		//vel.y = 0;
		pos.y = bounds.origin.y;
		//player->setVelocity(vel);
		player->setPos(pos);
	}
	else if (pos.y >= bounds.size.height + bounds.origin.y) {
		//vel.y = 0;
		pos.y = bounds.size.height + bounds.origin.y - 1.0f;
		//player->setVelocity(vel);
		player->setPos(pos);
	}
}
