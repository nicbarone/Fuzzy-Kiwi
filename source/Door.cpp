#include "Door.h"
using namespace cugl;

#define DOOR_OFFSET 64
Door::Door() :
	_frame(0),
	_frameCounter(8)
{
	_keys = { }; 
	_blockedEnemy = nullptr;
	_isOpen = false;
	_unlocked = false;

}

void Door::dispose() {
	ConstructionElement::dispose();
	_blockedEnemy = nullptr;
	_isOpen = false;
	_unlocked = false;
	_keys = { 0 };
}

bool Door::init(int x, float ang, Vec2 scale, int level, Color4 color, std::vector<int> keys, int rows, int columns, std::shared_ptr<Texture> unlockedDoor, 
	std::shared_ptr<Texture> greenLockedDoor, std::shared_ptr<Texture> pinkLockedDoor, std::shared_ptr<Texture> redLockedDoor, std::shared_ptr<Texture> blueLockedDoor)
{

	if(keys.size()==0){
		setSceneNode(scene2::AnimationNode::alloc(unlockedDoor, rows, columns));
	}
	else if (keys[0] == 1) {
		setSceneNode(scene2::AnimationNode::alloc(redLockedDoor, rows, columns));

	}
	else if (keys[0] == 2)
	{
		setSceneNode(scene2::AnimationNode::alloc(blueLockedDoor, rows, columns));

	}
	else if (keys[0] == 3)
	{
		setSceneNode(scene2::AnimationNode::alloc(pinkLockedDoor, rows, columns));

	}
	else if (keys[0] == 4)
	{
		setSceneNode(scene2::AnimationNode::alloc(greenLockedDoor, rows, columns));

	}
	
	setPos(Vec2(x, level * FLOOR_HEIGHT + FLOOR_OFFSET + DOOR_OFFSET));
	setAngle(ang);
	setScale(scale);
	setLevel(level);
	setColor(color);
	_keys = keys;
	_frame = 0;
	_blockedEnemy = nullptr;
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
	if (_blockedEnemy != nullptr) {
		CULog("door's old patrol: %d", _blockedEnemy->getOldPatrol().y);
		Vec2 temp = Vec2(_blockedEnemy->getPatrol().x, _blockedEnemy->getPatrol().y);
		if (_blockedEnemy->isActive() ) {
			_blockedEnemy->setPatrol(_blockedEnemy->getOldPatrol().x, _blockedEnemy->getOldPatrol().y);
			_blockedEnemy->setOldPatrol(temp);
		}
		_blockedEnemy = nullptr;
		setBlockedEnemy(nullptr);
		CULog("%d", _blockedEnemy == nullptr);

	}
	
}

void  Door::doorOpenAnimation() {
	int i = 0;
	std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(i);
}