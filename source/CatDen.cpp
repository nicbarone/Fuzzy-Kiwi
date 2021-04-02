#include "CatDen.h"
using namespace cugl;

CatDen::CatDen() :
	_frame(0),
	_frameCounter(9)
{
	_isOpen = false;
}

void CatDen::dispose() {
	ConstructionElement::dispose();
	_isOpen = false;
}

bool CatDen::init(int x, float ang, Vec2 scale, int level, Color4 color, int rows, int columns, std::shared_ptr<Texture> texture)
{
	setSceneNode(scene2::AnimationNode::alloc(texture, rows, columns));
	setTexture(texture);
	setPos(Vec2(x, level * FLOOR_HEIGHT + FLOOR_OFFSET+ CAT_DEN_OFFSET));
	setAngle(ang);
	setScale(scale);
	setLevel(level);
	setColor(color);
	_frame = 0;
	_isOpen = false;
	return true;
}

void CatDen::setDoor(bool open) {
	_isOpen = open;
	CULog("staircase door state %d", _isOpen);
	if (open) {

		for (int i = 0; i < 7; i++) {
			std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(i);
		}
	}
	else
	{
		for (int i = 7; i > -1; i--) {
			std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(i);
		}
	}

}

void  CatDen::doorOpenAnimation() {
	int i = 0;
	std::dynamic_pointer_cast<scene2::AnimationNode>(getSceneNode())->setFrame(i);
}