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
    _levelID = "";
    _backButtonPressed = false;
    _levelSelected = false;
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
    //CULog("You are now in Level Select Mode. Enjoy your stay");
}

void LevelSelectMode::updateLevelIcon() {
    std::shared_ptr<JsonReader> reader = JsonReader::alloc(Application::get()->getSaveDirectory() + "completed_levels.json");
    if (reader == nullptr) {
        // if file doesn't exist create a new one
        shared_ptr<JsonValue> result = JsonValue::allocObject();
        shared_ptr<JsonValue> r_complete = JsonValue::allocObject();
        for (int i = 0; i < 10; i++) {
            r_complete->appendValue("level" + to_string(i + 1), false);
        }
        result->appendChild("completed", r_complete);
        shared_ptr<JsonWriter> writer = JsonWriter::alloc(Application::get()->getSaveDirectory() + "completed_levels.json");
        writer->writeJson(result);
        writer->close();
        reader = JsonReader::alloc(Application::get()->getSaveDirectory() + "completed_levels.json");
    }
    std::shared_ptr<JsonValue> json = reader->readJson();
    shared_ptr<JsonValue> completed = json->get("completed");
    if (completed->getBool("level1")) {
        _levelSelectPanel->getChildButtons()[0]->getButton()->setColor(Color4::GREEN);
    }
    if (completed->getBool("level2")) {
        _levelSelectPanel->getChildButtons()[1]->getButton()->setColor(Color4::GREEN);
    }
    if (completed->getBool("level3")) {
        _levelSelectPanel->getChildButtons()[2]->getButton()->setColor(Color4::GREEN);
    }
    if (completed->getBool("level4")) {
        _levelSelectPanel->getChildButtons()[3]->getButton()->setColor(Color4::GREEN);
    }
    if (completed->getBool("level5")) {
        _levelSelectPanel->getChildButtons()[4]->getButton()->setColor(Color4::GREEN);
    }
    if (completed->getBool("level6")) {
        _levelSelectPanel->getChildButtons()[5]->getButton()->setColor(Color4::GREEN);
    }
    if (completed->getBool("level7")) {
        _levelSelectPanel->getChildButtons()[6]->getButton()->setColor(Color4::GREEN);
    }
    if (completed->getBool("level8")) {
        _levelSelectPanel->getChildButtons()[7]->getButton()->setColor(Color4::GREEN);
    }
    if (completed->getBool("level9")) {
        _levelSelectPanel->getChildButtons()[8]->getButton()->setColor(Color4::GREEN);
    }
    if (completed->getBool("level10")) {
        _levelSelectPanel->getChildButtons()[9]->getButton()->setColor(Color4::GREEN);
    }
    reader->close();
}

void LevelSelectMode::buildScene() {
    Size  size = Application::get()->getDisplaySize();
    float scale = GAME_WIDTH / size.width;
    size *= scale;
    
    _levelSelectPanel = ui::PanelElement::alloc(size.width / 2, size.height / 2, 0, _assets->get<Texture>("levelSelectBG"));
    _levelSelectPanel->getSceneNode()->setScale(min(size.width / _assets->get<Texture>("levelSelectBG")->getSize().width, size.height / _assets->get<Texture>("levelSelectBG")->getSize().height));
    _levelSelectPanel->createChildButton(-400, 40, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level1"));
    _levelSelectPanel->getChildButtons()[0]->getButton()->setScale(1.0f);
    _levelSelectPanel->getChildButtons()[0]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _levelID = "0";
            _levelSelected = true;
        }
    });
    _levelSelectPanel->createChildButton(-220, 40, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level2"));
    _levelSelectPanel->getChildButtons()[1]->getButton()->setScale(1.0f);
    _levelSelectPanel->getChildButtons()[1]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _levelID = "1";
            _levelSelected = true;
        }
        });
    _levelSelectPanel->createChildButton(-50, 42, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level3"));
    _levelSelectPanel->getChildButtons()[2]->getButton()->setScale(1.0f);
    _levelSelectPanel->getChildButtons()[2]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _levelID = "2";
            _levelSelected = true;
        }
        });
    _levelSelectPanel->createChildButton(110, 50, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level4"));
    _levelSelectPanel->getChildButtons()[3]->getButton()->setScale(1.0f);
    _levelSelectPanel->getChildButtons()[3]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _levelID = "3";
            _levelSelected = true;
        }
        });
    _levelSelectPanel->createChildButton(260, 63, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level5"));
    _levelSelectPanel->getChildButtons()[4]->getButton()->setScale(1.0f);
    _levelSelectPanel->getChildButtons()[4]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _levelID = "4";
            _levelSelected = true;
        }
        });
    _levelSelectPanel->createChildButton(-390, -130, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level6"));
    _levelSelectPanel->getChildButtons()[5]->getButton()->setScale(1.0f);
    _levelSelectPanel->getChildButtons()[5]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _levelID = "5";
            _levelSelected = true;
        }
        });
    _levelSelectPanel->createChildButton(-230, -123, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level7"));
    _levelSelectPanel->getChildButtons()[6]->getButton()->setScale(1.0f);
    _levelSelectPanel->getChildButtons()[6]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _levelID = "6";
            _levelSelected = true;
        }
        });
    _levelSelectPanel->createChildButton(-52, -140, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level8"));
    _levelSelectPanel->getChildButtons()[7]->getButton()->setScale(1.0f);
    _levelSelectPanel->getChildButtons()[7]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _levelID = "7";
            _levelSelected = true;
        }
        });
    _levelSelectPanel->createChildButton(120, -135, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level9"));
    _levelSelectPanel->getChildButtons()[8]->getButton()->setScale(1.0f);
    _levelSelectPanel->getChildButtons()[8]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _levelID = "8";
            _levelSelected = true;
        }
        });
    _levelSelectPanel->createChildButton(285, -135, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level10"));
    _levelSelectPanel->getChildButtons()[9]->getButton()->setScale(1.0f);
    _levelSelectPanel->getChildButtons()[9]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _levelID = "9";
            _levelSelected = true;
        }
        });
    // Common outside buttons
    _backButton = ui::ButtonElement::alloc(0, 0, 0, 0, ui::ButtonState::AVAILABLE);
    _backButton->setTexture(_assets->get<Texture>("backButton"));
    _backButton->getButton()->setName("backButton");
    _backButton->getButton()->setScale(1.0f);
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
    addChild(_levelSelectPanel->getSceneNode());
    addChild(_backButton->getButton());
    activateButtons();
    updateLevelIcon();
}

void LevelSelectMode::deactivateButtons() {
    _backButton->getButton()->deactivate();
    _levelSelectPanel->getChildButtons()[0]->getButton()->deactivate();
    _levelSelectPanel->getChildButtons()[1]->getButton()->deactivate();
    _levelSelectPanel->getChildButtons()[2]->getButton()->deactivate();
    _levelSelectPanel->getChildButtons()[3]->getButton()->deactivate();
    _levelSelectPanel->getChildButtons()[4]->getButton()->deactivate();
    _levelSelectPanel->getChildButtons()[5]->getButton()->deactivate();

}

void LevelSelectMode::activateButtons() {
    _backButton->getButton()->activate();
    _levelSelectPanel->getChildButtons()[0]->getButton()->activate();
    _levelSelectPanel->getChildButtons()[1]->getButton()->activate();
    _levelSelectPanel->getChildButtons()[2]->getButton()->activate();
    _levelSelectPanel->getChildButtons()[3]->getButton()->activate();
    _levelSelectPanel->getChildButtons()[4]->getButton()->activate();
    _levelSelectPanel->getChildButtons()[5]->getButton()->activate();
}
