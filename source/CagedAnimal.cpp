#include "CagedAnimal.h"
using namespace cugl;

#define CAGED_ANIMAL_OFFSET -10
CagedAnimal::CagedAnimal() :
	_frame(0),
	_frameCounter(8)
	
{
	_texture = nullptr;
	_sceneNode = nullptr;
}

void CagedAnimal::dispose() {
	ConstructionElement::dispose();
	_texture = nullptr;
	_sceneNode = nullptr;
}

bool CagedAnimal::init(int x, float ang, Vec2 scale, int level, Color4 color, std::vector<int> keys, int rows, int columns, int num_frames, std::shared_ptr<Texture> texture)
{
	//setSceneNode(scene2::AnimationNode::alloc(texture, rows, columns));
	_sceneNode = scene2::AnimationNode::alloc(_texture, 1, num_frames);
	setTexture(texture);
	_texture = texture;
	setPos(Vec2(x, level * FLOOR_HEIGHT + FLOOR_OFFSET + CAGED_ANIMAL_OFFSET));
	setAngle(ang);
	setScale(scale);
	setLevel(level);
	setColor(color);
	_keys = keys;
	_frame = 0;
	_isOpen = false;
	getSceneNode()->setPriority(level);
	return true;
}


void CagedAnimal::Free() {
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
