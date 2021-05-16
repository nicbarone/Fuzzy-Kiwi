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
    if (_toUpdateVolume) {
        _toUpdateVolume = false;
        if (_levelSelectPanel->getChildButtons()["muteButton"] != nullptr) {
            _levelSelectPanel->getChildButtons()["muteButton"]->getButton()->deactivate();
            _levelSelectPanel->getSceneNode()->removeChildByName("muteButton");
        }
        if (_levelSelectPanel->getChildButtons()["unmuteButton"] != nullptr) {
            _levelSelectPanel->getChildButtons()["unmuteButton"]->getButton()->deactivate();
            _levelSelectPanel->getSceneNode()->removeChildByName("unmuteButton");
        }
        if (_gameMuted) {
            audioQueue->setVolume(0);
            _levelSelectPanel->createChildButton((size.width - 100.0f) / 2.0f / _levelSelectPanel->getSceneNode()->getScaleX(), (size.height - 100.0f) / 2.0f / _levelSelectPanel->getSceneNode()->getScaleY(), 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("mute"), Color4f::WHITE, "unmuteButton");
            _levelSelectPanel->getChildButtons()["unmuteButton"]->getButton()->setScale(Vec2(0.8f, 0.8f));
            _levelSelectPanel->getChildButtons()["unmuteButton"]->getButton()->setName("unmuteButton");
            _levelSelectPanel->getChildButtons()["unmuteButton"]->getButton()->addListener([=](const std::string& name, bool down) {
                // Only quit when the button is released
                if (!down) {
                    _gameMuted = false;
                    _toUpdateVolume = true;
                }
                });
            _levelSelectPanel->getChildButtons()["unmuteButton"]->getButton()->setVisible(true);
            _levelSelectPanel->getChildButtons()["unmuteButton"]->getButton()->activate();
        }
        else {
            audioQueue->setVolume(1.0f);
            _levelSelectPanel->createChildButton((size.width - 100.0f) / 2.0f / _levelSelectPanel->getSceneNode()->getScaleX(), (size.height - 100.0f) / 2.0f / _levelSelectPanel->getSceneNode()->getScaleY(), 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("unmute"), Color4f::WHITE, "muteButton");
            _levelSelectPanel->getChildButtons()["muteButton"]->getButton()->setScale(Vec2(0.8f, 0.8f));
            _levelSelectPanel->getChildButtons()["muteButton"]->getButton()->setName("muteButton");
            _levelSelectPanel->getChildButtons()["muteButton"]->getButton()->addListener([=](const std::string& name, bool down) {
                // Only quit when the button is released
                if (!down) {
                    _gameMuted = true;
                    _toUpdateVolume = true;
                }
                });
            _levelSelectPanel->getChildButtons()["muteButton"]->getButton()->setVisible(true);
            _levelSelectPanel->getChildButtons()["muteButton"]->getButton()->activate();
        }
    }
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
        _levelSelectPanel->createChildButtonTextureWithName(-380, 60, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level1Complete"), "level1");
        _levelSelectPanel->getChildButtons()["level1"]->getButton()->setScale(1.0f);
        _levelSelectPanel->getChildButtons()["level1"]->getButton()->addListener([=](const std::string& name, bool down) {
            // Only quit when the button is released
            if (!down) {
                //CULog("Clicking on possess button!");
                // Mark this button as clicked, proper handle will take place in update()
                _levelID = "0";
                _levelSelected = true;
            }
            });
    }
    if (completed->getBool("level2")) {
        _levelSelectPanel->createChildButtonTextureWithName(-220, 60, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level2Complete"), "level2");
        _levelSelectPanel->getChildButtons()["level2"]->getButton()->setScale(1.0f);
        _levelSelectPanel->getChildButtons()["level2"]->getButton()->addListener([=](const std::string& name, bool down) {
            // Only quit when the button is released
            if (!down) {
                //CULog("Clicking on possess button!");
                // Mark this button as clicked, proper handle will take place in update()
                _levelID = "1";
                _levelSelected = true;
            }
            });
    }
    if (completed->getBool("level3")) {
        _levelSelectPanel->createChildButtonTextureWithName(-55, 60, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level3Complete"), "level3");
        _levelSelectPanel->getChildButtons()["level3"]->getButton()->setScale(1.0f);
        _levelSelectPanel->getChildButtons()["level3"]->getButton()->addListener([=](const std::string& name, bool down) {
            // Only quit when the button is released
            if (!down) {
                //CULog("Clicking on possess button!");
                // Mark this button as clicked, proper handle will take place in update()
                _levelID = "2";
                _levelSelected = true;
            }
            });
    }
    if (completed->getBool("level4")) {
        _levelSelectPanel->createChildButtonTextureWithName(105, 60, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level4Complete"), "level4");
        _levelSelectPanel->getChildButtons()["level4"]->getButton()->setScale(1.0f);
        _levelSelectPanel->getChildButtons()["level4"]->getButton()->addListener([=](const std::string& name, bool down) {
            // Only quit when the button is released
            if (!down) {
                //CULog("Clicking on possess button!");
                // Mark this button as clicked, proper handle will take place in update()
                _levelID = "3";
                _levelSelected = true;
            }
            });
    }
    if (completed->getBool("level5")) {
        _levelSelectPanel->createChildButtonTextureWithName(265, 55, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level5Complete"), "level5");
        _levelSelectPanel->getChildButtons()["level5"]->getButton()->setScale(1.0f);
        _levelSelectPanel->getChildButtons()["level5"]->getButton()->addListener([=](const std::string& name, bool down) {
            // Only quit when the button is released
            if (!down) {
                //CULog("Clicking on possess button!");
                // Mark this button as clicked, proper handle will take place in update()
                _levelID = "4";
                _levelSelected = true;
            }
            });
    }
    if (completed->getBool("level6")) {
        _levelSelectPanel->createChildButtonTextureWithName(-380, -110, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level6Complete"), "level6");
        _levelSelectPanel->getChildButtons()["level6"]->getButton()->setScale(1.0f);
        _levelSelectPanel->getChildButtons()["level6"]->getButton()->addListener([=](const std::string& name, bool down) {
            // Only quit when the button is released
            if (!down) {
                //CULog("Clicking on possess button!");
                // Mark this button as clicked, proper handle will take place in update()
                _levelID = "5";
                _levelSelected = true;
            }
            });
    }
    if (completed->getBool("level7")) {
        _levelSelectPanel->createChildButtonTextureWithName(-220, -110, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level7Complete"), "level7");
        _levelSelectPanel->getChildButtons()["level7"]->getButton()->setScale(1.0f);
        _levelSelectPanel->getChildButtons()["level7"]->getButton()->addListener([=](const std::string& name, bool down) {
            // Only quit when the button is released
            if (!down) {
                //CULog("Clicking on possess button!");
                // Mark this button as clicked, proper handle will take place in update()
                _levelID = "6";
                _levelSelected = true;
            }
            });
    }
    if (completed->getBool("level8")) {
        _levelSelectPanel->createChildButtonTextureWithName(-55, -115, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level8Complete"), "level8");
        _levelSelectPanel->getChildButtons()["level8"]->getButton()->setScale(1.0f);
        _levelSelectPanel->getChildButtons()["level8"]->getButton()->addListener([=](const std::string& name, bool down) {
            // Only quit when the button is released
            if (!down) {
                //CULog("Clicking on possess button!");
                // Mark this button as clicked, proper handle will take place in update()
                _levelID = "7";
                _levelSelected = true;
            }
            });
    }
    if (completed->getBool("level9")) {
        _levelSelectPanel->createChildButtonTextureWithName(110, -115, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level9Complete"), "level9");
        _levelSelectPanel->getChildButtons()["level9"]->getButton()->setScale(1.0f);
        _levelSelectPanel->getChildButtons()["level9"]->getButton()->addListener([=](const std::string& name, bool down) {
            // Only quit when the button is released
            if (!down) {
                //CULog("Clicking on possess button!");
                // Mark this button as clicked, proper handle will take place in update()
                _levelID = "8";
                _levelSelected = true;
            }
            });
    }
    if (completed->getBool("level10")) {
        _levelSelectPanel->createChildButtonTextureWithName(275, -120, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level10Complete"), "level10");
        _levelSelectPanel->getChildButtons()["level10"]->getButton()->setScale(1.0f);
        _levelSelectPanel->getChildButtons()["level10"]->getButton()->addListener([=](const std::string& name, bool down) {
            // Only quit when the button is released
            if (!down) {
                //CULog("Clicking on possess button!");
                // Mark this button as clicked, proper handle will take place in update()
                _levelID = "9";
                _levelSelected = true;
            }
            });
    }
    activateButtons();
    reader->close();
}

void LevelSelectMode::buildScene() {
    Size  size = Application::get()->getDisplaySize();
    float scale = GAME_WIDTH / size.width;
    size *= scale;
    
    _levelSelectPanel = ui::PanelElement::alloc(size.width / 2, size.height / 2, 0, _assets->get<Texture>("levelSelectBG"));
    _levelSelectPanel->getSceneNode()->setScale(min(size.width / _assets->get<Texture>("levelSelectBG")->getSize().width, size.height / _assets->get<Texture>("levelSelectBG")->getSize().height));
    _levelSelectPanel->createChildButton(-380, 60, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level1"), "level1");
    _levelSelectPanel->getChildButtons()["level1"]->getButton()->setScale(1.0f);
    _levelSelectPanel->getChildButtons()["level1"]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _levelID = "0";
            _levelSelected = true;
        }
    });
    _levelSelectPanel->createChildButton(-220, 60, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level2"), "level2");
    _levelSelectPanel->getChildButtons()["level2"]->getButton()->setScale(1.0f);
    _levelSelectPanel->getChildButtons()["level2"]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _levelID = "1";
            _levelSelected = true;
        }
        });
    _levelSelectPanel->createChildButton(-55, 60, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level3"), "level3");
    _levelSelectPanel->getChildButtons()["level3"]->getButton()->setScale(1.0f);
    _levelSelectPanel->getChildButtons()["level3"]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _levelID = "2";
            _levelSelected = true;
        }
        });
    _levelSelectPanel->createChildButton(105, 60, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level4"), "level4");
    _levelSelectPanel->getChildButtons()["level4"]->getButton()->setScale(1.0f);
    _levelSelectPanel->getChildButtons()["level4"]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _levelID = "3";
            _levelSelected = true;
        }
        });
    _levelSelectPanel->createChildButton(265, 55, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level5"), "level5");
    _levelSelectPanel->getChildButtons()["level5"]->getButton()->setScale(1.0f);
    _levelSelectPanel->getChildButtons()["level5"]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _levelID = "4";
            _levelSelected = true;
        }
        });
    _levelSelectPanel->createChildButton(-380, -110, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level6"), "level6");
    _levelSelectPanel->getChildButtons()["level6"]->getButton()->setScale(1.0f);
    _levelSelectPanel->getChildButtons()["level6"]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _levelID = "5";
            _levelSelected = true;
        }
        });
    _levelSelectPanel->createChildButton(-220, -110, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level7"), "level7");
    _levelSelectPanel->getChildButtons()["level7"]->getButton()->setScale(1.0f);
    _levelSelectPanel->getChildButtons()["level7"]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _levelID = "6";
            _levelSelected = true;
        }
        });
    _levelSelectPanel->createChildButton(-55, -115, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level8"), "level8");
    _levelSelectPanel->getChildButtons()["level8"]->getButton()->setScale(1.0f);
    _levelSelectPanel->getChildButtons()["level8"]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _levelID = "7";
            _levelSelected = true;
        }
        });
    _levelSelectPanel->createChildButton(110, -115, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level9"), "level9");
    _levelSelectPanel->getChildButtons()["level9"]->getButton()->setScale(1.0f);
    _levelSelectPanel->getChildButtons()["level9"]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _levelID = "8";
            _levelSelected = true;
        }
        });
    _levelSelectPanel->createChildButton(275, -120, 20, 20, ui::ButtonState::AVAILABLE, _assets->get<Texture>("level10"), "level10");
    _levelSelectPanel->getChildButtons()["level10"]->getButton()->setScale(1.0f);
    _levelSelectPanel->getChildButtons()["level10"]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _levelID = "9";
            _levelSelected = true;
        }
        });
    std::shared_ptr<AudioQueue> audioQueue = AudioEngine::get()->getMusicQueue();
    if (audioQueue->getVolume() < 0.5f) {
        // Game is muted
        _gameMuted = true;
    }
    else {
        // Game is unmuted
        _gameMuted = false;
    }
    if (_gameMuted) {
            audioQueue->setVolume(0);
            _levelSelectPanel->getSceneNode()->removeChildByName("muteButton");
            _levelSelectPanel->createChildButton((size.width - 100.0f) / 2.0f / _levelSelectPanel->getSceneNode()->getScaleX(), (size.height - 100.0f) / 2.0f / _levelSelectPanel->getSceneNode()->getScaleY(), 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("mute"), Color4f::WHITE, "unmuteButton");
            _levelSelectPanel->getChildButtons()["unmuteButton"]->getButton()->setScale(Vec2(0.8f, 0.8f));
            _levelSelectPanel->getChildButtons()["unmuteButton"]->getButton()->setName("unmuteButton");
            _levelSelectPanel->getChildButtons()["unmuteButton"]->getButton()->addListener([=](const std::string& name, bool down) {
                // Only quit when the button is released
                if (!down) {
                    _gameMuted = false;
                    _toUpdateVolume = true;
                }
                });
            _levelSelectPanel->getChildButtons()["unmuteButton"]->getButton()->setVisible(true);
            _levelSelectPanel->getChildButtons()["unmuteButton"]->getButton()->activate();
        }
        else {
            audioQueue->setVolume(1.0f);
            _levelSelectPanel->getSceneNode()->removeChildByName("unmuteButton");
            _levelSelectPanel->createChildButton((size.width - 100.0f) / 2.0f / _levelSelectPanel->getSceneNode()->getScaleX(), (size.height - 100.0f) / 2.0f / _levelSelectPanel->getSceneNode()->getScaleY(), 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("unmute"), Color4f::WHITE, "muteButton");
            _levelSelectPanel->getChildButtons()["muteButton"]->getButton()->setScale(Vec2(0.8f, 0.8f));
            _levelSelectPanel->getChildButtons()["muteButton"]->getButton()->setName("muteButton");
            _levelSelectPanel->getChildButtons()["muteButton"]->getButton()->addListener([=](const std::string& name, bool down) {
                // Only quit when the button is released
                if (!down) {
                    _gameMuted = true;
                    _toUpdateVolume = true;
                }
                });
            _levelSelectPanel->getChildButtons()["muteButton"]->getButton()->setVisible(true);
            _levelSelectPanel->getChildButtons()["muteButton"]->getButton()->activate();
        }
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
    _levelSelectPanel->getChildButtons()["level1"]->getButton()->deactivate();
    _levelSelectPanel->getChildButtons()["level2"]->getButton()->deactivate();
    _levelSelectPanel->getChildButtons()["level3"]->getButton()->deactivate();
    _levelSelectPanel->getChildButtons()["level4"]->getButton()->deactivate();
    _levelSelectPanel->getChildButtons()["level5"]->getButton()->deactivate();
    _levelSelectPanel->getChildButtons()["level6"]->getButton()->deactivate();
    _levelSelectPanel->getChildButtons()["level7"]->getButton()->deactivate();
    _levelSelectPanel->getChildButtons()["level8"]->getButton()->deactivate();
    _levelSelectPanel->getChildButtons()["level9"]->getButton()->deactivate();
    _levelSelectPanel->getChildButtons()["level10"]->getButton()->deactivate();
}

void LevelSelectMode::activateButtons() {
    _backButton->getButton()->activate();
    _levelSelectPanel->getChildButtons()["level1"]->getButton()->activate();
    _levelSelectPanel->getChildButtons()["level2"]->getButton()->activate();
    _levelSelectPanel->getChildButtons()["level3"]->getButton()->activate();
    _levelSelectPanel->getChildButtons()["level4"]->getButton()->activate();
    _levelSelectPanel->getChildButtons()["level5"]->getButton()->activate();
    _levelSelectPanel->getChildButtons()["level6"]->getButton()->activate();
    _levelSelectPanel->getChildButtons()["level7"]->getButton()->activate();
    _levelSelectPanel->getChildButtons()["level8"]->getButton()->activate();
    _levelSelectPanel->getChildButtons()["level9"]->getButton()->activate();
    _levelSelectPanel->getChildButtons()["level10"]->getButton()->activate();
}
