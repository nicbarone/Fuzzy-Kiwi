#include "StaircaseDoor.h"
using namespace cugl;

#define STAIRCASE_DOOR_OFFSET 30
StaircaseDoor::StaircaseDoor() :
	_frame(0),
	_frameCounter(9)
{
	_isOpen = false;
}

void StaircaseDoor::dispose() {
	ConstructionElement::dispose();
	_isOpen = false;
}

bool StaircaseDoor::init(int x, float ang, Vec2 scale, int level, Color4 color, std::vector<int> keys, int rows, int columns, std::shared_ptr<Texture> texture)
{
	setSceneNode(scene2::AnimationNode::alloc(texture, rows, columns));
	setTexture(texture);
	setPos(Vec2(x, level * FLOOR_HEIGHT + FLOOR_OFFSET+ STAIRCASE_DOOR_OFFSET));
	setAngle(ang);
	setScale(scale);
	setLevel(level);
	setColor(color);
	_keys = keys;
	_frame = 0;
	_isOpen = false;
	return true;
}

void StaircaseDoor::setDoor(bool open) {
	_isOpen = open;
	CULog("staircase door state %d", _isOpen);

	if (open) {

		for (int i = 0; i < 7; i++) {
			std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(i);
		}
		/*cugl::Application::schedule(Door::doorOpenAnimation, 3);*/
	}
	else
	{
		for (int i = 7; i > -1; i--) {
			std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(i);
		}
	}

}

void  StaircaseDoor::doorOpenAnimation() {
	int i = 0;
	std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(i);
}