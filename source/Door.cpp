#include "Door.h"
using namespace cugl;

#define DOOR_OFFSET 64
Door::Door() :
	_frame(0),
	_frameCounter(8)
{
	_keys = {}; 
	_blockedEnemy = {};
	_isOpen = false;
	_unlocked = false;
	_doorLock = nullptr;

}

void Door::dispose() {
	ConstructionElement::dispose();
	_blockedEnemy = {};
	_isOpen = false;
	_unlocked = false;
	_keys = {  };
	_doorLock = nullptr;
}

bool Door::init(int x, float ang, Vec2 scale, int level, Color4 color, std::vector<int> keys, int rows, int columns, std::shared_ptr<Texture> unlockedDoor, 
	std::shared_ptr<Texture> greenLockedDoor, std::shared_ptr<Texture> pinkLockedDoor, std::shared_ptr<Texture> redLockedDoor, std::shared_ptr<Texture> blueLockedDoor)
{
	setSceneNode(scene2::AnimationNode::alloc(unlockedDoor, rows, columns));
	if(keys.size()==0|| keys[0] == 5){
		_doorLock = scene2::PolygonNode::allocWithTexture(redLockedDoor);
		_doorLock->setScale(0.0025, 0.0025);
	}
	else if (keys[0] == 1) {
		_doorLock = scene2::PolygonNode::allocWithTexture(redLockedDoor);
		_doorLock->setScale(0.25, 0.25);
	}
	else if (keys[0] == 2)
	{
		_doorLock = scene2::PolygonNode::allocWithTexture(blueLockedDoor);
		_doorLock->setScale(0.25, 0.25);

	}
	else if (keys[0] == 3)
	{
		_doorLock = scene2::PolygonNode::allocWithTexture(pinkLockedDoor);
		_doorLock->setScale(0.25, 0.25);

	}
	else if (keys[0] == 4)
	{
		_doorLock = scene2::PolygonNode::allocWithTexture(greenLockedDoor);
		_doorLock->setScale(0.25, 0.25);

	}
	_doorLock->setPosition(240, 205);
	
	_doorLock->setPriority(level + 0.6f);
	getSceneNode()->addChild(_doorLock);

	setPos(Vec2(x, level * FLOOR_HEIGHT + FLOOR_OFFSET + DOOR_OFFSET));
	setAngle(ang);
	setScale(scale);
	setLevel(level);
	setColor(color);
	_keys = keys;
	_frame = 0;
	_blockedEnemy = {};
	_isOpen = false;
	getSceneNode()->setPriority(level + 0.1f);
	
	return true;
}

void Door::setDoor(bool open) {
	_isOpen = open;
	std::function<bool()> frame0 = [&]() {
		std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(0);
		return false;
	};
	std::function<bool()> frame1 = [&](){
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
	std::function<bool()> frame8 = [&]() {
		std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(8);
		return false;
	};
	if (open) {
		int timeDiff = 25;
		cugl::Application::get()->schedule(frame1, 50 + timeDiff);
		cugl::Application::get()->schedule(frame2, 50 + timeDiff*2);
		cugl::Application::get()->schedule(frame3, 50 + timeDiff*3);
		cugl::Application::get()->schedule(frame4, 50 + timeDiff*4);
		cugl::Application::get()->schedule(frame5, 50 + timeDiff*5);
		cugl::Application::get()->schedule(frame6, 50 + timeDiff*6);
		cugl::Application::get()->schedule(frame7, 50 + timeDiff*7);
	}
	else
	{
		int timeDiff = 25;
		cugl::Application::get()->schedule(frame6, 50 + timeDiff);
		cugl::Application::get()->schedule(frame5, 50 + timeDiff * 2);
		cugl::Application::get()->schedule(frame4, 50 + timeDiff * 3);
		cugl::Application::get()->schedule(frame3, 50 + timeDiff * 4);
		cugl::Application::get()->schedule(frame2, 50 + timeDiff * 5);
		cugl::Application::get()->schedule(frame1, 50 + timeDiff * 6);
		cugl::Application::get()->schedule(frame0, 50 + timeDiff * 7);
	}
	//std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(_frame);
	if (_blockedEnemy.size() != 0) {
		for (shared_ptr<Enemy> blockedEnemy : _blockedEnemy)
		{
			Vec2 temp = Vec2(blockedEnemy->getPatrol().x, blockedEnemy->getPatrol().y);
			if (blockedEnemy->isActive()) {
				blockedEnemy->setPatrol(blockedEnemy->getOldPatrol().x, blockedEnemy->getOldPatrol().y);
				blockedEnemy->setOldPatrol(temp);
			}
			_blockedEnemy = {};
			//CULog("%d", _blockedEnemy == nullptr);
		}
		_blockedEnemy = {};
		

	}
	
}

void  Door::doorOpenAnimation() {
	int i = 0;
	std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(i);
}