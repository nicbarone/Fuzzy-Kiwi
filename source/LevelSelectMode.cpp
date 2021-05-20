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
    _curPage = 0;
    _backButtonPressed = false;
    _pageChange = 0;
    _levelSelectPanel = std::vector<std::shared_ptr<ui::PanelElement>>();
    for (int i = 0; i < MAX_LEVEL_PAGE; i++) {
        _levelSelectPanel.push_back(nullptr);
    }
    _levelSelected = false;
    _toUpdateVolume = false;
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
    std::shared_ptr<AudioQueue> audioQueue = AudioEngine::get()->getMusicQueue();
    Size  size = Application::get()->getDisplaySize();
    float scale = GAME_WIDTH / size.width;
    size *= scale;
    if (_pageChange != 0) {
        updatePage();
    }
    if (_toUpdateVolume) {
        _toUpdateVolume = false;
        if (_uiPanel->getChildButtons()["muteButton"] != nullptr) {
            _uiPanel->getChildButtons()["muteButton"]->getButton()->deactivate();
            _uiPanel->getSceneNode()->removeChildByName("muteButton");
        }
        if (_uiPanel->getChildButtons()["unmuteButton"] != nullptr) {
            _uiPanel->getChildButtons()["unmuteButton"]->getButton()->deactivate();
            _uiPanel->getSceneNode()->removeChildByName("unmuteButton");
        }
        if (_gameMuted) {
            audioQueue->setVolume(0);
            _uiPanel->createChildButton((size.width - 100.0f) / 2.0f, (size.height - 100.0f) / 2.0f, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("mute"), Color4f::WHITE, "unmuteButton");
            _uiPanel->getChildButtons()["unmuteButton"]->getButton()->setScale(Vec2(0.8f, 0.8f));
            _uiPanel->getChildButtons()["unmuteButton"]->getButton()->setName("unmuteButton");
            _uiPanel->getChildButtons()["unmuteButton"]->getButton()->addListener([=](const std::string& name, bool down) {
                // Only quit when the button is released
                if (!down) {
                    _gameMuted = false;
                    _toUpdateVolume = true;
                }
                });
            _uiPanel->getChildButtons()["unmuteButton"]->getButton()->setVisible(true);
            _uiPanel->getChildButtons()["unmuteButton"]->getButton()->activate();
        }
        else {
            audioQueue->setVolume(1.0f);
            _uiPanel->createChildButton((size.width - 100.0f) / 2.0f, (size.height - 100.0f) / 2.0f, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("unmute"), Color4f::WHITE, "muteButton");
            _uiPanel->getChildButtons()["muteButton"]->getButton()->setScale(Vec2(0.8f, 0.8f));
            _uiPanel->getChildButtons()["muteButton"]->getButton()->setName("muteButton");
            _uiPanel->getChildButtons()["muteButton"]->getButton()->addListener([=](const std::string& name, bool down) {
                // Only quit when the button is released
                if (!down) {
                    _gameMuted = true;
                    _toUpdateVolume = true;
                }
                });
            _uiPanel->getChildButtons()["muteButton"]->getButton()->setVisible(true);
            _uiPanel->getChildButtons()["muteButton"]->getButton()->activate();
        }
    }
}

