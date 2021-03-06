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
void collisions::checkForCollision(const std::shared_ptr<Player>& player, const std::shared_ptr<Entity>& entity)
{
    // Calculate the normal of the (possible) point of collision
    float distance = player->get_pos().x - entity->get_pos().x;
    float impactDistance = player->get_size().x;

    // If this normal is too small, there was a collision
    if (distance < impactDistance) {
        // "Roll back" time so that the ships are barely touching (e.g. point of impact).
        Vec2 temp = Vec2(0, distance > 0 ? 1 : -1) * ((impactDistance - distance) / 2);
        player->set_pos(player->get_pos() + temp);

        // Stop the movement of player
        player->setVelocity(Vec2::ZERO);
        return; // Collision checked and complete
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
    Vec2 vel = player->getVelocity();
    Vec2 pos = player->get_pos();

    //Ensure player doesn't go out of view. Stop by walls
    if (pos.x <= bounds.origin.x) {
        vel.x = 0;
        pos.x = bounds.origin.x;
        player->setVelocity(vel);
        player->set_pos(pos);
    }
    else if (pos.x >= bounds.size.width + bounds.origin.x) {
        vel.x = 0;
        pos.x = bounds.size.width + bounds.origin.x - 1.0f;
        player->setVelocity(vel);
        player->set_pos(pos);
    }

    if (pos.y <= bounds.origin.y) {
        vel.y = 0;
        pos.y = bounds.origin.y;
        player->setVelocity(vel);
        player->set_pos(pos);
    }
    else if (pos.y >= bounds.size.height + bounds.origin.y) {
        vel.y = 0;
        pos.y = bounds.size.height + bounds.origin.y - 1.0f;
        player->setVelocity(vel);
        player->set_pos(pos);
    }
}
