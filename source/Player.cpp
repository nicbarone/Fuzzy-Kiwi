#include "Player.h"
using namespace cugl;

/** how fast the player moves*/

Player::Player() :

	_nPossessions(1),
	_isPossessing(false),
	_isHidden(false),
	_movingRight(false),
	_frame(0),
	_frameCounter(12),
	_currentDoor(0)
{

	_sceneNode = nullptr;
	_texture = nullptr;
	_currentDen = 0;
}


void Player::dispose() {
	_nPossessions = 0;
	_isPossessing = false;
	_isHidden = false;
	_movingRight = false;
	_texture = nullptr;
	_sceneNode = nullptr;
	_frame = 0;
	_frameCounter = 7;
	_currentDoor = 0;
	_currentDen = 0;
	Entity::dispose();
}


bool Player::init(float x, int level, float ang, int num_frames, std::shared_ptr<Texture> cat)

{
	Entity::setPos(x);
	Entity::setLevel(level);
	Entity::setAngle(0);
	Entity::setLevel(level);
	_texture = cat;
	_sceneNode = scene2::AnimationNode::alloc(_texture, 1, num_frames);
	_sceneNode->setScale(0.15, 0.15);
	_sceneNode->setPosition(Vec2(x, level*FLOOR_HEIGHT+FLOOR_OFFSET-55));
	getSceneNode()->setPriority(level + 0.3f);
	return true;
}

void Player::move(float direction) {
	float original = getPos();
	manualMove(direction, PLAYER_SPEED);
	_sceneNode->setPositionX(original + getVelocity().x);
	if (direction == 1) {
		_movingRight = true;
		_sceneNode->setScale(0.15, 0.15);
	}
	else if (direction == -1) {
		_movingRight = false;
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
	_sceneNode->setPriority(level+0.3f);
}

void Player::PossessAnimation(int possessing) {
	//use this field in brackets now you can reference any field defined in player or class
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
	std::function<bool()> frame5 = [&]() {
		std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(5);
		return false;
	};
	std::function<bool()> frame6 = [&]() {
		std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(6);
		return false;
	};
	std::function<bool()> frame7 = [&]() {
		std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(7);
		return false;
	};
	if (possessing==0) {
		int timeDiff = 75;
		CULog("h");
		cugl::Application::get()->schedule(frame0, 50 + timeDiff * 1);
		cugl::Application::get()->schedule(frame1, 50 + timeDiff * 2);
		cugl::Application::get()->schedule(frame2, 50 + timeDiff * 3);
		cugl::Application::get()->schedule(frame3, 50 + timeDiff * 4);
	}
	else if (possessing == 1)
	{
		CULog("h2");
		int timeDiff = 75;
		cugl::Application::get()->schedule(frame4, 50 + timeDiff * 1);
		cugl::Application::get()->schedule(frame5, 50 + timeDiff * 2);
		cugl::Application::get()->schedule(frame6, 50 + timeDiff * 3);
		cugl::Application::get()->schedule(frame7, 50 + timeDiff * 4);
		//std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(7);

	}
	else if (possessing == 2) {
		int timeDiff = 25;
		CULog("h");
		cugl::Application::get()->schedule(frame4, 0 + timeDiff * 5);
		cugl::Application::get()->schedule(frame5, 0 + timeDiff * 6);
		cugl::Application::get()->schedule(frame6, 0 + timeDiff * 7);
		cugl::Application::get()->schedule(frame7, 0 + timeDiff * 8);
	}
	else if (possessing == 3) {
		int timeDiff = 75;
		CULog("h");
		cugl::Application::get()->schedule(frame0, 50 + timeDiff * 1);
		cugl::Application::get()->schedule(frame1, 50 + timeDiff * 2);
		cugl::Application::get()->schedule(frame2, 50 + timeDiff * 3);
		cugl::Application::get()->schedule(frame3, 50 + timeDiff * 4);
		cugl::Application::get()->schedule(frame4, 50 + timeDiff * 5);
		cugl::Application::get()->schedule(frame5, 50 + timeDiff * 6);
	}
	else if (possessing == 4) {
		int timeDiff = 75;
		CULog("h");
		cugl::Application::get()->schedule(frame0, 50 + timeDiff * 1);
		cugl::Application::get()->schedule(frame1, 50 + timeDiff * 2);
		cugl::Application::get()->schedule(frame2, 50 + timeDiff * 3);
		cugl::Application::get()->schedule(frame3, 50 + timeDiff * 4);
		cugl::Application::get()->schedule(frame4, 50 + timeDiff * 5);
		cugl::Application::get()->schedule(frame5, 50 + timeDiff * 6);
		cugl::Application::get()->schedule(frame6, 50 + timeDiff * 7);
	}
}

	void Player::EnemyDying() {

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
		std::function<bool()> frame5 = [&]() {
			std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(5);
			return false;
		};
		std::function<bool()> frame6 = [&]() {
			std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(6);
			return false;
		};
		std::function<bool()> frame7 = [&]() {
			std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(7);
			return false;
		};


		int timeDiff = 130;
		CULog("h");
		cugl::Application::get()->schedule(frame0, 50 + timeDiff * 1);
		cugl::Application::get()->schedule(frame1, 50 + timeDiff * 2);
		cugl::Application::get()->schedule(frame2, 50 + timeDiff * 3);
		cugl::Application::get()->schedule(frame3, 50 + timeDiff * 4);
		cugl::Application::get()->schedule(frame5, 50 + timeDiff * 5);
		cugl::Application::get()->schedule(frame6, 50 + timeDiff * 6);
		cugl::Application::get()->schedule(frame7, 50 + timeDiff * 7);
	
	}