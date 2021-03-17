#pragma once
#ifndef __DOOR_H__
#define __DOOR_H__
#include <Interactable.h>
#include <cugl/cugl.h>
using namespace cugl;

class Door : public Interactable {

public:

	Door();

	~Door() { dispose(); }

	void dispose();

	bool init(Vec2 pos, float ang, Vec2 scale, int level, Color4 color,
		std::shared_ptr<Texture> texture);

	static std::shared_ptr<Door> alloc(Vec2 pos, float ang, Vec2 scale, int level, Color4 color, 
		std::shared_ptr<Texture> texture) {
		std::shared_ptr<Door> result = std::make_shared<Door>();
		return (result->init(pos, ang, scale, level, color, texture) ? result : nullptr);
	}



};

#endif/* __DOOR_H__ */