#include "CatDen.h"
using namespace cugl;

CatDen::CatDen() :
	_frame(0),
	_frameCounter(9),
	_connectedDens(0)
{
	_isOpen = false;
}

void CatDen::dispose() {
	ConstructionElement::dispose();
	_isOpen = false;
	_connectedDens = 0;
}

bool CatDen::init(int x, float ang, Vec2 scale, int level, Color4 color, int connectedDens, int rows, int columns, std::shared_ptr<Texture> texture,
	std::shared_ptr<Texture> greenTexture, std::shared_ptr<Texture> purpleTexture, std::shared_ptr<Texture> blueTexture, std::shared_ptr<Texture> greyTexture)
{
	_connectedDens = connectedDens;
	if (_connectedDens == 1)
	{
		setSceneNode(scene2::AnimationNode::alloc(texture, rows, columns));
	}
	else if (_connectedDens == 2)
	{
		setSceneNode(scene2::AnimationNode::alloc(greenTexture, rows, columns));
	}
	else if (_connectedDens == 3)
	{
		setSceneNode(scene2::AnimationNode::alloc(purpleTexture, rows, columns));
	}
	else if (_connectedDens == 4)
	{
		setSceneNode(scene2::AnimationNode::alloc(blueTexture, rows, columns));
	}
	else if (_connectedDens == 5)
	{
		setSceneNode(scene2::AnimationNode::alloc(greyTexture, rows, columns));
	}
	setTexture(texture);
	setPos(Vec2(x, level * FLOOR_HEIGHT + FLOOR_OFFSET + CAT_DEN_OFFSET));
	setAngle(ang);
	setScale(scale);
	setLevel(level);
	setColor(color);
	_frame = 0;
	_isOpen = false;
	getSceneNode()->setPriority(level);
	return true;
}

void CatDen::setDoor(bool open) {
	_isOpen = open;
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
		int timeDiff = 25;
		cugl::Application::get()->schedule(frame1, 50 + timeDiff);
		cugl::Application::get()->schedule(frame2, 50 + timeDiff * 2);
		cugl::Application::get()->schedule(frame3, 50 + timeDiff * 3);
		cugl::Application::get()->schedule(frame4, 50 + timeDiff * 4);
		cugl::Application::get()->schedule(frame5, 50 + timeDiff * 5);
		cugl::Application::get()->schedule(frame6, 50 + timeDiff * 6);
	}
	else
	{
		int timeDiff = 25;
		cugl::Application::get()->schedule(frame5, 50 + timeDiff);
		cugl::Application::get()->schedule(frame4, 50 + timeDiff * 2);
		cugl::Application::get()->schedule(frame3, 50 + timeDiff * 3);
		cugl::Application::get()->schedule(frame2, 50 + timeDiff * 4);
		cugl::Application::get()->schedule(frame1, 50 + timeDiff * 5);
		cugl::Application::get()->schedule(frame0, 50 + timeDiff * 6);
	}

}

void  CatDen::doorOpenAnimation() {
	int i = 0;
	std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(i);
}