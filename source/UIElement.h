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
		float _pos_y;
		bool _clicked;
		ButtonState _buttonState;
		std::shared_ptr<cugl::scene2::Button> _button;
	public:
		
		ButtonElement();
		~ButtonElement() { dispose(); }
		void dispose();
		static std::shared_ptr<ButtonElement> alloc(float x, float y, float width, float height, ButtonState buttonState) {
			std::shared_ptr<ButtonElement> result = std::make_shared<ButtonElement>();
			return (result->init(x, y, width, height, buttonState) ? result : nullptr);
		}
		bool init(float x, float y, float width, float height, ButtonState buttonState);
		void setPos(Vec2 pos);
		float getY() { return _pos_y; }
		void setClicked(bool click) {
			_clicked = click;
		}
		bool getClicked() {
			return _clicked;
		}
		void setTexture(const std::shared_ptr<Texture> texture);
		void setTexture(const std::shared_ptr<Texture> texture, Color4f down);

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
#pragma text
	class TextElement : public Entity {
	private:
		std::shared_ptr<scene2::Label> _text;
	public:
		TextElement();
		~TextElement() { dispose(); }

		void dispose();
		static std::shared_ptr<TextElement> alloc(float x, float y, float width, float height, std::string message, std::shared_ptr<Font> font) {
			std::shared_ptr<TextElement> result = std::make_shared<TextElement>();
			return (result->init(x, y, width, height, message, font) ? result : nullptr);
		}
		bool init(float x, float y, float width, float height, std::string message, std::shared_ptr<Font> font);
		void setPos(Vec2 pos) {
			_text->setPosition(pos);
		}
		void setScale(Vec2 scale) {
			_text->setScale(scale);
		}
		void setColor(Color4f color) {
			_text->setColor(color);
		}
		Color4f getColor() {
			return _text->getColor();
		}
		std::shared_ptr<scene2::Label> getLabel() {
			return _text;
		}
		void setText(std::string text, bool resize = false) {
			_text->setText(text, resize);
		}
		std::string getText() {
			return _text->getText();
		}
	};
#pragma panel
	class PanelElement : public Entity {
	private:
		std::shared_ptr<scene2::AnimationNode> _sceneNode;
		std::shared_ptr<Texture> _texture;
		std::vector<std::shared_ptr<ButtonElement>> _childButtons;
		std::vector<std::shared_ptr<PanelElement>> _childPanels;
		std::vector<std::shared_ptr<TextElement>> _childTexts;
		bool _visible;
		int _frame;
		int _frameCounter;

	public:
		PanelElement();

		~PanelElement() { dispose(); }

		void dispose();

		bool init(float x, float y, float ang, const std::shared_ptr<Texture> panelTexture);

		static std::shared_ptr<PanelElement> alloc(float x, int y, float ang, const std::shared_ptr<Texture> panelTexture) {
			std::shared_ptr<PanelElement> result = std::make_shared<PanelElement>();
			return (result->init(x, y, ang, panelTexture) ? result : nullptr);
		}
		/** returns the AnimationNode associated with the panel*/
		std::shared_ptr<scene2::AnimationNode> getSceneNode() {
			return _sceneNode;
		}

		bool createChildButton(float x, float y, float width, float height, ButtonState buttonState, std::shared_ptr<Texture> panelTexture);

		bool createChildButton(float x, float y, float width, float height, ButtonState buttonState, std::shared_ptr<Texture> panelTexture, Color4f down);


		bool createChildPanel(float x, int y, float ang, const std::shared_ptr<Texture> panelTexture);
		
		bool createChildText(float x, float y, float width, float height, std::string message, const std::shared_ptr<Font> font);

		std::vector<std::shared_ptr<ButtonElement>> getChildButtons() {
			return _childButtons;
		}

		std::vector<std::shared_ptr<PanelElement>> getChildPanels() {
			return _childPanels;
		}

		std::vector<std::shared_ptr<ui::TextElement>> getChildTexts() {
			return _childTexts;
		}

		void setPos(Vec2 pos);
		void setVisible(bool visibility) {
			_visible = visibility;
			if (_visible) {
				_sceneNode->setColor(Color4f::WHITE);
			}
			else {
				_sceneNode->setColor(Color4f::CLEAR);
			}
		}
		bool getVisible() {
			return _visible;
		}
	};

};

#endif /* __UI_ELEMENT_H__ */