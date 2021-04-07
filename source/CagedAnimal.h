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
	/*list of keys that the door.*/
	std::vector<int> _keys;
public:

	CagedAnimal();

	~CagedAnimal() { dispose(); }

	void dispose();

	bool init(int x, float ang, Vec2 scale, int level, Color4 color, std::vector<int> keys, int rows, int columns,
		std::shared_ptr<Texture> texture);

	static std::shared_ptr<CagedAnimal> alloc(int x, float ang, Vec2 scale, int level, Color4 color, std::vector<int> keys, int rows, int columns,
		std::shared_ptr<Texture> texture) {
		std::shared_ptr<CagedAnimal> result = std::make_shared<CagedAnimal>();
		return (result->init(x, ang, scale, level, color, keys, rows, columns, texture) ? result : nullptr);
	}
};

#endif/* __CAGEDANIMAL_H__ */