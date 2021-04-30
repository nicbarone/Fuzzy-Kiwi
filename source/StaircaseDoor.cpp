#include "StaircaseDoor.h"
using namespace cugl;

StaircaseDoor::StaircaseDoor() :
	_frame(0),
	_frameCounter(9)
{
	_connectedDoors = 0;
	_isOpen = false;
}

void StaircaseDoor::dispose() {
	ConstructionElement::dispose();
	_connectedDoors = 0;
	_isOpen = false;
}

bool StaircaseDoor::init(int x, float ang, Vec2 scale, int level, Color4 color, std::vector<int> keys, int connectedDoors, 
	int rows, int columns, std::shared_ptr<Texture> texture, std::shared_ptr<Texture> Redtexture, 
	std::shared_ptr<Texture> Purpletexture, std::shared_ptr<Texture> yellowTexture, std::shared_ptr<Texture> Orangetexture)
{
	setSceneNode(scene2::AnimationNode::alloc(texture, rows, columns));
	setPos(Vec2(x, level * FLOOR_HEIGHT + FLOOR_OFFSET+ STAIRCASE_DOOR_OFFSET));
	setAngle(ang);
	setScale(scale);
	setLevel(level);
	_connectedDoors = connectedDoors;
	if (_connectedDoors == 1)
	{
		setTexture(texture);
	}
	if (_connectedDoors == 2)
	{
		setTexture(Redtexture);
	}
	if (_connectedDoors == 3)
	{
		setTexture(Purpletexture);
	}
	if (_connectedDoors == 4)
	{
		setTexture(yellowTexture);
	}
	if (_connectedDoors == 5)
	{
		setTexture(Orangetexture);
	}

	_keys = keys;
	_frame = 0;
	_isOpen = false;
	getSceneNode()->setPriority(level);
	return true;
}

void StaircaseDoor::setDoor(bool open) {
	_isOpen = open;
	//CULog("staircase %d", open);
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
	std::function<bool()> frame8 = [&]() {
		std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(8);
		return false;
	};
	if (open) {
		int timeDiff = 50;
		cugl::Application::get()->schedule(frame1, 50 + timeDiff);
		cugl::Application::get()->schedule(frame2, 50 + timeDiff * 2);
		cugl::Application::get()->schedule(frame3, 50 + timeDiff * 3);
		cugl::Application::get()->schedule(frame4, 50 + timeDiff * 4);
		cugl::Application::get()->schedule(frame5, 50 + timeDiff * 5);
		cugl::Application::get()->schedule(frame6, 50 + timeDiff * 6);
		cugl::Application::get()->schedule(frame7, 50 + timeDiff * 7);
	}
	else
	{
		int timeDiff = 50;
		cugl::Application::get()->schedule(frame6, 50 + timeDiff);
		cugl::Application::get()->schedule(frame5, 50 + timeDiff * 2);
		cugl::Application::get()->schedule(frame4, 50 + timeDiff * 3);
		cugl::Application::get()->schedule(frame3, 50 + timeDiff * 4);
		cugl::Application::get()->schedule(frame2, 50 + timeDiff * 5);
		cugl::Application::get()->schedule(frame1, 50 + timeDiff * 6);
		cugl::Application::get()->schedule(frame0, 50 + timeDiff * 7);
	}

}

void  StaircaseDoor::doorOpenAnimation() {
	int i = 0;
	std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(i);
}