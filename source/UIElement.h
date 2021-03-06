#ifndef __UI_ELEMENT_H__
#define __UI_ELEMENT_H__
#include <cugl/cugl.h>
#include "Entity.h"
#include "UINode.h"

namespace ui {
#pragma button
	class ButtonElement : public Entity {
	private:
		bool _clicked;
		bool _available;
		std::shared_ptr<ui_node::ButtonNode> _buttonNode;
	public:
		ButtonElement();
		~ButtonElement() { dispose(); }
		void dispose();
		void init(float x, float y, float width, float height);
		void set_texture(const std::shared_ptr<Texture> texture);
		bool isAvailable() {
			return _available;
		}
		void setAvailable(bool available) {
			_available = available;
		}
		bool getClicked() {
			return _clicked;
		}
		std::shared_ptr<ui_node::ButtonNode> getButtonNode() {
			return _buttonNode;
		}
		void clickButton() {
			_clicked = true;
		}
		void unClickButton() {
			_clicked = false;
		}
	};

#pragma panel
	class PanelElement : public Entity {

	};

#pragma text
	class TextElement : public Entity {
		std::string text;
	};

};

#endif /* __UI_ELEMENT_H__ */