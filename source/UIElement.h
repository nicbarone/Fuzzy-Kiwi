#ifndef __UI_ELEMENT_H__
#define __UI_ELEMENT_H__
#include <cugl/cugl.h>
#include "Entity.h"

/** How far we can click outside of the button but still be recognized as clicking on it */
#define BUTTON_TOUCH_ERROR       10.0f
namespace ui {
#pragma button
	/** Enumeration identifying a button for its current state */
	enum class ButtonState {
		/** The button is now a possess button */
		POSSESS,
		/** The button is now a free button */
		UNPOSSESS,
		/** The button is now a available for clicks */
		AVAILABLE,
		/** The button is now a unavailable for clicks */
		UNAVAILABLE
	};
	class ButtonElement : public Entity {
	private:
		ButtonState _buttonState;
		std::shared_ptr<cugl::scene2::Button> _button;
	public:
		
		ButtonElement();
		~ButtonElement() { dispose(); }
		void dispose();
		void init(float x, float y, float width, float height, ButtonState buttonState);
		void set_texture(const std::shared_ptr<Texture> texture);
		ButtonState getButtonState() {
			return _buttonState;
		}
		void setButtonState(ButtonState buttonState) {
			_buttonState = buttonState;
		}
		std::shared_ptr<cugl::scene2::Button> getButton() {
			return _button;
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