#pragma once
#ifndef __CONSTRUCTIONELEMENT_H__
#define __CONSTRUCTIONELEMENT_H__
#include <cugl/cugl.h>
using namespace cugl;
class ConstructionElement {

private:
	/** Position of the ship */
	Vec2 _pos;
	Vec2 _size;
	float _angle;
	Color4 _color;
	int _level;
	std::shared_ptr<Texture> _texture;
	std::vector<Vec2> _vertices;


public:

	ConstructionElement();

	~ConstructionElement() { dispose(); }

	void dispose();

	/** sets the position of the construction element*/
	void setPos(Vec2 pos) {
		_pos = pos;
	}

	/** returns a Vec2 representing the position of the construction element*/
	Vec2 getPos() {
		return _pos;
	}

	/** sets the angle of the construction element*/
	void setAngle(float angle) {
		_angle = angle;
	}

	/** returns a float representing the angle of the construction element*/
	float getAngle() {
		return _angle;
	}
	/** sets the level of the entity*/
	void setLevel(int level) {
		_level = level;
	}

	/** returns a int representing the level of the construction element*/
	int getLevel() {
		return _level;
	}

	void setTexture(const std::shared_ptr<Texture> texture) {
		_texture = texture;
	}
	/** returns a texture representing the floor level of the construction element*/
	std::shared_ptr<Texture>  getTextures() {
		return _texture;
	}

	/** sets the color of the construction element*/
	void setColor(Color4 color) {
		_color = color;
	}

	/** returns a Color4 representing the color of the construction element*/
	Color4 getColor() {
		return _color;
	}

	/** sets a vector representing the vertices of the construction element*/
	void setVertices(vector<Vec2> vertices) {
		_vertices = vertices;
	}

	/** returns a vector representing the vertices of the construction element*/
	std::vector<Vec2> getVertices() {
		return _vertices;
	}

	
};

#endif/* __CONSTRUCTIONELEMENT_H__ */