#pragma once
#ifndef __FLOOR_H__
#define __FLOOR_H__
#include "ConstructionElement.h"
#include <cugl/cugl.h>
#include "Constants.h"
using namespace cugl;

class Floor : public ConstructionElement {

public:

	Floor();

	~Floor() { dispose(); }

	void dispose();

	bool init(int x, float ang, Vec2 scale, int level, Color4 color, int rows, int columns,
		std::shared_ptr<Texture> texture);

	static std::shared_ptr<Floor> alloc(int x, float ang, Vec2 scale, int level, Color4 color, 
		int rows, int columns, std::shared_ptr<Texture> texture) {
		std::shared_ptr<Floor> result = std::make_shared<Floor>();
		return (result->init(x, ang, scale, level, color, rows, columns, texture) ? result : nullptr);
	}



};

#endif/* __FLOOR_H__ */