#pragma once
#ifndef __CAGEDANIMAL_H__
#define __CAGEDANIMAL_H__
#include "ConstructionElement.h"
#include "Enemy.h"
#include <cugl/cugl.h>
#include "Constants.h"
using namespace cugl;

class CagedAnimal : public ConstructionElement {
private:
	int _frame;
	int _frameCounter;
	bool _isOpen;
	std::shared_ptr<Texture> _texture;
	std::shared_ptr<scene2::AnimationNode> _sceneNode;
	/*list of keys that the door.*/
	std::vector<int> _keys;
public:

	CagedAnimal();

	~CagedAnimal() { dispose(); }

	void dispose();

	bool init(int x, float ang, Vec2 scale, int level, Color4 color, std::vector<int> keys, int rows, int columns,
		int num_frames, std::shared_ptr<Texture> texture);

	static std::shared_ptr<CagedAnimal> alloc(int x, float ang, Vec2 scale, int level, Color4 color, std::vector<int> keys, int rows, int columns,
		int num_frames, std::shared_ptr<Texture> texture) {
		std::shared_ptr<CagedAnimal> result = std::make_shared<CagedAnimal>();
		return (result->init(x, ang, scale, level, color, keys, rows, columns,  num_frames, texture) ? result : nullptr);
	}

	void CagedAnimal::Free();
	
	void SetSceneNode(std::shared_ptr<scene2::AnimationNode> newNode) {
		_sceneNode = newNode;
		_sceneNode->setPriority(getLevel());
	}
	/** returns the AnimationNode associated with the player*/
	std::shared_ptr<scene2::AnimationNode> getSceneNode() {
		return _sceneNode;
	}
};

#endif/* __CAGEDANIMAL_H__ */