void LevelSelectMode::updateLevelIcon() {
    std::shared_ptr<JsonReader> reader = JsonReader::alloc(Application::get()->getSaveDirectory() + "completed_levels.json");
    if (reader == nullptr) {
        // if file doesn't exist create a new one
        shared_ptr<JsonValue> result = JsonValue::allocObject();
        shared_ptr<JsonValue> r_complete = JsonValue::allocObject();
        for (int i = 0; i < 12; i++) {
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
    for (int i = 0; i < MAX_LEVEL_PAGE; i++) {
        if (completed->getBool("level" + to_string(i * 10 + 1))) {
            _levelSelectPanel[i]->createChildButtonTextureWithName(-380, 60, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level" + to_string(i * 10 + 1) + "Complete"), "level" + to_string(i * 10 + 1));
            _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 1)]->getButton()->setScale(1.0f);
            _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 1)]->getButton()->addListener([=](const std::string& name, bool down) {
                // Only quit when the button is released
                if (!down) {
                    //CULog("Clicking on possess button!");
                    // Mark this button as clicked, proper handle will take place in update()
                    _levelID = to_string(i * 10 + 0);
                    CULog("Invisible button?");
                    _levelSelected = true;
                }
                });
        }
        if (completed->getBool("level" + to_string(i * 10 + 2))) {
            _levelSelectPanel[i]->createChildButtonTextureWithName(-220, 60, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level" + to_string(i * 10 + 2) + "Complete"), "level" + to_string(i * 10 + 2));
            _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 2)]->getButton()->setScale(1.0f);
            _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 2)]->getButton()->addListener([=](const std::string& name, bool down) {
                // Only quit when the button is released
                if (!down) {
                    //CULog("Clicking on possess button!");
                    // Mark this button as clicked, proper handle will take place in update()
                    _levelID = to_string(i * 10 + 1);
                    CULog("Invisible button?");
                    _levelSelected = true;
                }
                });
        }
        if (completed->getBool("level" + to_string(i * 10 + 3))) {
            _levelSelectPanel[i]->createChildButtonTextureWithName(-55, 60, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level" + to_string(i * 10 + 3) + "Complete"), "level" + to_string(i * 10 + 3));
            _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 3)]->getButton()->setScale(1.0f);
            _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 3)]->getButton()->addListener([=](const std::string& name, bool down) {
                // Only quit when the button is released
                if (!down) {
                    //CULog("Clicking on possess button!");
                    // Mark this button as clicked, proper handle will take place in update()
                    _levelID = to_string(i * 10 + 2);
                    CULog("Invisible button?");
                    _levelSelected = true;
                }
                });
        }
        if (completed->getBool("level" + to_string(i * 10 + 4))) {
            _levelSelectPanel[i]->createChildButtonTextureWithName(105, 60, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level" + to_string(i * 10 + 4) + "Complete"), "level" + to_string(i * 10 + 4));
            _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 4)]->getButton()->setScale(1.0f);
            _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 4)]->getButton()->addListener([=](const std::string& name, bool down) {
                // Only quit when the button is released
                if (!down) {
                    //CULog("Clicking on possess button!");
                    // Mark this button as clicked, proper handle will take place in update()
                    _levelID = to_string(i * 10 + 3);
                    CULog("Invisible button?");
                    _levelSelected = true;
                }
                });
        }
        if (completed->getBool("level" + to_string(i * 10 + 5))) {
            _levelSelectPanel[i]->createChildButtonTextureWithName(265, 55, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level" + to_string(i * 10 + 5) + "Complete"), "level" + to_string(i * 10 + 5));
            _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 5)]->getButton()->setScale(1.0f);
            _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 5)]->getButton()->addListener([=](const std::string& name, bool down) {
                // Only quit when the button is released
                if (!down) {
                    //CULog("Clicking on possess button!");
                    // Mark this button as clicked, proper handle will take place in update()
                    _levelID = to_string(i * 10 + 4);
                    CULog("Invisible button?");
                    _levelSelected = true;
                }
                });
        }
        if (completed->getBool("level" + to_string(i * 10 + 6))) {
            _levelSelectPanel[i]->createChildButtonTextureWithName(-380, -110, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level" + to_string(i * 10 + 6) + "Complete"), "level" + to_string(i * 10 + 6));
            _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 6)]->getButton()->setScale(1.0f);
            _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 6)]->getButton()->addListener([=](const std::string& name, bool down) {
                // Only quit when the button is released
                if (!down) {
                    //CULog("Clicking on possess button!");
                    // Mark this button as clicked, proper handle will take place in update()
                    _levelID = to_string(i * 10 + 5);
                    CULog("Invisible button?");
                    _levelSelected = true;
                }
                });
        }
        if (completed->getBool("level" + to_string(i * 10 + 7))) {
            _levelSelectPanel[i]->createChildButtonTextureWithName(-220, -110, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level" + to_string(i * 10 + 7) + "Complete"), "level" + to_string(i * 10 + 7));
            _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 7)]->getButton()->setScale(1.0f);
            _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 7)]->getButton()->addListener([=](const std::string& name, bool down) {
                // Only quit when the button is released
                if (!down) {
                    //CULog("Clicking on possess button!");
                    // Mark this button as clicked, proper handle will take place in update()
                    _levelID = to_string(i * 10 + 6);
                    CULog("Invisible button?");
                    _levelSelected = true;
                }
                });
        }
        if (completed->getBool("level" + to_string(i * 10 + 8))) {
            _levelSelectPanel[i]->createChildButtonTextureWithName(-55, -115, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level" + to_string(i * 10 + 8) + "Complete"), "level" + to_string(i * 10 + 8));
            _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 8)]->getButton()->setScale(1.0f);
            _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 8)]->getButton()->addListener([=](const std::string& name, bool down) {
                // Only quit when the button is released
                if (!down) {
                    //CULog("Clicking on possess button!");
                    // Mark this button as clicked, proper handle will take place in update()
                    _levelID = to_string(i * 10 + 7);
                    CULog("Invisible button?");
                    _levelSelected = true;
                }
                });
        }
        if (completed->getBool("level" + to_string(i * 10 + 9))) {
            _levelSelectPanel[i]->createChildButtonTextureWithName(110, -115, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level"+ to_string(i * 10 + 9) +"Complete"), "level" + to_string(i * 10 + 9));
            _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 9)]->getButton()->setScale(1.0f);
            _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 9)]->getButton()->addListener([=](const std::string& name, bool down) {
                // Only quit when the button is released
                if (!down) {
                    //CULog("Clicking on possess button!");
                    // Mark this button as clicked, proper handle will take place in update()
                    _levelID = to_string(i * 10 + 8);
                    CULog("Invisible button?");
                    _levelSelected = true;
                }
                });
        }
        if (completed->getBool("level" + to_string(i * 10 + 10))) {
            _levelSelectPanel[i]->createChildButtonTextureWithName(275, -120, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level"+ to_string(i * 10 + 10) +"Complete"), "level" + to_string(i * 10 + 10));
            _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 10)]->getButton()->setScale(1.0f);
            _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 10)]->getButton()->addListener([=](const std::string& name, bool down) {
                // Only quit when the button is released
                if (!down) {
                    //CULog("Clicking on possess button!");
                    // Mark this button as clicked, proper handle will take place in update()
                    _levelID = to_string(i * 10 + 9);
                    CULog("Invisible button?");
                    _levelSelected = true;
                }
                });
        }
    }
    
    activateButtons();
    reader->close();
}

