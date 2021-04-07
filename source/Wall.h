#pragma once
#ifndef __WALL_H__
#define __WALL_H__
#include "ConstructionElement.h"
#include <cugl/cugl.h>
#include "Constants.h"
using namespace cugl;

class Wall : public ConstructionElement {

public:

	Wall();

	~Wall() { dispose(); }

	void dispose();

	bool init(int x, float ang, Vec2 scale, int level, Color4 color, int rows, int columns,
		std::shared_ptr<Texture> texture);

	static std::shared_ptr<Wall> alloc(int x, float ang, Vec2 scale, int level, Color4 color,
		int rows, int columns, std::shared_ptr<Texture> texture) {
		std::shared_ptr<Wall> result = std::make_shared<Wall>();
		return (result->init(x, ang, scale, level, color, rows, columns, texture) ? result : nullptr);
	}



};

#endif/* __WAll_H__ */