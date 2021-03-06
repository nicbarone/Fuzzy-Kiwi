#pragma once
#ifndef __ENTITY_H__
#define __ENTITY_H__
#include <cugl/cugl.h>
using namespace cugl;
class Entity {

private:

	Vec2 _pos;
	Vec2 _size;
	float _angle = 0;
	Color4 _color;


public:

	Entity();

	~Entity() { dispose(); }

	void dispose();

	/** sets the position of the entity*/
	void set_pos(Vec2 pos) {
		_pos = pos;
	}

	/** returns a Vec2 representing the position of the entity*/
	Vec2 get_pos() {
		return _pos;
	}

	/** sets the angle of the entity*/
	void set_angle(float angle) {
		_angle = angle;
	}

	/** returns a float representing the angle of the entity*/
	float get_angle() {
		return _angle;
	}

	/** returns a Vec2 representing the width and height of the entity*/
	Vec2 get_size() {
		return _size;
	}

	void set_texture(const std::shared_ptr<Texture> texture) {
		
	}

	void set_textures(vector<std::shared_ptr<Texture>> textures) {

	}

	std::shared_ptr<Texture> get_texture() {
		
	}

	/** sets the color of the entity*/
	void set_color(Color4 color) {
		_color = color;
	}

	/** returns a Color4 representing the color of the entity*/
	Color4 get_color() {
		return _color;
	}

	
};
#endif