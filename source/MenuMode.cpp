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
        _menuPanel->getChildButtons()[1]->getButton()->setVisible(true);
        _menuPanel->getChildButtons()[1]->getButton()->activate();
    }
    else {
        _menuPanel->getChildButtons()[1]->getButton()->setVisible(false);
        _menuPanel->getChildButtons()[1]->getButton()->deactivate();
    }
}

GameplayMode MenuMode::getGameScene(std::string id, std::shared_ptr<InputManager> inputManager) {
    if (id == "0") {
        shared_ptr<JsonReader> reader = JsonReader::allocWithAsset("levels\\level1.json");
        _gameplay.init(_assets, 0, reader->readJson(), inputManager);
        _gameplay.setShowTutorial(true);
        return _gameplay;
    }
    else if (id == "1") {
        shared_ptr<JsonReader> reader = JsonReader::allocWithAsset("levels\\level2.json");
        _gameplay.init(_assets, 1, reader->readJson(), inputManager);
        return _gameplay;
    }
    else if (id == "2") {
        shared_ptr<JsonReader> reader = JsonReader::allocWithAsset("levels\\level3.json");
        _gameplay.init(_assets, 2, reader->readJson(), inputManager);
        return _gameplay;
    }
    else if (id == "3") {
        shared_ptr<JsonReader> reader = JsonReader::allocWithAsset("levels\\level4.json");
        _gameplay.init(_assets, 3, reader->readJson(), inputManager);
        return _gameplay;
    }
    //else if (id == "4") {
    else {
        shared_ptr<JsonReader> reader = JsonReader::allocWithAsset("levels\\level5.json");
        _gameplay.init(_assets, 4, reader->readJson(), inputManager);
        return _gameplay;
    }
    /*else {
        throw;
        return _gameplay;
    }*/
    return _gameplay;
}

void MenuMode::buildScene() {
    std::shared_ptr<Sound> menuBGM = _assets->get<Sound>("menuBGM");
    std::shared_ptr<AudioQueue> audioQueue = AudioEngine::get()->getMusicQueue();
    //audioQueue->play(menuBGM, true, 1.0f); //This line of code is the devil

    Size  size = Application::get()->getDisplaySize();
    float scale = GAME_WIDTH / size.width;
    size *= scale;

    menuPanel = _assets->get<Texture>("menuBackground");
    _menuPanel = ui::PanelElement::alloc(size.width / 2, size.height / 2, 0, menuPanel);
    _menuPanel->getSceneNode()->setScale(max(size.width / menuPanel->getSize().width, size.height / menuPanel->getSize().height));
    _menuPanel->createChildButton(1000, 325, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("playButton"), Color4f::WHITE);
    _menuPanel->getChildButtons()[0]->getButton()->setName("playButton");
    _menuPanel->getChildButtons()[0]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            _playPressed = true;
        }
    });
    _menuPanel->getChildButtons()[0]->getButton()->activate();
    _menuPanel->createChildButton(950, -700, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("loadGame"), Color4f::WHITE);
    _menuPanel->getChildButtons()[1]->getButton()->setScale(Vec2(1.2f, 1.2f));
    _menuPanel->getChildButtons()[1]->getButton()->setName("loadButton");
    _menuPanel->getChildButtons()[1]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            _gameLoaded = true;
        }
        });
    _menuPanel->getChildButtons()[1]->getButton()->setVisible(false);
    _menuPanel->getChildButtons()[1]->getButton()->activate();
    addChild(_menuPanel->getSceneNode());
}

void MenuMode::deactivateButtons() {
    _menuPanel->getChildButtons()[0]->getButton()->deactivate();
}

void MenuMode::activateButtons() {
    _menuPanel->getChildButtons()[0]->getButton()->activate();
}
