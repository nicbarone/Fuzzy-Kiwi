#ifndef __COLLISION_MANAGER_H__
#define __COLLISION_MANAGER_H__
#include <cugl/cugl.h>
#include "Entity.h"
#include "Player.h"
#include "Door.h"
#include "Enemy.h"

/**
 * Namespace of functions implementing simple game physics.
 *
 * This is the simplest of physics engines. In reality, you will
 * probably use Box2d just like you did in 3152.
 */
namespace collisions {
	/**
	 *  Handles collisions the player and wall, causing the player to stop further movement.
	 *
	 *  This method updates the velocities of the cat.
	 *
	 *  @param player    Player in candidate collision
	 *  @param entity    Entity in candidate collision
	 */
	void checkForDoorCollision(const std::shared_ptr<Enemy>& possessedEnemy,
		const std::shared_ptr<Player>& player, const  std::vector<shared_ptr<Door>>& doors);

	/**
	 * Nudge the player to ensure it does not do out of view.
	 *
	 * This code stops the cat gets out of the screen
	 *
	 * @param player      They player which may have collided
	 * @param bounds    The rectangular bounds of the playing field
	 */
	void checkInBounds(const std::shared_ptr<Player>& player, const cugl::Rect bounds);
}

#endif /* __COLLISION_MANAGER_H__ */
