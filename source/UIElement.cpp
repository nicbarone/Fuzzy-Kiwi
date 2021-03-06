#include "UIElement.h"

using namespace cugl;

ui::ButtonElement::ButtonElement()
{
	_clicked = false;
	_available = false;
	_buttonNode = nullptr;
	set_pos(Vec2::ZERO);
}

void ui::ButtonElement::dispose()
{
	_clicked = false;
	_available = false;
	_buttonNode = nullptr;
}

void ui::ButtonElement::init(float x, float y, float width, float height)
{
	set_pos(Vec2(x, y));
	set_size(Vec2(width, height));
}

void ui::ButtonElement::set_texture(const std::shared_ptr<Texture> texture)
{
	// Initialize button scene graph
    _buttonNode = scene2::PolygonNode::alloc(Rect(get_pos(),get_size()));
	//_buttonNode->setColor(Color4f(0.5f, 0.5f, 1.0f, 1.0f));   // Color yet to decide
	_buttonNode->setTexture(texture);
}
