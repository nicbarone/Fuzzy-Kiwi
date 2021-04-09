#include "Player.h"
using namespace cugl;

/** how fast the player moves*/

Player::Player() :

	_nPossessions(1),
	_isPossessing(false),
	_isHidden(false),
	_frame(0),
	_frameCounter(12)
{

	_sceneNode = nullptr;
	_texture = nullptr;
}


void Player::dispose() {
	_nPossessions = 0;
	_isPossessing = false;
	_isHidden = false;
	_texture = nullptr;
	_sceneNode = nullptr;
	_frame = 0;
	_frameCounter = 7;
	Entity::dispose();
}


bool Player::init(float x, int level, float ang, std::shared_ptr<Texture> cat)

{
	Entity::setPos(x);
	Entity::setLevel(level);
	Entity::setAngle(0);
	Entity::setLevel(level);
	_texture = cat;
	_sceneNode = scene2::AnimationNode::alloc(_texture, 1, 8);
	_sceneNode->setScale(0.15, 0.15);
	_sceneNode->setPosition(Vec2(x, level*FLOOR_HEIGHT+FLOOR_OFFSET-55));
	return true;
}

void Player::move(float direction) {
	float original = getPos();
	manualMove(direction, PLAYER_SPEED);
	_sceneNode->setPositionX(original + getVelocity().x);
	if (direction == 1) {
		_sceneNode->setScale(0.15, 0.15);
	}
	else if (direction == -1) {
		_sceneNode->setScale(-0.15, 0.15);
	}
	if (direction != 0)
	{
		if (_frameCounter > 0) {
			_frameCounter--;
		}
		else {
			_frameCounter = 7;
			_frame++;
			_frame = _frame % 8;
		}
	}
	_sceneNode->setFrame(_frame);
}

void Player::setLevel(int level) {
	Entity::setLevel(level);
	_sceneNode->setPositionY(Entity::getLevel() * FLOOR_HEIGHT + FLOOR_OFFSET + PLAYER_OFFSET);
}

void Player::PossessAnimation(bool possessing) {
	std::function<bool()> frame0 = [&]() {
		std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(0);
		return false;
	};
	std::function<bool()> frame1 = [&]() {
		std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(1);
		return false;
	};
	std::function<bool()> frame2 = [&]() {
		std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(2);
		return false;
	};
	std::function<bool()> frame3 = [&]() {
		std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(3);
		return false;
	};
	std::function<bool()> frame4 = [&]() {
		std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(4);
		return false;
	};
	if (possessing) {
		int timeDiff = 25;
		cugl::Application::get()->schedule(frame0, 50 + timeDiff);
		cugl::Application::get()->schedule(frame2, 50 + timeDiff * 2);
		cugl::Application::get()->schedule(frame3, 50 + timeDiff * 3);
		cugl::Application::get()->schedule(frame4, 50 + timeDiff * 4);
	}
	else
	{
		int timeDiff = 25;
		cugl::Application::get()->schedule(frame3, 50 + timeDiff * 4);
		cugl::Application::get()->schedule(frame2, 50 + timeDiff * 5);
		cugl::Application::get()->schedule(frame1, 50 + timeDiff * 6);
		cugl::Application::get()->schedule(frame0, 50 + timeDiff * 7);
	}


}