#include "UIElement.h"

using namespace cugl;

ui::ButtonElement::ButtonElement()
{
	Entity::setPos(Vec2::ZERO);
	setSize(Vec2::ZERO);
	_clicked = false;
	_button = nullptr;
	_buttonState = ButtonState::UNAVAILABLE;
}

void ui::ButtonElement::dispose()
{
	Entity::setPos(Vec2::ZERO);
	setSize(Vec2::ZERO);
	_button = nullptr;
	_buttonState = ButtonState::UNAVAILABLE;
}

bool ui::ButtonElement::init(float x, float y, float width, float height, ButtonState buttonState)
{
	Entity::setPos(Vec2(x, y));
	setSize(Vec2(width, height));
	_buttonState = buttonState;
	return true;
}

void ui::ButtonElement::setPos(Vec2 pos)
{
	Entity::setPos(pos);
	_button->setPosition(pos);
}

void ui::ButtonElement::setTexture(const std::shared_ptr<Texture> texture)
{
	// Initialize button scene graph
	// Only temp node
	std::shared_ptr<cugl::scene2::PolygonNode> buttonUpNode = cugl::scene2::PolygonNode::alloc((Rect(getPos(), getSize())));
	buttonUpNode->setTexture(texture);
	Color4 downColor = Color4f::CORNFLOWER;
	_button = cugl::scene2::Button::alloc(scene2::PolygonNode::allocWithTexture(texture), downColor);
}
