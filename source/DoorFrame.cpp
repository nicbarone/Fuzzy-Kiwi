#include "DoorFrame.h"
using namespace cugl;

#define DOOR_FRAME_OFFSET 43
DoorFrame::DoorFrame() :
	_frame(0),
	_frameCounter(8)
{
	_keys = { 0 };
	_blockedEnemy = nullptr;
	_isOpen = false;

}

void DoorFrame::dispose() {
	ConstructionElement::dispose();
	_blockedEnemy = nullptr;
	_isOpen = false;
	_keys = { 0 };
}

bool DoorFrame::init(int x, float ang, Vec2 scale, int level, Color4 color, std::vector<int> keys, int rows, int columns, std::shared_ptr<Texture> texture)
{
	setSceneNode(scene2::AnimationNode::alloc(texture, rows, columns));
	setTexture(texture);
	setPos(Vec2(x, level * FLOOR_HEIGHT + FLOOR_OFFSET + DOOR_FRAME_OFFSET));
	setAngle(ang);
	setScale(scale);
	setLevel(level);
	setColor(color);
	_keys = keys;
	_frame = 0;
	_blockedEnemy = nullptr;
	_isOpen = false;
	return true;
}
