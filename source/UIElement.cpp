#include "UIElement.h"

using namespace cugl;

ui::ButtonElement::ButtonElement()
{
	Entity::setPos(0);
	setSize(Vec2::ZERO);
	_clicked = false;
	_button = nullptr;
	_buttonState = ButtonState::UNAVAILABLE;
}

void ui::ButtonElement::dispose()
{
	Entity::setPos(0);
	setSize(Vec2::ZERO);
	_button = nullptr;
	_buttonState = ButtonState::UNAVAILABLE;
}

bool ui::ButtonElement::init(float x, float y, float width, float height, ButtonState buttonState)
{
	Entity::setPos(x);
	_pos_y = y;
	setSize(Vec2(width, height));
	_buttonState = buttonState;
	return true;
}

void ui::ButtonElement::setPos(Vec2 pos)
{
	Entity::setPos(pos.x);
	_pos_y = pos.y;
	_button->setPosition(pos);
}

void ui::ButtonElement::setTexture(const std::shared_ptr<Texture> texture)
{
	// Initialize button scene graph
	// Only temp node
	std::shared_ptr<cugl::scene2::PolygonNode> buttonUpNode = cugl::scene2::PolygonNode::alloc((Rect(Vec2(getPos(), getY()), getSize())));
	buttonUpNode->setTexture(texture);
	Color4 downColor = Color4f::CORNFLOWER;
	_button = cugl::scene2::Button::alloc(scene2::PolygonNode::allocWithTexture(texture), downColor);
	_button->setPosition(getPos(), _pos_y);
}

void ui::ButtonElement::setTexture(const std::shared_ptr<Texture> texture, Color4f down)
{
	// Initialize button scene graph
	// Only temp node
	std::shared_ptr<cugl::scene2::PolygonNode> buttonUpNode = cugl::scene2::PolygonNode::alloc((Rect(Vec2(getPos(), getY()), getSize())));
	buttonUpNode->setTexture(texture);
	_button = cugl::scene2::Button::alloc(scene2::PolygonNode::allocWithTexture(texture), down);
	_button->setPosition(getPos(), _pos_y);
}

ui::PanelElement::PanelElement():
_frame(0),
_frameCounter(1)
{
	Entity::setPos(0);
	setSize(Vec2::ZERO);
	_sceneNode = nullptr;
	_texture = nullptr;
	_visible = false;
}

void ui::PanelElement::dispose()
{
	Entity::setPos(0);
	setSize(Vec2::ZERO);
	_sceneNode = nullptr;
	_texture = nullptr;
	_visible = false;
}

bool ui::PanelElement::init(float x, float y, float ang, std::shared_ptr<Texture> panelTexture)

{
	Entity::setPos(x);
	Entity::setAngle(ang);
	_texture = panelTexture;
	if (_texture == nullptr) {
		_sceneNode = scene2::SceneNode::alloc();
	}
	else {
		_sceneNode = scene2::AnimationNode::alloc(_texture, 1, 1);
	}
	//_sceneNode->setScale(0.15, 0.15);
	_sceneNode->setAnchor(Vec2::ANCHOR_CENTER);
	_sceneNode->setPosition(x,y);
	_visible = true;
	return true;
}

bool ui::PanelElement::createChildButton(float x, float y, float width, float height, ButtonState buttonState, std::shared_ptr<Texture> panelTexture, std::string name) {
	std::shared_ptr<ButtonElement> result = std::make_shared<ButtonElement>();
	if (result->init(0, 0, width, height, buttonState)) {
		result->setTexture(panelTexture);
		_sceneNode->addChildWithName(result->getButton(),name);
		_childButtons[name] = result;
		result->getButton()->setAnchor(Vec2::ANCHOR_CENTER);
		result->getButton()->setPosition(_sceneNode->getContentWidth() / 2 + x, _sceneNode->getContentHeight() / 2 + y);
		return true;
	}
	else {
		return false;
	}
}

bool ui::PanelElement::createChildButton(float x, float y, float width, float height, ButtonState buttonState, std::shared_ptr<Texture> panelTexture, Color4f down, std::string name) {
	std::shared_ptr<ButtonElement> result = std::make_shared<ButtonElement>();
	if (result->init(0, 0, width, height, buttonState)) {
		result->setTexture(panelTexture, down);
		_sceneNode->addChildWithName(result->getButton(), name);
		_childButtons[name] = result;
		result->getButton()->setAnchor(Vec2::ANCHOR_CENTER);
		result->getButton()->setPosition(_sceneNode->getContentWidth() / 2 + x, _sceneNode->getContentHeight() / 2 + y);
		return true;
	}
	else {
		return false;
	}
}

bool ui::PanelElement::createChildButtonTextureWithName(float x, float y, float width, float height, ButtonState buttonState, std::shared_ptr<Texture> panelTexture, std::string name) {
	std::shared_ptr<ButtonElement> result = std::make_shared<ButtonElement>();
	if (result->init(0, 0, width, height, buttonState)) {
		result->setTexture(panelTexture);
		_sceneNode->removeChildByName(name);
		_sceneNode->addChildWithName(result->getButton(), name);
		_childButtons[name] = result;
		result->getButton()->setAnchor(Vec2::ANCHOR_CENTER);
		result->getButton()->setPosition(_sceneNode->getContentWidth() / 2 + x, _sceneNode->getContentHeight() / 2 + y);
		return true;
	}
	else {
		return false;
	}
}

bool ui::PanelElement::createChildPanel(float x, int y, float ang, const std::shared_ptr<Texture> panelTexture) {
	std::shared_ptr<PanelElement> result = std::make_shared<PanelElement>();
	if (result->init(_sceneNode->getContentWidth() / 2 + x, _sceneNode->getContentHeight() / 2 + y, ang, panelTexture)) {
		_sceneNode->addChild(result->getSceneNode());
		_childPanels.push_back(result);
		return true;
	}
	else {
		return false;
	}
}

bool ui::PanelElement::createChildText(float x, float y, float width, float height, std::string message, const std::shared_ptr<Font> font) {
	std::shared_ptr<TextElement> result = std::make_shared<TextElement>();
	if (result->init(0, 0, width, height, message, font)) {
		_sceneNode->addChild(result->getLabel());
		_childTexts.push_back(result);
		result->getLabel()->setAnchor(Vec2::ANCHOR_CENTER);
		result->getLabel()->setPosition(_sceneNode->getContentWidth() / 2 + 8 + x, _sceneNode->getContentHeight() / 2 - 5 + y);
		return true;
	}
	else {
		return false;
	}
}

void ui::PanelElement::setPos(Vec2 pos)
{
	Entity::setPos(pos.x);
	_sceneNode->setPosition(pos);
}

ui::TextElement::TextElement()
{
	Entity::setPos(0);
	setSize(Vec2::ZERO);
	_text = nullptr;
}

void ui::TextElement::dispose()
{
	Entity::setPos(0);
	setSize(Vec2::ZERO);
	_text = nullptr;
}

bool ui::TextElement::init(float x, float y, float width, float height, std::string message, std::shared_ptr<Font> font)
{
	Entity::setPos(x);
	setSize(Vec2(width, height));
	_text = scene2::Label::alloc(message, font);
	return true;
}