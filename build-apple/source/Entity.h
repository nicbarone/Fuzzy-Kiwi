#pragma once
#ifndef __ENTITY_H__
#define __ENTITY_H__
#include "cugl/cugl.h"
using namespace cugl;

extern const float FLOOR_HEIGHT;
extern const float FLOOR_OFFSET;

class Entity {

private:

	float _pos;
	Vec2 _size;
	float _angle;
	Color4 _color;
	int _level;
	Vec2 _velocity;

public:

	Entity();

	~Entity() { dispose(); }

	void dispose();

	/** sets the x position of the entity*/
	void setPos(float pos) {
		_pos = pos;
	}


	/** returns a float representing the x position of the entity*/
	float getPos() {
		return _pos;
	}

	/** sets the angle of the entity*/
	void setAngle(float angle) {
		_angle = angle;
	}

	/** returns a float representing the angle of the entity*/
	float getAngle() {
		return _angle;
	}

	/** sets a Vec2 representing the width and height of the entity*/
	void setSize(Vec2 size) {
		_size = size;
	}

	/** returns a Vec2 representing the width and height of the entity*/
	Vec2 getSize() {
		return _size;
	}

	void setLevel(int level) {
		_level = level;
	}

	int getLevel() {
		return _level;
	}

	float getPosY() {
		return _level * FLOOR_HEIGHT + FLOOR_OFFSET;
	}

	void setTexture(const std::shared_ptr<Texture> texture) {
		
	}

	void setTextures(vector<std::shared_ptr<Texture>> textures) {

	}


	/** sets the color of the entity*/
	void setColor(Color4 color) {
		_color = color;
	}

	/** returns a Color4 representing the color of the entity*/
	Color4 getColor() {
		return _color;
	}

	/** sets the velocity of the entity*/
	void setVelocity(Vec2 velocity) {
		_velocity = velocity;
	}

	/** returns a float representing the velocity of the entity*/
	Vec2 getVelocity() {
		return _velocity;
	}

	/** handling movement with player input*/
	void manualMove(float direction, float speed);
	
};
#endif