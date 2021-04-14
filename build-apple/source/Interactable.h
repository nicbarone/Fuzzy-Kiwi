#pragma once
#ifndef __Interactable_H__
#define __Interactable_H__
#include "cugl/cugl.h"
#include "cugl/scene2/graph/CUSceneNode.h"
using namespace cugl;
class Interactable {

private:
	Vec2 _pos;
	float _angle;
	Vec2 _scale;
	Color4 _color;
	int _level;
	std::shared_ptr<Texture> _texture;
	std::shared_ptr<scene2::AnimationNode> _sceneNode;


public:

	Interactable();

	~Interactable() { dispose(); }

	void dispose();

	/** sets the position of the construction element*/
	void setPos(Vec2 pos) {
		_pos = pos;
		_sceneNode->setPosition(pos);
	}

	/** returns a Vec2 representing the position of the construction element*/
	Vec2 getPos() {
		//CULog("pos %d", _pos.x);
		return _pos;
	}

	/** sets the angle of the construction element*/
	void setAngle(float angle) {
		_angle = angle;
		_sceneNode->setAngle(angle);
	}

	/** returns a float representing the angle of the construction element*/
	float getAngle() {
		return _angle;
	}

	/** sets the scale of the construction element*/
	void setScale(Vec2 scale) {
		_scale = scale;
		_sceneNode->setScale(scale);
	}

	/** returns a Vec2 representing the scale of the construction element*/
	Vec2 getScale() {
		return _scale;
	}

	/** returns a texture representing the floor level of the construction element*/
	void  setTexture(std::shared_ptr<Texture> texture) {
		_texture = texture;
	}

	/** returns a texture representing the floor level of the construction element*/
	std::shared_ptr<Texture>  getTexture() {
		return _texture;
	}

	/** sets the color of the construction element*/
	void setColor(Color4 color) {
		_color = color;
		_sceneNode->setColor(color);
	}

	/** returns a Color4 representing the color of the construction element*/
	Color4 getColor() {
		return _color;
	}

	/** sets the floor level of the construction element*/
	void setLevel(int level) {
		_level = level;
	}

	/** returns an int representing the floor level of the construction element*/
	int getLevel() {
		return _level;
	}


	/** returns the SceneNode associated with the construction element*/
	std::shared_ptr<scene2::AnimationNode> getSceneNode() {
		return _sceneNode;
	}
	/** sets the SceneNode associated with the construction element*/
	void setSceneNode(shared_ptr<scene2::AnimationNode>sceneNode) {
		_sceneNode = sceneNode;
	}

};

#endif/* __Interactable_H__ */