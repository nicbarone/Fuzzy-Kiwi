#pragma once
#ifndef __CATDEN_H__
#define __CATDEN_H__
#include "ConstructionElement.h"
#include "Enemy.h"
#include <cugl/cugl.h>
#include "Constants.h"
using namespace cugl;

class CatDen : public ConstructionElement {
private:
	int _frame;
	int _frameCounter;
	bool _isOpen;
	int _connectedDens;
public:

	CatDen();

	~CatDen() { dispose(); }

	void dispose();

	bool init(int x, float ang, Vec2 scale, int level, Color4 color, int connectedDens, int rows, int columns, std::shared_ptr<Texture> texture,
		std::shared_ptr<Texture> greenTexture, std::shared_ptr<Texture> purpleTexture, std::shared_ptr<Texture> blueTexture, std::shared_ptr<Texture> greyTexture);

	static std::shared_ptr<CatDen> alloc(int x, float ang, Vec2 scale, int level, Color4 color, int connectedDens, int rows, int columns, std::shared_ptr<Texture> texture,
		std::shared_ptr<Texture> greenTexture, std::shared_ptr<Texture> purpleTexture, std::shared_ptr<Texture> blueTexture, std::shared_ptr<Texture> greyTexture) {
		std::shared_ptr<CatDen> result = std::make_shared<CatDen>();
		return (result->init(x, ang, scale, level, color, connectedDens, rows, columns, texture, greenTexture, purpleTexture, blueTexture, greyTexture) ? result : nullptr);
	}

	void setIsOpen(bool open) {
		_isOpen = open;
	}

	bool getIsOpen() {
		return _isOpen;
	}

	void setDoor(bool open);

	void doorOpenAnimation();
	
	int getConnectedDens() {
		return _connectedDens;
	}
	void setCurrentDoor(int connectedDens) {
		_connectedDens = connectedDens;
	}
};

#endif/* __CATDEN_H__ */