#pragma once
#ifndef __FLOOR_H__
#define __FLOOR_H__
#include <ConstructionElement.h>
#include <cugl/cugl.h>
using namespace cugl;

class Floor : public ConstructionElement {

public:

	Floor();

	~Floor() { dispose(); }

	void dispose();

	bool init(Vec2 pos, float ang, Vec2 scale, int level, Color4 color, std::vector<Vec2> vertices, 
		std::shared_ptr<Texture> cat);

	static std::shared_ptr<Floor> alloc(Vec2 pos, float ang, Vec2 scale, int level, Color4 color, 
		std::vector<Vec2> vertices, std::shared_ptr<Texture> cat) {
		std::shared_ptr<Floor> result = std::make_shared<Floor>();
		return (result->init(pos, ang, scale, level, color, vertices, cat) ? result : nullptr);
	}



};

#endif/* __FLOOR_H__ */