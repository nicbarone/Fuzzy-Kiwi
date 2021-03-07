#include "UIElement.h"

using namespace cugl;

ui::ButtonElement::ButtonElement()
{
	set_pos(Vec2::ZERO);
	set_size(Vec2::ZERO);
	_button = nullptr;
	_buttonState = ButtonState::UNAVAILABLE;
}

void ui::ButtonElement::dispose()
{
	set_pos(Vec2::ZERO);
	set_size(Vec2::ZERO);
	_button = nullptr;
	_buttonState = ButtonState::UNAVAILABLE;
}

void ui::ButtonElement::init(float x, float y, float width, float height, ButtonState buttonState)
{
	set_pos(Vec2(x, y));
	set_size(Vec2(width, height));
	_buttonState = buttonState;
}

void ui::ButtonElement::set_texture(const std::shared_ptr<Texture> texture)
{
	// Initialize button scene graph
	// Only temp node
	std::shared_ptr<cugl::scene2::PolygonNode> buttonUpNode = cugl::scene2::PolygonNode::alloc((Rect(get_pos(), get_size())));
	buttonUpNode->setTexture(texture);
	Color4 downColor = Color4f::CORNFLOWER;
    _button = cugl::scene2::Button::alloc(buttonUpNode, downColor);
	
}
