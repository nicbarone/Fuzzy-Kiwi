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
    
}

GameplayMode MenuMode::getGameScene(std::string id, std::shared_ptr<InputManager> inputManager) {
    if (id == "0_0") {
        if (_gameplay.init(_assets, 0, 0, inputManager)) {
            _gameplay.clearJson();
          return _gameplay;
        }
    }
    else if (id == "0_1") {
        shared_ptr<JsonReader> reader = JsonReader::allocWithAsset("levels\\level2.json");
        _gameplay.init(_assets, 0, 1, reader->readJson(), inputManager);
        return _gameplay;
    }
    else {
        throw;
        return _gameplay;
    }
    return _gameplay;
}

void MenuMode::buildScene() {
    std::shared_ptr<Sound> menuBGM = _assets->get<Sound>("menuBGM");
    //AudioEngine::get()->play("menuBGM", menuBGM, true, 1.0f, false);
    std::shared_ptr<AudioQueue> audioQueue = AudioEngine::get()->getMusicQueue();
    audioQueue->play(menuBGM, true, 1.0f);
    //bool success = AudioEngine::get()->play("menuBGM", _assets->get<Sound>("menuBGM"));
    //AudioEngine::get()->setVolume("menuBGM",1.0f);
    //CULog("successful? %i", success?1:0);
    Size  size = Application::get()->getDisplaySize();
    float scale = GAME_WIDTH / size.width;
    size *= scale;

    menuPanel = _assets->get<Texture>("menuBackground");
    _menuPanel = ui::PanelElement::alloc(size.width / 2, size.height / 2, 0, menuPanel);
    _menuPanel->createChildPanel(420, 180, 0, _assets->get<Texture>("teamIcon"));
    _menuPanel->getChildPanels()[0]->getSceneNode()->setScale(0.2f);
    _menuPanel->createChildButton(0, -160, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("playButton"), Color4f::WHITE);
    _menuPanel->getChildButtons()[0]->getButton()->setName("playButton");
    _menuPanel->getChildButtons()[0]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _playPressed = true;
        }
    });
    _menuPanel->getChildButtons()[0]->getButton()->activate();
    _menuPanel->createChildButton(-450, 220, 25, 25, ui::ButtonState::AVAILABLE, _assets->get<Texture>("settingsButton"), Color4f::WHITE);
    _menuPanel->getChildButtons()[1]->getButton()->setName("settingsButton");
    _menuPanel->getChildButtons()[1]->getButton()->setScale(0.8f);
    _menuPanel->getChildButtons()[1]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            CULog("Clicking on settings button!");
            // Mark this button as clicked, proper handle will take place in update()
        }
    });
    _menuPanel->getChildButtons()[1]->getButton()->activate();
    addChild(_menuPanel->getSceneNode());
}

void MenuMode::deactivateButtons() {
    _menuPanel->getChildButtons()[0]->getButton()->deactivate();
    _menuPanel->getChildButtons()[1]->getButton()->deactivate();
}

void MenuMode::activateButtons() {
    _menuPanel->getChildButtons()[0]->getButton()->activate();
    _menuPanel->getChildButtons()[1]->getButton()->activate();
}

