#include "LevelSelectMode.h"

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
bool LevelSelectMode::init(const std::shared_ptr<AssetManager>& assets) {
    _locationIndex = -1;
    _levelID = "";
    _backButtonPressed = false;
    _locationSelected = false;
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
void LevelSelectMode::update(float progress) {
    CULog("You are now in Level Select Mode. Enjoy your stay");
}

GameplayMode LevelSelectMode::getGameScene() {
    if (_gameplay.init(_assets)) {
        return _gameplay;
    }
}

void LevelSelectMode::buildScene() {
    Size  size = Application::get()->getDisplaySize();
    float scale = GAME_WIDTH / size.width;
    size *= scale;
    _mapPanel = ui::PanelElement::alloc(size.width / 2, size.height / 2, 0, _assets->get<Texture>("mapBackground"));
    _mapPanel->createChildButton(-200,-100,20,20,ui::ButtonState::AVAILABLE, _assets->get<Texture>("smallLab"));
    _mapPanel->getChildButtons()[0]->getButton()->setScale(0.3f);
    _mapPanel->getChildButtons()[0]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _locationIndex = 0;
            _locationSelected = true;
        }
    });
    _levelSelectPanel = ui::PanelElement::alloc(size.width / 2, size.height / 2, 0, _assets->get<Texture>("levelSelectBG"));
    _levelSelectPanel->createChildButton(0, 0, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level1"));
    _levelSelectPanel->getChildButtons()[0]->getButton()->setScale(1.0f);
    _levelSelectPanel->getChildButtons()[0]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _levelID = to_string(_locationIndex) + "_0";
            _levelSelected = true;
        }
    });
    _levelSelectPanel->setVisible(false);
    // Common outside buttons
    _settingsButton = ui::ButtonElement::alloc(0, 0, 0, 0, ui::ButtonState::AVAILABLE);
    _settingsButton->setTexture(_assets->get<Texture>("settingsButton"));
    _settingsButton->getButton()->setName("settingLevelSelectButton");
    _settingsButton->getButton()->setScale(0.8f);
    _settingsButton->getButton()->setAnchor(Vec2::ANCHOR_CENTER);
    _settingsButton->setPos(Vec2(60, size.height - 60));
    _backButton = ui::ButtonElement::alloc(0, 0, 0, 0, ui::ButtonState::AVAILABLE);
    _backButton->setTexture(_assets->get<Texture>("backButton"));
    _backButton->getButton()->setName("backButton");
    _backButton->getButton()->setScale(0.3f);
    _backButton->getButton()->setAnchor(Vec2::ANCHOR_CENTER);
    _backButton->setPos(Vec2(size.width - 100, 60));
    _backButton->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _backButtonPressed = true;
        }
    });
    addChild(_mapPanel->getSceneNode());
    addChild(_levelSelectPanel->getSceneNode());
    addChild(_settingsButton->getButton());
    addChild(_backButton->getButton());
    activateButtons();
}

void LevelSelectMode::deactivateButtons() {
    _mapPanel->getChildButtons()[0]->getButton()->deactivate();
    _settingsButton->getButton()->deactivate();
    _backButton->getButton()->deactivate();
}

void LevelSelectMode::activateButtons() {
    _mapPanel->getChildButtons()[0]->getButton()->activate();
    _settingsButton->getButton()->activate();
    _backButton->getButton()->activate();
}

void LevelSelectMode::deactivateLevelSelectButtons() {
    _levelSelectPanel->setVisible(false);
    _levelSelectPanel->getChildButtons()[0]->getButton()->deactivate();
}

void LevelSelectMode::activateLevelSelectButtons() {
    _levelSelectPanel->setVisible(true);
    _levelSelectPanel->getChildButtons()[0]->getButton()->activate();
}
