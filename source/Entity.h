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

#pragma setters and getters
	Vec2 get_pos() {
		return _pos;
	}

	float get_angle() {
		return _angle;
	}

	Vec2 get_size() {
		return _size;
	}

	void set_texture(const std::shared_ptr<Texture> texture) {
		
	}

	void set_textures(vector<std::shared_ptr<Texture>> textures) {

	}

	std::shared_ptr<Texture> get_texture() {
		
	}

	void set_color(Color4 color) {
		_color = color;
	}

	Color4 get_color() {
		return _color;
	}

	
};
#endif