void LevelSelectMode::buildScene() {
    Size  size = Application::get()->getDisplaySize();
    float scale = GAME_WIDTH / size.width;
    size *= scale;
    _levelSelectRootNode = scene2::SceneNode::alloc();
    _levelSelectRootNode->setAnchor(Vec2::ANCHOR_CENTER);
    _levelSelectRootNode->setPosition(0, 0);
    for (int i = 0; i < MAX_LEVEL_PAGE; i++) {
        _levelSelectPanel[i] = ui::PanelElement::alloc((i + 0.5f) * size.width, 0.5f * size.height, 0, _assets->get<Texture>("levelSelectBG"));
        _levelSelectPanel[i]->getSceneNode()->setScale(min(size.width / _assets->get<Texture>("levelSelectBG")->getSize().width, size.height / _assets->get<Texture>("levelSelectBG")->getSize().height));
        _levelSelectRootNode->addChild(_levelSelectPanel[i]->getSceneNode());
        _levelSelectPanel[i]->createChildButton(-380, 60, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level"+to_string(i * 10 + 1)), "level" + to_string(i * 10 + 1));
        _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 1)]->getButton()->setScale(1.0f);
        _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 1)]->getButton()->addListener([=](const std::string& name, bool down) {
            // Only quit when the button is released
            if (!down) {
                //CULog("Clicking on possess button!");
                // Mark this button as clicked, proper handle will take place in update()
                _levelID = to_string(i * 10 + 0);
                CULog("Invisible button?");
                _levelSelected = true;
            }
            });
        _levelSelectPanel[i]->createChildButton(-220, 60, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level" + to_string(i * 10 + 2)), "level" + to_string(i * 10 + 2));
        _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 2)]->getButton()->setScale(1.0f);
        _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 2)]->getButton()->addListener([=](const std::string& name, bool down) {
            // Only quit when the button is released
            if (!down) {
                //CULog("Clicking on possess button!");
                // Mark this button as clicked, proper handle will take place in update()
                _levelID = to_string(i * 10 + 1);
                CULog("Invisible button?");
                _levelSelected = true;
            }
            });
        _levelSelectPanel[i]->createChildButton(-55, 60, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level" + to_string(i * 10 + 3)), "level" + to_string(i * 10 + 3));
        _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 3)]->getButton()->setScale(1.0f);
        _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 3)]->getButton()->addListener([=](const std::string& name, bool down) {
            // Only quit when the button is released
            if (!down) {
                //CULog("Clicking on possess button!");
                // Mark this button as clicked, proper handle will take place in update()
                _levelID = to_string(i * 10 + 2);
                CULog("Invisible button?");
                _levelSelected = true;
            }
            });
        _levelSelectPanel[i]->createChildButton(105, 60, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level" + to_string(i * 10 + 4)), "level" + to_string(i * 10 + 4));
        _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 4)]->getButton()->setScale(1.0f);
        _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 4)]->getButton()->addListener([=](const std::string& name, bool down) {
            // Only quit when the button is released
            if (!down) {
                //CULog("Clicking on possess button!");
                // Mark this button as clicked, proper handle will take place in update()
                _levelID = to_string(i * 10 + 3);
                CULog("Invisible button?");
                _levelSelected = true;
            }
            });
        _levelSelectPanel[i]->createChildButton(265, 55, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level" + to_string(i * 10 + 5)), "level" + to_string(i * 10 + 5));
        _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 5)]->getButton()->setScale(1.0f);
        _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 5)]->getButton()->addListener([=](const std::string& name, bool down) {
            // Only quit when the button is released
            if (!down) {
                //CULog("Clicking on possess button!");
                // Mark this button as clicked, proper handle will take place in update()
                _levelID = to_string(i * 10 + 4);
                CULog("Invisible button?");
                _levelSelected = true;
            }
            });
        _levelSelectPanel[i]->createChildButton(-380, -110, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level" + to_string(i * 10 + 6)), "level" + to_string(i * 10 + 6));
        _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 6)]->getButton()->setScale(1.0f);
        _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 6)]->getButton()->addListener([=](const std::string& name, bool down) {
            // Only quit when the button is released
            if (!down) {
                //CULog("Clicking on possess button!");
                // Mark this button as clicked, proper handle will take place in update()
                _levelID = to_string(i * 10 + 5);
                CULog("Invisible button?");
                _levelSelected = true;
            }
            });
        _levelSelectPanel[i]->createChildButton(-220, -110, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level" + to_string(i * 10 + 7)), "level" + to_string(i * 10 + 7));
        _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 7)]->getButton()->setScale(1.0f);
        _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 7)]->getButton()->addListener([=](const std::string& name, bool down) {
            // Only quit when the button is released
            if (!down) {
                //CULog("Clicking on possess button!");
                // Mark this button as clicked, proper handle will take place in update()
                _levelID = to_string(i * 10 + 6);
                CULog("Invisible button?");
                _levelSelected = true;
            }
            });
        _levelSelectPanel[i]->createChildButton(-55, -115, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level" + to_string(i * 10 + 8)), "level" + to_string(i * 10 + 8));
        _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 8)]->getButton()->setScale(1.0f);
        _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 8)]->getButton()->addListener([=](const std::string& name, bool down) {
            // Only quit when the button is released
            if (!down) {
                //CULog("Clicking on possess button!");
                // Mark this button as clicked, proper handle will take place in update()
                _levelID = to_string(i * 10 + 7);
                CULog("Invisible button?");
                _levelSelected = true;
            }
            });
        _levelSelectPanel[i]->createChildButton(110, -115, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level" + to_string(i * 10 + 9)), "level" + to_string(i * 10 + 9));
        _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 9)]->getButton()->setScale(1.0f);
        _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 9)]->getButton()->addListener([=](const std::string& name, bool down) {
            // Only quit when the button is released
            if (!down) {
                //CULog("Clicking on possess button!");
                // Mark this button as clicked, proper handle will take place in update()
                _levelID = to_string(i * 10 + 8);
                CULog("Invisible button?");
                _levelSelected = true;
            }
            });
        _levelSelectPanel[i]->createChildButton(275, -120, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level" + to_string(i * 10 + 10)), "level" + to_string(i * 10 + 10));
        _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 10)]->getButton()->setScale(1.0f);
        _levelSelectPanel[i]->getChildButtons()["level" + to_string(i * 10 + 10)]->getButton()->addListener([=](const std::string& name, bool down) {
            // Only quit when the button is released
            if (!down) {
                //CULog("Clicking on possess button!");
                // Mark this button as clicked, proper handle will take place in update()
                _levelID = to_string(i * 10 + 9);
                CULog("Invisible button?");
                _levelSelected = true;
            }
            });
    }
  
    std::shared_ptr<AudioQueue> audioQueue = AudioEngine::get()->getMusicQueue();
    if (audioQueue->getVolume() < 0.5f) {
        // Game is muted
        _gameMuted = true;
    }
    else {
        // Game is unmuted
        _gameMuted = false;
    }
    _uiPanel = ui::PanelElement::alloc(size.width / 2.0f, size.height / 2.0f, 0, nullptr);
    if (_gameMuted) {
        audioQueue->setVolume(0);
        _uiPanel->getSceneNode()->removeChildByName("muteButton");
        _uiPanel->createChildButton((size.width - 100.0f) / 2.0f, (size.height - 100.0f) / 2.0f, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("mute"), Color4f::WHITE, "unmuteButton");
        _uiPanel->getChildButtons()["unmuteButton"]->getButton()->setScale(Vec2(0.8f, 0.8f));
        _uiPanel->getChildButtons()["unmuteButton"]->getButton()->setName("unmuteButton");
        _uiPanel->getChildButtons()["unmuteButton"]->getButton()->addListener([=](const std::string& name, bool down) {
            // Only quit when the button is released
            if (!down) {
                _gameMuted = false;
                _toUpdateVolume = true;
            }
            });
        _uiPanel->getChildButtons()["unmuteButton"]->getButton()->setVisible(true);
        _uiPanel->getChildButtons()["unmuteButton"]->getButton()->activate();
    }
    else {
        audioQueue->setVolume(1.0f);
        _uiPanel->getSceneNode()->removeChildByName("unmuteButton");
        _uiPanel->createChildButton((size.width - 100.0f) / 2.0f, (size.height - 100.0f) / 2.0f, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("unmute"), Color4f::WHITE, "muteButton");
        _uiPanel->getChildButtons()["muteButton"]->getButton()->setScale(Vec2(0.8f, 0.8f));
        _uiPanel->getChildButtons()["muteButton"]->getButton()->setName("muteButton");
        _uiPanel->getChildButtons()["muteButton"]->getButton()->addListener([=](const std::string& name, bool down) {
            // Only quit when the button is released
            if (!down) {
                _gameMuted = true;
                _toUpdateVolume = true;
            }
            });
        _uiPanel->getChildButtons()["muteButton"]->getButton()->setVisible(true);
        _uiPanel->getChildButtons()["muteButton"]->getButton()->activate();
    }
    // Add the page flip buttons
    _uiPanel->createChildButton((size.width - 150.0f) / 2.0f, (size.height - 350.0f) / 2.0f, 50, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("arrowNextPage"), Color4f::WHITE, "nextPage");
    _uiPanel->getChildButtons()["nextPage"]->getButton()->setScale(Vec2(0.5f, 0.5f));
    _uiPanel->getChildButtons()["nextPage"]->getButton()->setName("nextPage");
    _uiPanel->getChildButtons()["nextPage"]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            _pageChange = 1;
        }
        });
    _uiPanel->createChildButton((size.width - 150.0f) / 2.0f, (size.height - 525.0f) / 2.0f, 50, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("arrowPrevPage"), Color4f::WHITE, "prevPage");
    _uiPanel->getChildButtons()["prevPage"]->getButton()->setScale(Vec2(0.5f, 0.5f));
    _uiPanel->getChildButtons()["prevPage"]->getButton()->setName("prevPage");
    _uiPanel->getChildButtons()["prevPage"]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            _pageChange = -1;
        }
        });
    updatePage();
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
    addChild(_levelSelectRootNode);
    addChild(_uiPanel->getSceneNode());
    addChild(_backButton->getButton());
    activateButtons();
    updateLevelIcon();
}

void LevelSelectMode::deactivateButtons() {
    _backButton->getButton()->deactivate();
    for (int i = 0; i < MAX_LEVEL_PAGE; i++) {
        for (int j = 1; j <= 10; j++) {
            _levelSelectPanel[i]->getChildButtons()["level" + to_string(10 * i + j)]->getButton()->deactivate();
        }
    }
}

void LevelSelectMode::activateButtons() {
    _backButton->getButton()->activate();
    for (int i = 0; i < MAX_LEVEL_PAGE; i++) {
        for (int j = 1; j <= 10; j++) {
            if (10 * i + j > MAX_LEVEL_NUM_PER_LOC) continue;
            _levelSelectPanel[i]->getChildButtons()["level" + to_string(10 * i + j)]->getButton()->activate();
        }
    }
}
