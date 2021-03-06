#ifndef __UI_NODE_H__
#define __UI_NODE_H__
#include <cugl/cugl.h>
#include "Entity.h"

namespace ui_node {
	class ButtonNode : public cugl::scene2::PolygonNode {
		ButtonNode() : cugl::scene2::PolygonNode() {}

		~ButtonNode() { dispose(); }
	};
}

#endif /* __UI_NODE_H__ */