#include "MenuMode.h"

using namespace cugl;
#define SCENE_SIZE  1024


/**
 * Initializes the controller contents, making it ready for loading
 *
 * The constructor does not allocate any objects or memory.  This allows
 * us to have a non-pointer reference to this controller, reducing our
 * memory allocation.  Instead, allocation happens in this method.
 *
 * @param assets    The (loaded) assets for this game mode
 *
 * @return true if the controller is initialized properly, false otherwise.
 */
bool MenuMode::init(const std::shared_ptr<AssetManager>& assets) {
    _playPressed = false;
    _gameLoaded = false;
    // Initialize the scene to a locked width
    Size dimen = Application::get()->getDisplaySize();
    // Lock the scene to a reasonable resolution
    if (dimen.width > dimen.height) {
        dimen *= SCENE_SIZE / dimen.width;
    }
    else {
        dimen *= SCENE_SIZE / dimen.height;
    }
    if (assets == nullptr) {
        return false;
    }
    else if (!Scene2::init(dimen)) {
        return false;
    }


    _assets = assets;
    buildScene();
    return true;
}



#pragma mark -
#pragma mark Progress Monitoring
/**
 * The method called to update the game mode.
 *
 * This method updates the progress bar amount.
 *
 * @param timestep  The amount of time (in seconds) since the last frame
 */
void MenuMode::update(float progress) {
    //CULog("You are now in Menu Mode. Enjoy your stay");
    // If save file exists then show this button
    if (filetool::file_exists(Application::get()->getSaveDirectory() + "save.json")) {
        _menuPanel->getChildButtons()["loadButton"]->getButton()->setVisible(true);
        _menuPanel->getChildButtons()["loadButton"]->getButton()->activate();
    }
    else {
        _menuPanel->getChildButtons()["loadButton"]->getButton()->setVisible(false);
        _menuPanel->getChildButtons()["loadButton"]->getButton()->deactivate();
    }
    std::shared_ptr<AudioQueue> audioQueue = AudioEngine::get()->getMusicQueue();
    if (_gameMuted) {
        audioQueue->setVolume(0);
        _menuPanel->getChildButtons()["muteButton"]->getButton()->setVisible(false);
        _menuPanel->getChildButtons()["muteButton"]->getButton()->deactivate();
        _menuPanel->getChildButtons()["unmuteButton"]->getButton()->setVisible(true);
        _menuPanel->getChildButtons()["unmuteButton"]->getButton()->activate();
    }
    else {
        audioQueue->setVolume(1.0f);
        _menuPanel->getChildButtons()["unmuteButton"]->getButton()->setVisible(false);
        _menuPanel->getChildButtons()["unmuteButton"]->getButton()->deactivate();
        _menuPanel->getChildButtons()["muteButton"]->getButton()->setVisible(true);
        _menuPanel->getChildButtons()["muteButton"]->getButton()->activate();
    }
}

GameplayMode MenuMode::getGameScene(std::string id, std::shared_ptr<InputManager> inputManager, bool muted) {
    for (int i = 0; i < MAX_LEVEL_PAGE * 10; i++) {
        if (id == to_string(i)) {
            shared_ptr<JsonReader> reader = JsonReader::allocWithAsset("levels\\level" + to_string(i + 1) + ".json");
            _gameplay.init(_assets, i, reader->readJson(), inputManager, muted);
            if (i <= 4) {
                _gameplay.setShowTutorial(i+1);
            }
            return _gameplay;
        }
    }
    return _gameplay;
}

void MenuMode::buildScene() {
    std::shared_ptr<Sound> menuBGM = _assets->get<Sound>("menuBGM");
    std::shared_ptr<AudioQueue> audioQueue = AudioEngine::get()->getMusicQueue();
    audioQueue->play(menuBGM, true, 1.0f);

    Size  size = Application::get()->getDisplaySize();
    float scale = GAME_WIDTH / size.width;
    size *= scale;

    menuPanel = _assets->get<Texture>("menuBackground");
    _menuPanel = ui::PanelElement::alloc(size.width / 2, size.height / 2, 0, menuPanel);
    _menuPanel->getSceneNode()->setScale(max(size.width / menuPanel->getSize().width, size.height / menuPanel->getSize().height));
    _menuPanel->createChildButton(1000, 325, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("playButton"), Color4f::WHITE,"play");
    _menuPanel->getChildButtons()["play"]->getButton()->setName("play");
    _menuPanel->getChildButtons()["play"]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            _playPressed = true;
        }
    });
    _menuPanel->getChildButtons()["play"]->getButton()->activate();
    _menuPanel->createChildButton(950, -700, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("loadGame"), Color4f::WHITE, "loadButton");
    _menuPanel->getChildButtons()["loadButton"]->getButton()->setScale(Vec2(6.0f, 6.0f));
    _menuPanel->getChildButtons()["loadButton"]->getButton()->setName("loadButton");
    _menuPanel->getChildButtons()["loadButton"]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            _gameLoaded = true;
        }
        });
    _menuPanel->getChildButtons()["loadButton"]->getButton()->setVisible(false);
    _menuPanel->getChildButtons()["loadButton"]->getButton()->activate();
    _menuPanel->createChildButton((-size.width + 100.0f) / 2.0f / _menuPanel->getSceneNode()->getScaleX(), (size.height - 100.0f) / 2.0f / _menuPanel->getSceneNode()->getScaleY(), 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("unmute"), Color4f::WHITE, "muteButton");
    _menuPanel->getChildButtons()["muteButton"]->getButton()->setScale(Vec2(3.0f, 3.0f));
    _menuPanel->getChildButtons()["muteButton"]->getButton()->setName("muteButton");
    _menuPanel->getChildButtons()["muteButton"]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            _gameMuted = true;
        }
        });
    _menuPanel->getChildButtons()["muteButton"]->getButton()->setVisible(true);
    _menuPanel->getChildButtons()["muteButton"]->getButton()->activate();
    _menuPanel->createChildButton((-size.width + 100.0f) / 2.0f / _menuPanel->getSceneNode()->getScaleX(), (size.height - 100.0f) / 2.0f / _menuPanel->getSceneNode()->getScaleY(), 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("mute"), Color4f::WHITE, "unmuteButton");
    _menuPanel->getChildButtons()["unmuteButton"]->getButton()->setScale(Vec2(3.0f, 3.0f));
    _menuPanel->getChildButtons()["unmuteButton"]->getButton()->setName("unmuteButton");
    _menuPanel->getChildButtons()["unmuteButton"]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            _gameMuted = false;
        }
        });
    _menuPanel->getChildButtons()["unmuteButton"]->getButton()->setVisible(false);
    _menuPanel->getChildButtons()["unmuteButton"]->getButton()->deactivate();
    addChild(_menuPanel->getSceneNode());
}

void MenuMode::deactivateButtons() {
    _menuPanel->getChildButtons()["play"]->getButton()->deactivate();
}

void MenuMode::activateButtons() {
    _menuPanel->getChildButtons()["play"]->getButton()->activate();
}
