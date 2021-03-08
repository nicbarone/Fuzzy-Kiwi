#pragma once
#ifndef __FLOOR_H__
#define __FLOOR_H__
#include <cugl/cugl.h>
#include "Entity.h"
using namespace cugl;
class Floor : public Entity {

private:

	std::shared_ptr<scene2::PolygonNode> _sceneNode;
	std::shared_ptr<Texture> _texture;
	std::vector<Vec2> _vertices;


public:

	Floor();

	~Floor() { dispose(); }

	void dispose();

	bool init(float x, float y, float ang, std::vector<Vec2> vertices, std::shared_ptr<Texture> cat);

	static std::shared_ptr<Floor> alloc(float x, float y, float ang, std::vector<Vec2> vertices, std::shared_ptr<Texture> cat) {
		std::shared_ptr<Floor> result = std::make_shared<Floor>();
		return (result->init(x, y, ang, vertices, cat) ? result : nullptr);
	}


	/** returns the AnimationNode associated with the player*/
	std::shared_ptr<scene2::PolygonNode> getSceneNode() {
		return _sceneNode;
	}

};

#endif/* __FLOOR_H__ */