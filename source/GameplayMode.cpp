//  Cornell University Game Library (CUGL)
//
//  This is the implementation file for the custom application. This is the
//  definition of your root (and in this case only) class.
//
//  CUGL zlib License:
//      This software is provided 'as-is', without any express or implied
//      warranty.  In no event will the authors be held liable for any damages
//      arising from the use of this software.
//
//      Permission is granted to anyone to use this software for any purpose,
//      including commercial applications, and to alter it and redistribute it
//      freely, subject to the following restrictions:
//
//      1. The origin of this software must not be misrepresented; you must not
//      claim that you wrote the original software. If you use this software
//      in a product, an acknowledgment in the product documentation would be
//      appreciated but is not required.
//
//      2. Altered source versions must be plainly marked as such, and must not
//      be misrepresented as being the original software.
//
//      3. This notice may not be removed or altered from any source distribution.
//
//  Author: Walker White
//  Version: 1/8/17
//
// Include the class header, which includes all of the CUGL classes
#include "GameplayMode.h"
#include "cugl/base/CUBase.h"
#include <cugl/scene2/ui/CULabel.h>

// Add support for simple random number generation
#include "cstdlib"
#include "ctime"

// This keeps us from having to write cugl:: all the time
using namespace cugl;

// The number of frames before moving the logo to a new position
#define TIME_STEP 60


/**
 * The method called after OpenGL is initialized, but before running the application.
 *
 * This is the method in which all user-defined program intialization should
 * take place.  You should not create a new init() method.
 *
 * When overriding this method, you should call the parent method as the
 * very last line.  This ensures that the state will transition to FOREGROUND,
 * causing the application to run.
 */

bool GameplayMode::init(const std::shared_ptr<cugl::AssetManager>& assets, int level, std::shared_ptr<InputManager> inputManager, bool muted) {
    setGameStatus(GameStatus::RUNNING);
    _victoryPage = false;
    _inputManager = inputManager;
    _levelIndex = level;
    _gameMuted = muted;
    Size size = Application::get()->getDisplaySize();

    size *= GAME_WIDTH / size.width;
    if (assets == nullptr) {
        return false;
    }
    else if (!Scene2::init(size)) {
        return false;
    }
    // Create a scene graph the same size as the window
    //_scene = Scene2::alloc(size.width, size.height);
    _rootScene = scene2::OrderedNode::allocWithOrder(scene2::OrderedNode::Order::PRE_ASCEND);
    _rootScene->setAnchor(Vec2::ANCHOR_CENTER);

    _rootScene->setContentSize(size);
    _reset = false;
    _backToMenu = false;

    // Create an asset manager to load all assets
    _assets = assets;


    // Activate mouse or touch screen input as appropriate
    // We have to do this BEFORE the scene, because the scene has a button
#if defined (CU_TOUCH_SCREEN)
    Input::activate<Touchscreen>();
#else
    Input::activate<Mouse>();
    Input::get<Mouse>()->setPointerAwareness(Mouse::PointerAwareness::DRAG);
    Input::activate<Keyboard>();
#endif

    // Build the scene from these assets


    // Report the safe area
    Rect bounds = Display::get()->getSafeBounds();
    // CULog("Safe Area %sx%s",bounds.origin.toString().c_str(),
     //                        bounds.size.toString().c_str());

    bounds = Application::get()->getSafeBounds();
    //CULog("Safe Area %sx%s",bounds.origin.toString().c_str(),
    //                        bounds.size.toString().c_str());

    bounds = Application::get()->getDisplayBounds();
    //CULog("Full Area %sx%s",bounds.origin.toString().c_str(),
    //                        bounds.size.toString().c_str());
    return true;
}

bool GameplayMode::init(const std::shared_ptr<cugl::AssetManager>& assets, int level, std::shared_ptr<JsonValue> json, std::shared_ptr<InputManager> inputManager, bool muted) {
    _showTutorialText = false;
    _inputManager = inputManager;
    _levelIndex = level;
    _gameMuted = muted;
    _victoryPage = false;
    Size size = Application::get()->getDisplaySize();
    _json = json;
    size *= GAME_WIDTH / size.width;
    if (assets == nullptr) {
        return false;
    }
    else if (!Scene2::init(size)) {
        return false;
    }
    // Create a scene graph the same size as the window
    //_scene = Scene2::alloc(size.width, size.height);
    _rootScene = scene2::OrderedNode::allocWithOrder(scene2::OrderedNode::Order::ASCEND);
    _rootScene->setAnchor(Vec2::ANCHOR_CENTER);
    setName("gameplayMode");
    _rootScene->setName("rootScene");
    _rootScene->setContentSize(size);
    _reset = false;
    _backToMenu = false;

    Application::get()->setClearColor(Color4(0, 0, 0, 1));

    // Create an asset manager to load all assets
    _assets = assets;


    // Activate mouse or touch screen input as appropriate
    // We have to do this BEFORE the scene, because the scene has a button
#if defined (CU_TOUCH_SCREEN)
    Input::activate<Touchscreen>();
#else
    Input::activate<Mouse>();
    Input::get<Mouse>()->setPointerAwareness(Mouse::PointerAwareness::DRAG);
    Input::activate<Keyboard>();
#endif

    // Build the scene from these assets
    buildScene(json);


    // Report the safe area
    Rect bounds = Display::get()->getSafeBounds();
    // CULog("Safe Area %sx%s",bounds.origin.toString().c_str(),
     //                        bounds.size.toString().c_str());

    bounds = Application::get()->getSafeBounds();
    //CULog("Safe Area %sx%s",bounds.origin.toString().c_str(),
    //                        bounds.size.toString().c_str());

    bounds = Application::get()->getDisplayBounds();
    //CULog("Full Area %sx%s",bounds.origin.toString().c_str(),
    //                        bounds.size.toString().c_str());
    return true;
}


void GameplayMode::reset() {
    setGameStatus(GameStatus::RUNNING);
    //_scene = nullptr;
    Size size = Application::get()->getDisplaySize();
    _winPanel->getChildButtons()["next"]->getButton()->deactivate();
    _winPanel->getChildButtons()["retry"]->getButton()->deactivate();
    _winPanel->getChildButtons()["toMenu"]->getButton()->deactivate();
    _losePanel->getChildButtons()["retry"]->getButton()->deactivate();
    _losePanel->getChildButtons()["toMenu"]->getButton()->deactivate();
    _menuPanel->getChildButtons()["resume"]->getButton()->deactivate();
    _menuPanel->getChildButtons()["restart"]->getButton()->deactivate();
    _menuPanel->getChildButtons()["exit"]->getButton()->deactivate();
    size *= GAME_WIDTH / size.width;
    removeAllChildren();

    // Create a scene graph the same size as the window
    alloc(size.width, size.height);
    _rootScene = scene2::OrderedNode::allocWithOrder(scene2::OrderedNode::Order::ASCEND);
    _rootScene->setAnchor(Vec2::ANCHOR_CENTER);
    _rootScene->setContentSize(size);
    _reset = false;
    if (_json != nullptr) {
        buildScene(_json);
    }

    if (_json->getLong("level", -1L) != -1) {
        _levelIndex = _json->getLong("level", -1L);
        shared_ptr<JsonReader> reader = JsonReader::allocWithAsset("levels\\level" + std::to_string(_levelIndex + 1) + ".json");
        _json = reader->readJson();
    }
}

/**
 * The method called to update the application data.
 *
 * This is your core loop and should be replaced with your custom implementation.
 * This method should contain any code that is not an OpenGL call.
 *
 * When overriding this method, you do not need to call the parent method
 * at all. The default implmentation does nothing.
 *
 * @param timestep  The amount of time (in seconds) since the last frame
 */
void GameplayMode::update(float timestep) {
    if (_reset) {
        reset();
    }
    std::shared_ptr<AudioQueue> audioQueue = AudioEngine::get()->getMusicQueue();
    if (_victoryPage) {
        audioQueue->setVolume(0);
    }
    else {
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
    if (getGameStatus() == GameStatus::PAUSED) {
        if (_tutorialAnimation != nullptr) {
            _tutorialAnimation->setVisible(false);
        }
        _levelBoardPanel->getChildPanels()[0]->setVisible(true);
        _menuPanel->setVisible(true);
        _menuPanel->getChildButtons()["resume"]->getButton()->activate();
        _menuPanel->getChildButtons()["restart"]->getButton()->activate();
        _menuPanel->getChildButtons()["exit"]->getButton()->activate();
    }
    else {
        _menuPanel->setVisible(false);
        _menuPanel->getChildButtons()["resume"]->getButton()->deactivate();
        _menuPanel->getChildButtons()["restart"]->getButton()->deactivate();
        _menuPanel->getChildButtons()["exit"]->getButton()->deactivate();
    }
    if (getGameStatus() != GameStatus::RUNNING) {
        return;
    }
    if (_showTutorialText == 1) {
        if (!_player->canPossess() && !_player->getPossess() && !_doors.at(0)->getIsOpen()
            && _player->getPos() < 662 && _tutorialAnimation->getAngle() == 0) {
            _tutorialText->setText("Oh no! You are stuck! Use the pause button on the top left to retry this level.");
            removeFromParentByName("tutorialAnimation");
            std::shared_ptr<Texture> arrow = _assets->get<Texture>("arrow");
            _tutorialAnimation = scene2::AnimationNode::alloc(arrow, 1, 2);
            _tutorialAnimation->setName("tutorialAnimation");
            _tutorialAnimation->setPosition(135, 425);
            _tutorialAnimation->setScale(0.25, 0.25);
            _tutorialAnimation->setAngle(3 * 0.785398);
            addChild(_tutorialAnimation);
            tutMaxFrame = 2;
            FRAME_SWITCH = 14;
            _tutorialAnimation->setPriority(10);
        }
        else if (!_player->getPossess() && _player->getPos() > 662 && tutMaxFrame != 2) {
            removeFromParentByName("tutorialAnimation");
            std::shared_ptr<Texture> arrow = _assets->get<Texture>("arrow");
            _tutorialAnimation = scene2::AnimationNode::alloc(arrow, 1, 2);
            _tutorialAnimation->setName("tutorialAnimation");
            _tutorialAnimation->setPosition(1075, 200);
            _tutorialAnimation->setScale(0.25, 0.25);
            _rootScene->addChild(_tutorialAnimation);
            tutMaxFrame = 2;
            FRAME_SWITCH = 14;
            _tutorialAnimation->setPriority(10);
        }
    }
    else if (_showTutorialText == 2) {
        if (_player->getCurrentDen() > 0 && _rootScene->getPositionX() < 700 && _tutorialAnimation->getPositionX() != 760) {
            _tutorialText2->setText("Now tap on the other cat den to leave!");
            _tutorialText2->setPositionX(300);
            removeFromParentByName("tutorialAnimation");
            std::shared_ptr<Texture> tapHand = _assets->get<Texture>("tapHand");
            _tutorialAnimation = scene2::AnimationNode::alloc(tapHand, 1, 5);
            _tutorialAnimation->setName("tutorialAnimation");
            _tutorialAnimation->setPosition(760, 175);
            _tutorialAnimation->setScale(0.25, 0.25);
            _rootScene->addChild(_tutorialAnimation);
            tutMaxFrame = 4;
        }
        if (_player->getCurrentDoor() > 0 && (_rootScene->getScaleX() < 0.31f  || _rootScene->getScaleX() == 1.0f) 
            && _tutorialAnimation->getPositionX() != -50) {
            _tutorialText2->setText("Now tap on the other staircase door to leave!");
            _tutorialText2->setPositionX(300);
            removeFromParentByName("tutorialAnimation");
            std::shared_ptr<Texture> tapHand = _assets->get<Texture>("tapHand");
            _tutorialAnimation = scene2::AnimationNode::alloc(tapHand, 1, 5);
            _tutorialAnimation->setName("tutorialAnimation");
            _tutorialAnimation->setPosition(366, 600);
            _tutorialAnimation->setScale(0.25, 0.25);
            _tutorialAnimation->setPriority(1000);
            //FRAME_SWITCH = 14;
            _rootScene->addChild(_tutorialAnimation);
            tutMaxFrame = 4;
        }
        if (_player->getLevel() == 1) {
            removeFromParentByName("tutorialAnimation");
            _tutorialText2->setText("Nice work! Touch the cage in the cat form to complete the level.");
            _tutorialText2->setPositionX(250);
        }
        if (!_player->canPossess() && !_player->getPossess() && _player->getLevel() == 0) {
            _tutorialText2->setText("Oh no! You are stuck! Use the pause button on the top left to retry this level.");
            _tutorialText2->setPositionX(100);
            removeFromParentByName("tutorialAnimation");
            std::shared_ptr<Texture> arrow = _assets->get<Texture>("arrow");
            _tutorialAnimation = scene2::AnimationNode::alloc(arrow, 1, 2);
            _tutorialAnimation->setName("tutorialAnimation");
            _tutorialAnimation->setPosition(135, 425);
            _tutorialAnimation->setScale(0.25, 0.25);
            _tutorialAnimation->setAngle(3 * 0.785398);
            addChild(_tutorialAnimation);
            tutMaxFrame = 2;
            FRAME_SWITCH = 14;
            _tutorialAnimation->setPriority(10);
        }
    }
    else if (_showTutorialText == 3 && _player->getLevel() == 1){
        _tutorialText->setText("Patrolling enemies can be blocked by closed doors in their paths. Vision will also be blocked.");
        _tutorialText->setPositionX(25);
    }
    if (_showTutorialText == 1 || _showTutorialText == 2) {
        if (tutFrameSwitch > 0) {
            tutFrameSwitch--;
        }
        else {
            tutFrameSwitch = FRAME_SWITCH;
            tutFrame++;
            tutFrame = tutFrame % tutMaxFrame;
            _tutorialAnimation->setFrame(tutFrame);
        }
    }
    // Read input controller input
    _inputManager->readInput();
#ifdef CU_MOBILE
    _inputManager->updatePossessCoolDown(timestep);
#endif
    if (_hasControl) {
        if (_enemyController->closestEnemy() != nullptr && _player->canPossess()) {
            _enemyController->closestEnemy()->setGlow(true);
            if (_showTutorialText == 1) {
                if (USE_TAP_POSSESS) {
                    _tutorialText->setText("When in range to possess, the enemy will be highlighted. Double tap anywhere to possess!");
                    _tutorialText->setPositionX(25);
                }

                if (!USE_TAP_POSSESS) {
                    if (_tutorialAnimation->getPositionX() != 800 && _showTutorialText == 1) {
                        std::shared_ptr<Texture> upDownHand = _assets->get<Texture>("upDownHand");
                        _tutorialAnimation->setTexture(upDownHand);
                        _tutorialAnimation->setPosition(800, 300);
                    }
                }
                else {
                    if (_tutorialAnimation->getPositionX() != 850 && _showTutorialText == 1) {
                        removeFromParentByName("tutorialAnimation");
                        std::shared_ptr<Texture> tapHand = _assets->get<Texture>("tapHand");
                        _tutorialAnimation = scene2::AnimationNode::alloc(tapHand, 1, 5);
                        _tutorialAnimation->setName("tutorialAnimation");
                        addChild(_tutorialAnimation);
                        _tutorialAnimation->setPosition(850, 300);
                        _tutorialAnimation->setScale(0.25, 0.25);
                        tutMaxFrame = 4;
                        _tutorialAnimation->setPriority(10);
                    }

                }



            }

        }

        Size  size = Application::get()->getDisplaySize();
        float scale = GAME_WIDTH / size.width;
        size *= scale;
        Rect safe = Application::get()->getSafeBounds();
        safe.origin *= scale;
        safe.size *= scale;
        // Get the right and bottom offsets.
        float bOffset = safe.origin.y;
        float rOffset = (size.width) - (safe.origin.x + safe.size.width);

        if (_inputManager->getAndResetPossessPressed()) {
            if (_enemyController->closestEnemy() != nullptr && _player->canPossess()) {
                if (attemptPossess()) {
                    AudioEngine::get()->play("possess", _assets->get<Sound>("possess"));
                    //_possessPanel->getChildTexts()[0]->setText("x "+to_string(_player->get_nPossess()));
                    for (int i = _player->get_nPossess(); i < _possessPanel->getChildPanels().size() / 2; i++) {
                        _possessPanel->getChildPanels()[i * 2 + 1]->setVisible(false);
                    }
                    if (_showTutorialText == 1) {
                        if (_tutorialAnimation->getPositionX() != 662) {
                            _tutorialText->setText("You can open doors by clicking on them while possessing an enemy.");
                            _tutorialText->setPositionX(150);
                            removeFromParentByName("tutorialAnimation");
                            _rootScene->addChild(_tutorialAnimation);
                            _tutorialAnimation->setPosition(Vec2(662, 220));
                        }
                    }

                }
            }
        }
        else if (_inputManager->getAndResetUnpossessPressed()) {
            if (_player->get_possessEnemy() != nullptr && _player->getCurrentDen() + _player->getCurrentDoor() == 0) {
                unpossess();
            }
        }

        checkStaircaseDoors();
        checkDoors();
        checkCatDens();
        //checkEnemyPossession();
        collisions::checkForDoorCollision(_enemyController->getPossessed(), _enemyController->getEnemies(), _player, _doors);
        int cageCollision = collisions::checkForCagedAnimalCollision(_player, _cagedAnimal);
        if (cageCollision != 0 && _hasControl) {
            _hasControl = false;
            std::shared_ptr<Texture> unlockCagedAnimal = _assets->get<Texture>("UnlockCagedAnimal");
            int level = _cagedAnimal->getLevel();
            int pos = _cagedAnimal->getPos();
            int movingRight = _cagedAnimal->getMovingRight();
            _rootScene->removeChild(_cagedAnimal->getSceneNode());
            _cagedAnimal->SetSceneNode(Player::alloc(pos, level, 0, 7, unlockCagedAnimal)->getSceneNode());
            _player->getSceneNode()->setPositionX(_cagedAnimal->getPos() - 70);
            _cagedAnimal->getSceneNode()->setScale(-0.1605, 0.165);
            _cagedAnimal->setLevel(level);
            _cagedAnimal->getSceneNode()->setPosition(pos, level* FLOOR_HEIGHT + FLOOR_OFFSET-59);
            _cagedAnimal->PossessAnimation(4);
            _rootScene->addChild(_cagedAnimal->getSceneNode());
            
            std::function<bool()> winning = [&]() {
                _cagedAnimal->getSceneNode()->setVisible(false);
                setGameStatus(GameStatus::WIN);
                AudioEngine::get()->play("win", _assets->get<Sound>("winCondition"));
                _victoryPage = true;
                // shows win Panel
                _levelBoardPanel->getChildTexts()[0]->getLabel()->setVisible(true);
                _winPanel->setVisible(true);
                _winPanel->getChildButtons()["next"]->getButton()->activate();
                _winPanel->getChildButtons()["retry"]->getButton()->activate();
                _winPanel->getChildButtons()["toMenu"]->getButton()->activate();
                // and save the update to level_completed.json
                // first get all fields
                shared_ptr<JsonReader> reader = JsonReader::alloc(Application::get()->getSaveDirectory() + "completed_levels.json");
                std::shared_ptr<JsonValue> json = reader->readJson();
                shared_ptr<JsonValue> completed = json->get("completed");
                shared_ptr<JsonValue> result = JsonValue::allocObject();
                shared_ptr<JsonValue> r_complete = JsonValue::allocObject();
                for (int i = 0; i < MAX_LEVEL_PAGE * 10; i++) {
                    if (_levelIndex == i) {
                        r_complete->appendValue("level" + to_string(i + 1), true);
                    }
                    else {
                        r_complete->appendValue("level" + to_string(i + 1), completed->getBool("level" + to_string(i + 1)));
                    }
                }
                reader->close();
                result->appendChild("completed", r_complete);
                shared_ptr<JsonWriter> writer = JsonWriter::alloc(Application::get()->getSaveDirectory() + "completed_levels.json");

                writer->writeJson(result);
                writer->close();
                _hasControl = false;
                return false;
            };

            cugl::Application::get()->schedule(winning, 1500);
            
            
            
          
        }
        collisions::checkInBounds(_enemyController->getPossessed(), _player);

        /**possess code works a bit better when movement is processed last (scene node position is updated here)
            else you get one frame of wrong position*/
            // For now, if possessing, disable cat movement, put it to the same location as the possessed enemy
        if (_player->getPossess()) {
            float oldX = _player->getPos();
            _player->setPos(_player->get_possessEnemy()->getPos());
            _player->getSceneNode()->setPositionX(_player->get_possessEnemy()->getPos());
            float newX = _player->getPos();
            float deltaX = newX - oldX;
            _rootScene->setPositionX(_rootScene->getPositionX() - deltaX * _rootScene->getScale().x);
            centerCamera();

        }
        else if (_hasControl && _player->getCurrentDen() + _player->getCurrentDoor() == 0) {
            //EXPERIMENTAL LOCKED CAMERA CODE
            float oldX = _player->getSceneNode()->getPositionX();
            _player->move(_inputManager->getForward());
            float newX = _player->getSceneNode()->getPositionX();
            float deltaX = newX - oldX;
            _rootScene->setPositionX(_rootScene->getPositionX() - deltaX * _rootScene->getScale().x);
            centerCamera();

        }
#ifdef CU_MOBILE
        // move joystick for visualization
        _joystickPanel->getChildPanels()[0]->setPos(_joystickPanel->getSceneNode()->getPosition() / _joystickPanel->getSceneNode()->getScaleX() - _inputManager->getJoystickPosition() / 2.0f);
#endif
        // Enemy movement
        _enemyController->moveEnemies(_inputManager->getForward());
        _enemyController->findClosest(_player->getPos(), _player->getLevel(), closedDoors());
        if (_hasControl && _enemyController->detectedPlayer(_player->getPos(), _player->getLevel(), closedDoors())) {
            if (_player->getSceneNode()->isVisible() ||
                (_enemyController->getPossessed() != nullptr && _enemyController->getPossessed()->getSceneNode()->isVisible())) {
                _hasControl = false;
                std::shared_ptr<Texture> CagedCat = _assets->get<Texture>("CagedCat");
                if (_enemyController->getPossessed() != nullptr) {
                    std::shared_ptr<Texture> DetectingEnemy = _assets->get<Texture>("DetectingEnemy");
                    _enemyController->getDetectingEnemy()->setTexture(DetectingEnemy);
                    _enemyController->colorDetectingPlayer(_player->getPos(), _player->getLevel(), closedDoors());
                    _hasControl = false;
                    _enemyController->getPossessed()->getSceneNode()->setVisible(false);
                    _rootScene->removeChild(_player->getSceneNode());
                    _player->SetSceneNode(Player::alloc(_enemyController->getPossessed()->getPos(), _enemyController->getPossessed()->getLevel(), 0, 6,
                        CagedCat)->getSceneNode());
                    _player->setLevel(_enemyController->getPossessed()->getLevel());
                    _player->getSceneNode()->setPosition(_player->getPos(), _enemyController->getPossessed()->getLevel() * FLOOR_HEIGHT + FLOOR_OFFSET - 50);
                    if (_enemyController->getPossessed()->getMovingRight()) {
                        _player->getSceneNode()->setScale(0.06, 0.06);
                    }
                    else {
                        _player->getSceneNode()->setScale(-0.06, 0.06);
                    }
                    _player->PossessAnimation(3);
                    _rootScene->addChild(_player->getSceneNode());
                    std::function<bool()> losing = [&]() {

                        _player->getSceneNode()->setVisible(false);
                        setGameStatus(GameStatus::LOSE);
                        if (_showTutorialText > 0 && _tutorialAnimation != nullptr) {
                            _tutorialAnimation->setVisible(false);
                        }
                        _levelBoardPanel->getChildPanels()[1]->setVisible(true);
                        _losePanel->setVisible(true);
                        _losePanel->getChildButtons()["retry"]->getButton()->activate();
                        _losePanel->getChildButtons()["toMenu"]->getButton()->activate();
                        if (_showTutorialText == 1) {
                            _tutorialText->setText("Oh no! You got caught!");
                            _tutorialText->setPosition(Vec2(100, 110));
                            _tutorialText2->setVisible(false);
                        }
                        _hasControl = false;
                        return false;
                    };
                    cugl::Application::get()->schedule(losing, 1500);
                }
                else {
                    std::shared_ptr<Texture> DetectingEnemy = _assets->get<Texture>("DetectingEnemy");
                    _enemyController->getDetectingEnemy()->setTexture(DetectingEnemy);
                    _enemyController->colorDetectingPlayer(_player->getPos(), _player->getLevel(), closedDoors());
                    int level = _player->getLevel();
                    int pos = _player->getPos();
                    int movingRight = _player->getMovingRight();
                    _rootScene->removeChild(_player->getSceneNode());
                    _player->SetSceneNode(Player::alloc(pos, level, 0, 6, CagedCat)->getSceneNode());
                    _player->setLevel(level);
                    _player->getSceneNode()->setPosition(pos, level * FLOOR_HEIGHT + FLOOR_OFFSET - 50);
                    if (movingRight) {
                        _player->getSceneNode()->setScale(0.06, 0.06);
                    }
                    else {
                        _player->getSceneNode()->setScale(-0.06, 0.06);
                    }
                    _player->PossessAnimation(3);
                    _rootScene->addChild(_player->getSceneNode());
                 
                    std::function<bool()> losing = [&]() {
                        _player->getSceneNode()->setVisible(false);
                        setGameStatus(GameStatus::LOSE);
                        if (_showTutorialText > 0) {
                            if (_tutorialAnimation != nullptr) {
                                _tutorialAnimation->setVisible(false);
                            }
                        }
                        _levelBoardPanel->getChildPanels()[1]->setVisible(true);
                        _losePanel->setVisible(true);
                        _losePanel->getChildButtons()["retry"]->getButton()->activate();
                        _losePanel->getChildButtons()["toMenu"]->getButton()->activate();
                        if (_showTutorialText == 1) {
                            _tutorialText->setText("Oh no! You got caught!");
                            _tutorialText->setPosition(Vec2(100, 110));
                            _tutorialText2->setVisible(false);
                        }
                        _hasControl = false;
                        return false;
                    };
                   

                    cugl::Application::get()->schedule(losing, 1500);
                }


            }
        }
        else {
            _player->getSceneNode()->setAngle(0);
            if (_enemyController->getPossessed() != nullptr) {
                _enemyController->getPossessed()->getSceneNode()->setAngle(0);
            }
        }


        //tutorial text trigger
        if (!_player->canPossess() && !_player->getPossess() && _player->getLevel() == 0) {
            /*if (_showTutorialText) {
                _tutorialText->setText("Oh no! You are stuck! Use the top left pause button to retry");
                _tutorialText->setPosition(Vec2(100, 110));
                _tutorialText2->setText("The paw on the top right indicates the number of possessions you have remaining.");
                _tutorialText2->setPosition(Vec2(100, 420));
            }*/
        }
    }

    //resetting
    if (_inputManager->didReset()) {
        reset();
    }
}

bool GameplayMode::attemptPossess() {
#ifdef CU_MOBILE
    if (_inputManager->isInCoolDown()) return false;
#endif
    std::shared_ptr<Enemy> enemy = _enemyController->closestEnemy();
    if (enemy != nullptr) {
        _hasControl = false;
        _player->setPossess(true);
        _player->set_possessEnemy(_enemyController->closestEnemy());
        _enemyController->updatePossessed(_enemyController->closestEnemy());
        //        _rootScene->removeChild(_enemyController->closestEnemy()->getPatrolNode());
        _enemyController->closestEnemy()->setGlow(false);

        std::shared_ptr<Texture> catJump = _assets->get<Texture>("catPossessing");
        _rootScene->removeChild(_player->getSceneNode());
        _player->SetSceneNode(Player::alloc(150, 0, 0, 8, catJump)->getSceneNode());
        _player->getSceneNode()->setPosition(_player->getPos() + 50, _player->getLevel() * FLOOR_HEIGHT + FLOOR_OFFSET);
        if (_player->getMovingRight()) {
            _player->getSceneNode()->setScale(0.15, 0.15);
        }
        else {
            _player->getSceneNode()->setScale(-0.15, 0.15);
        }

        _rootScene->addChild(_player->getSceneNode());
        _player->PossessAnimation(0);

        std::function<bool()> setPossessed = [&]() {
            _player->getSceneNode()->setVisible(false);
            _enemyController->closestEnemy()->setAsPossessed();
            std::shared_ptr<scene2::PolygonNode> keyCircle = _enemyController->closestEnemy()->getKeyCircle();
            _enemyController->closestEnemy()->getSceneNode()->removeAllChildren();
            _enemyController->closestEnemy()->getSceneNode()->addChild(keyCircle);
            _hasControl = true;

            return false;
        };
        cugl::Application::get()->schedule(setPossessed, 500);
#ifdef CU_MOBILE
        _inputManager->resetPossessCoolDown();
#endif
        return true;

        //code used for cat jumping animation, incomplete and not activated in our release
    }
    return false;
}

void GameplayMode::unpossess() {
#ifdef CU_MOBILE
    if (_inputManager->isInCoolDown()) return;
#endif
    std::shared_ptr<Enemy> enemy = _enemyController->getPossessed();
    if (enemy == nullptr) return;
    _hasControl = false;
    AudioEngine::get()->play("unpossess", _assets->get<Sound>("jumpOff"));
    _player->setLevel(enemy->getLevel());
    _player->getSceneNode()->setVisible(true);
    int cageCollision = collisions::checkForCagedAnimalCollision(_player, _cagedAnimal);
    if (cageCollision != 0) {
        return;
    }
    int level = _player->getLevel();
    std::shared_ptr<Texture> enemyDying = _assets->get<Texture>("EnemyDying");
    _rootScene->removeChild(_player->getSceneNode());
    bool movingRight = enemy->getMovingRight();
    if (enemy->getMovingRight()) {
        _player->SetSceneNode(Player::alloc(150, 0, 270, 9, enemyDying)->getSceneNode());
        _player->getSceneNode()->setScale(Vec2(1, -1));
        _player->getSceneNode()->setScale(0.3, 0.3);
    }
    else {
        _player->SetSceneNode(Player::alloc(150, 0, 270, 9, enemyDying)->getSceneNode());
        _player->getSceneNode()->setScale(Vec2(1, -1));
        _player->getSceneNode()->setScale(-0.3, 0.3);
    }
    _player->setLevel(level);
    //_player->getSceneNode()->setPositionY(_player->getSceneNode()->getPositionY() + 50);

    _rootScene->addChild(_player->getSceneNode());

    _player->EnemyDying();
    _player->setPossess(false);
    //_player->setPos((enemy->getPos()));


   /*int level = enemy->getLevel();
   int pos = enemy->getPos();*/
    enemy->getSceneNode()->setVisible(false);
    enemy->dispose();
    _enemyController->removeEnemy(enemy);
    _enemyController->updatePossessed(nullptr);
    _player->getSceneNode()->setPosition(_player->getPos(), _player->getLevel() * FLOOR_HEIGHT + FLOOR_OFFSET + 50);


    std::function<bool()> delayInputRight = [&]() {
        int level = _player->getLevel();
        std::shared_ptr<Texture> catJump = _assets->get<Texture>("cat-walking");
        _rootScene->removeChild(_player->getSceneNode());
        _player->SetSceneNode(Player::alloc(150, 0, 0, 8, catJump)->getSceneNode());
        _player->setPos(_player->getPos() + 55);
        _player->setLevel(level);
        _player->getSceneNode()->setScale(0.15, 0.15);
        _rootScene->addChild(_player->getSceneNode());
        _hasControl = true;

        return false;
    };
    std::function<bool()> delayInputLeft = [&]() {
        int level = _player->getLevel();
        std::shared_ptr<Texture> catJump = _assets->get<Texture>("cat-walking");
        _rootScene->removeChild(_player->getSceneNode());
        _player->SetSceneNode(Player::alloc(150, 0, 0, 8, catJump)->getSceneNode());
        _player->setPos(_player->getPos() - 55);
        _player->setLevel(level);
        _player->getSceneNode()->setScale(-0.15, 0.15);
        _rootScene->addChild(_player->getSceneNode());
        _hasControl = true;

        return false;
    };
    if (movingRight) {
        cugl::Application::get()->schedule(delayInputRight, 1100);
    }
    else {
        cugl::Application::get()->schedule(delayInputLeft, 1100);
    }
    //_player->getSceneNode()->setFrame(7);
#ifdef CU_MOBILE
    _inputManager->resetPossessCoolDown();
#endif
}

/**
 * The method called to draw the application to the screen.
 *
 * This is your core loop and should be replaced with your custom implementation.
 * This method should OpenGL and related drawing calls.
 *
 * When overriding this method, you do not need to call the parent method
 * at all. The default implmentation does nothing.
 */
void GameplayMode::draw() {
    // This takes care of begin/end
    render(_batch);
}

void GameplayMode::clearRootSceneNode() {
    Size size = Application::get()->getDisplaySize();
    _rootScene = scene2::OrderedNode::allocWithOrder(scene2::OrderedNode::Order::PRE_ASCEND);
    _rootScene->setAnchor(Vec2::ANCHOR_CENTER);
    _rootScene->setContentSize(size);
}

/**
 * Internal helper to build the scene graph.
 *
 * Scene graphs are not required.  You could manage all scenes just like
 * you do in 3152.  However, they greatly simplify scene management, and
 * have become standard in most game engines.
 */


void GameplayMode::buildScene(std::shared_ptr<JsonValue> json) {

    //clearRootSceneNode();
    Size  size = Application::get()->getDisplaySize();
    float scale = GAME_WIDTH / size.width;
    size *= scale;

    _hasControl = true;
    _doors.clear();
    _doorFrames.clear();
    _decorations.clear();
    _staircaseDoors.clear();
    _catDens.clear();

    std::shared_ptr<Texture> cat = _assets->get<Texture>("cat-walking");
    //floor texture creation
    std::shared_ptr<Texture> wall = _assets->get<Texture>("levelWall");

    std::shared_ptr<Texture> floor = _assets->get<Texture>("levelFloor");
    //Staircase door texture creation
    std::shared_ptr<Texture> orangeStaircaseDoor = _assets->get<Texture>("staircaseDoorOrange");
    std::shared_ptr<Texture> redStaircaseDoor = _assets->get<Texture>("staircaseDoorRed");
    std::shared_ptr<Texture> purpleStaircaseDoor = _assets->get<Texture>("staircaseDoorPurple");
    std::shared_ptr<Texture> yellowStaircaseDoor = _assets->get<Texture>("staircaseDoorYellow");
    //Cat den texture creation
    std::shared_ptr<Texture> catden = _assets->get<Texture>("catDen");
    std::shared_ptr<Texture> catDenGreen = _assets->get<Texture>("catDenGreen");
    std::shared_ptr<Texture> catDenPurple = _assets->get<Texture>("catDenPurple");
    std::shared_ptr<Texture> catDenBlue = _assets->get<Texture>("catDenBlue");
    std::shared_ptr<Texture> catDenGrey = _assets->get<Texture>("catDenGrey");
    //Door texture creation
    std::shared_ptr<Texture> door = _assets->get<Texture>("door");
    std::shared_ptr<Texture> BlueLockedDoor = _assets->get<Texture>("BlueDoorLock");
    std::shared_ptr<Texture> RedLockedDoor = _assets->get<Texture>("RedDoorLock");
    std::shared_ptr<Texture> YellowLockedDoor = _assets->get<Texture>("YellowDoorLock");
    std::shared_ptr<Texture> GreenLockedDoor = _assets->get<Texture>("GreenDoorLock");

    std::shared_ptr<Texture> doorFrame = _assets->get<Texture>("doorFrame");
    std::shared_ptr<Texture> coats1 = _assets->get<Texture>("labcoat1");
    std::shared_ptr<Texture> coats2 = _assets->get<Texture>("labcoat2");
    std::shared_ptr<Texture> coats3 = _assets->get<Texture>("labcoat3");
    std::shared_ptr<Texture> shower = _assets->get<Texture>("shower");
    //caged animal
    std::shared_ptr<Texture> cagedAnimal = _assets->get<Texture>("cagedAnimal");
    // Enemy creation
    _enemyController = make_shared<EnemyController>();
    enemyTexture = _assets->get<Texture>("enemy");
    std::shared_ptr<Texture> altTexture = _assets->get<Texture>("possessed-enemy");
    enemyHighlightTexture = _assets->get<Texture>("enemy-glow");
    tableTexture = _assets->get<Texture>("lab-table");
    std::shared_ptr<Texture> visionTexture = _assets->get<Texture>("vision-cone");
    std::shared_ptr<Texture> RedKey = _assets->get<Texture>("RedKey");
    std::shared_ptr<Texture> GreenKey = _assets->get<Texture>("GreenKey");
    std::shared_ptr<Texture> YellowKey = _assets->get<Texture>("YellowKey");
    std::shared_ptr<Texture> BlueKey = _assets->get<Texture>("BlueKey");



    //JSON PROCESSING
    shared_ptr<JsonValue> playerJSON = json->get("player");
    shared_ptr<JsonValue> enemiesJSON = json->get("enemy");
    shared_ptr<JsonValue> staircaseDoorJSON = json->get("staircase-door");
    shared_ptr<JsonValue> doorJSON = json->get("door");
    shared_ptr<JsonValue> decorationsJSON = json->get("decorations");
    shared_ptr<JsonValue> objectTemp;
    if (playerJSON != nullptr) {
        string numPossessions = to_string(playerJSON->getInt("num_possessions"));
        _player = Player::alloc(playerJSON->getFloat("x_pos"), playerJSON->getInt("level"), 0, 8, cat);
        _player->set_nPossess(playerJSON->getFloat("num_possessions"));

    }
    if (enemiesJSON != nullptr) {
        for (int i = 0; i < enemiesJSON->size(); i++) {
            objectTemp = enemiesJSON->get(i);
            shared_ptr<JsonValue> keyArray = objectTemp->get("keyInt");
            vector<int> key = {};
            if (!keyArray->isNull()) {
                for (int j = 0; j < keyArray->size(); j++) {
                    key.push_back(stoi(keyArray->get(j)->toString()));
                }
            }
            _enemyController->addEnemy(objectTemp->getFloat("x_pos"), objectTemp->getInt("level"), 0,
                key, objectTemp->getFloat("patrol_start"), objectTemp->getFloat("patrol_end"), 5, 
                enemyTexture, altTexture, enemyHighlightTexture, tableTexture, visionTexture, RedKey, BlueKey, YellowKey, GreenKey);
            if (objectTemp->getBool("possessed")) {
                _player->setPos(objectTemp->getFloat("x_pos"));
                _player->setLevel(objectTemp->getInt("level"));
                _player->getSceneNode()->setVisible(false);

                _player->setPossess(true);
                _player->set_possessEnemy(_enemyController->getEnemies().back());
                _player->get_possessEnemy()->setAsPossessed();
                _enemyController->updatePossessed(_player->get_possessEnemy());
            }
        }
    }
    if (staircaseDoorJSON != nullptr) {
        for (int i = 0; i < staircaseDoorJSON->size(); i++) {
            objectTemp = staircaseDoorJSON->get(i);
            if (objectTemp->getBool("isDen")) {
                _catDens.push_back(CatDen::alloc(objectTemp->getFloat("x_pos"), 0, Vec2(0.05, 0.05), objectTemp->getInt("level"),
                    cugl::Color4::WHITE, objectTemp->getInt("connection"), 1, 1, catden, catDenGreen, catDenPurple, catDenBlue, catDenGrey));
            }
            else {
                _staircaseDoors.push_back(StaircaseDoor::alloc(objectTemp->getFloat("x_pos"), 0, Vec2(1, 1), objectTemp->getInt("level"),
                    cugl::Color4::WHITE, { 1 }, objectTemp->getInt("connection"), 1, 8, orangeStaircaseDoor,
                    redStaircaseDoor, purpleStaircaseDoor, yellowStaircaseDoor, orangeStaircaseDoor));
            }

        }
    }
    if (doorJSON != nullptr) {
        for (int i = 0; i < doorJSON->size(); i++) {
            objectTemp = doorJSON->get(i);
            shared_ptr<JsonValue> keyArray = objectTemp->get("keyInt");
            vector<int> key = {};
            if (!keyArray->isNull()) {
                for (int j = 0; j < keyArray->size(); j++) {
                    key.push_back(stoi(keyArray->get(j)->toString()));
                }
            }
            shared_ptr<Door> temp = Door::alloc(objectTemp->getFloat("x_pos"), 0, Vec2(1, 1), objectTemp->getInt("level"),
                cugl::Color4::WHITE, key, 1, 8, door, GreenLockedDoor, YellowLockedDoor, RedLockedDoor, BlueLockedDoor);
            if (objectTemp->getBool("isOpen")) {
                temp->getDoorLock()->setVisible(false);
                temp->setUnlocked(true);
                temp->setDoor(true);
            }
            _doors.push_back(temp);
            _doorFrames.push_back(DoorFrame::alloc(objectTemp->getFloat("x_pos") - 77, 0, Vec2(1.0, 1), objectTemp->getInt("level"), cugl::Color4::WHITE, { 1 }, 1, 8, doorFrame));
            int random = rand() % 100;
            if (temp->getKeys().size() > 0) {
                random = 99;
            }
            shared_ptr<scene2::PolygonNode> deco;
            if (random < 10) {
                deco = scene2::PolygonNode::allocWithTexture(shower);
                deco->setScale(Vec2(0.25, 0.25));
                deco->setPosition(temp->getPos().x + 150, temp->getSceneNode()->getPositionY());
                _decorations.push_back(deco);
            }
            else if (random < 25) {
                deco = scene2::PolygonNode::allocWithTexture(coats1);
                deco->setScale(Vec2(0.25, 0.25));
                deco->setPosition(temp->getPos().x + 150, temp->getSceneNode()->getPositionY());
                _decorations.push_back(deco);
            }
            else if (random < 40) {
                deco = scene2::PolygonNode::allocWithTexture(coats2);
                deco->setScale(Vec2(0.25, 0.25));
                deco->setPosition(temp->getPos().x + 150, temp->getSceneNode()->getPositionY());
                _decorations.push_back(deco);
            }
            else if (random < 55) {
                deco = scene2::PolygonNode::allocWithTexture(coats3);
                deco->setScale(Vec2(0.25, 0.25));
                deco->setPosition(temp->getPos().x + 150, temp->getSceneNode()->getPositionY());
                _decorations.push_back(deco);
            }

            
        }
    }
    if (decorationsJSON != nullptr) {
        for (int i = 0; i < decorationsJSON->size(); i++) {
            objectTemp = decorationsJSON->get(i);
            _cagedAnimal = Player::alloc(objectTemp->getFloat("x_pos"), objectTemp->getInt("level"), 0, 1,
                cagedAnimal);
            _cagedAnimal->getSceneNode()->setScale(Vec2(-0.3, 0.3));

        }
    }




    Rect safe = Application::get()->getSafeBounds();
    safe.origin *= scale;
    safe.size *= scale;


    float bOffset = safe.origin.y;
    float rOffset = (size.width) - (safe.origin.x + safe.size.width);


    vector<std::shared_ptr<Enemy>> enemies = _enemyController->getEnemies();

    _player->getSceneNode()->setName("Player");

    //_rootScene->addChild(_level1Wall->getSceneNode());
    //_rootScene->addChild(_level2Wall->getSceneNode());
    //_level1Floor->getSceneNode()->setName("Level1Floor");
    //_level2Floor->getSceneNode()->setName("Level2Floor");
    //_rootScene->addChild(_level1Floor->getSceneNode());
    //_rootScene->addChild(_level2Floor->getSceneNode());

    _numFloors = json->getInt("floor");
    int s = 1.4;
    shared_ptr<Wall> tempwall;
    shared_ptr<Floor> tempfloor;
    for (int i = 0; i < json->getInt("floor"); i++) {
        tempwall = Wall::alloc(550, 0, Vec2(s, s), i, cugl::Color4::WHITE, 1, 1, wall);
        tempwall->setLevel(i);
        _rootScene->addChild(tempwall->getSceneNode());
        tempfloor = Floor::alloc(555, 0, Vec2(s, s), i, cugl::Color4::WHITE, 1, 1, floor);
        _rootScene->addChild(tempfloor->getSceneNode());
        tempfloor->setLevel(i);
    }

    for (auto it = begin(_staircaseDoors); it != end(_staircaseDoors); ++it) {
        _rootScene->addChild(it->get()->getSceneNode());
    }
    for (auto it = begin(_catDens); it != end(_catDens); ++it) {
        _rootScene->addChild(it->get()->getSceneNode());
    }
    for (auto it = begin(_doors); it != end(_doors); ++it) {
        _rootScene->addChild(it->get()->getSceneNode());
    }
    for (auto it = begin(_decorations); it != end(_decorations); ++it) {
        _rootScene->addChild(*it);
    }
    for (auto it = begin(enemies); it != end(enemies); ++it) {
        if (it->get()->getStartTableNode() != nullptr) {
            _rootScene->addChild(it->get()->getStartTableNode());
        }
        if (it->get()->getEndTableNode() != nullptr) {
            _rootScene->addChild(it->get()->getEndTableNode());
        }
        if (it->get()->getPatrolNode() != nullptr) {
            _rootScene->addChild(it->get()->getPatrolNode());
        }
    }
    _rootScene->addChild(_cagedAnimal->getSceneNode());
    for (auto it = begin(enemies); it != end(enemies); ++it) {
        _rootScene->addChild(it->get()->getSceneNode());
    }


    _rootScene->addChild(_player->getSceneNode());
    for (auto it = begin(_doorFrames); it != end(_doorFrames); ++it) {
        _rootScene->addChild(it->get()->getSceneNode());
    }




    std::shared_ptr<Font> font = _assets->get<Font>("futura");
    if (_showTutorialText == 3) {
        _tutorialText = scene2::Label::alloc("Patrolling enemies move between their tables.", font);
        _tutorialText->setForeground(Color4::WHITE);
        _tutorialText->setScale(Vec2(0.5, 0.5));
        _tutorialText->setPosition(Vec2(325, 20));
    }
    else if (_showTutorialText == 4) {
        _tutorialText = scene2::Label::alloc("Some doors are locked. Circles beneath enemies represent a key of the corresponding color.", font);
        _tutorialText->setForeground(Color4::WHITE);
        _tutorialText->setScale(Vec2(0.5, 0.5));
        _tutorialText->setPosition(Vec2(25, 20));
    }
    else {
        _tutorialText = scene2::Label::alloc("Use the bottom left joystick to move!", font);
        _tutorialText->setForeground(Color4::WHITE);
        _tutorialText->setScale(Vec2(0.5, 0.5));
        _tutorialText->setPosition(Vec2(350, 20));
        _tutorialText->setVisible(_showTutorialText == 1);
    }

    _tutorialText2 = scene2::Label::alloc("Tap on the holes in the walls in cat form to enter cat dens!", font);
    _tutorialText2->setForeground(Color4::WHITE);
    _tutorialText2->setScale(Vec2(0.5, 0.5));
    _tutorialText2->setPosition(Vec2(200, 20));
    _tutorialText2->setVisible(_showTutorialText == 2);

    if (_showTutorialText == 1) {
        std::shared_ptr<Texture> leftRightHand = _assets->get<Texture>("lrHand");
        _tutorialAnimation = scene2::AnimationNode::alloc(leftRightHand, 1, 13);
        _tutorialAnimation->setPosition(Vec2(100, 175));
        _tutorialAnimation->setScale(0.25, -0.25);
        _tutorialAnimation->setName("tutorialAnimation");
        tutMaxFrame = 12;
    }
    else if (_showTutorialText == 2) {
        std::shared_ptr<Texture> tapHand = _assets->get<Texture>("tapHand");
        _tutorialAnimation = scene2::AnimationNode::alloc(tapHand, 1, 5);
        _tutorialAnimation->setName("tutorialAnimation");
        _tutorialAnimation->setPosition(Vec2(-190, 175));
        _tutorialAnimation->setScale(0.25, 0.25);
        tutMaxFrame = 5;
    }
    addChild(_rootScene);
    addChild(_tutorialText);
    addChild(_tutorialText2);
#ifdef CU_MOBILE
    // make joystick panel
    _joystickPanel = ui::PanelElement::alloc(0, 0, 0, _assets->get<Texture>("joystickBorder"));
    _joystickPanel->getSceneNode()->setScale(1.0f);
    _joystickPanel->getSceneNode()->setPosition(_assets->get<Texture>("joystickBorder")->getWidth() * _joystickPanel->getSceneNode()->getScaleX() / 2.0f, _assets->get<Texture>("joystickBorder")->getHeight() * _joystickPanel->getSceneNode()->getScaleY() / 2.0f);
    _joystickPanel->createChildPanel(0, 0, 0, _assets->get<Texture>("joystick"));
    addChild(_joystickPanel->getSceneNode());
#endif

    // make possess panel
    _possessPanel = ui::PanelElement::alloc(0, 0, 0, _assets->get<Texture>("possessCounter"));
    _possessPanel->getSceneNode()->setAnchor(Vec2::ANCHOR_TOP_RIGHT);
    _possessPanel->getSceneNode()->setScale(1.0f);
    _possessPanel->getSceneNode()->setPosition(size);

    std::shared_ptr<Texture> possessUsed = _assets->get<Texture>("usedPossession");
    std::shared_ptr<Texture> possessAvailable = _assets->get<Texture>("availablePossession");
    for (int i = 0; i < _player->get_nPossess(); i++) {
        _possessPanel->createChildPanel(0, 0, 0, possessUsed);
        _possessPanel->getChildPanels()[i * 2]->getSceneNode()->setAnchor(Vec2::ANCHOR_TOP_RIGHT);
        _possessPanel->getChildPanels()[i * 2]->getSceneNode()->setScale(0.3f);
        _possessPanel->getChildPanels()[i * 2]->setPos(_possessPanel->getSceneNode()->getContentSize()
            * _possessPanel->getSceneNode()->getScaleX() - Vec2(i * (possessAvailable->getSize().width + 20.0f) * _possessPanel->getChildPanels()[0]->getSceneNode()->getScaleX() + 20.0f, 20.0f));

        _possessPanel->createChildPanel(0, 0, 0, possessAvailable);
        _possessPanel->getChildPanels()[i * 2 + 1]->getSceneNode()->setAnchor(Vec2::ANCHOR_TOP_RIGHT);
        _possessPanel->getChildPanels()[i * 2 + 1]->getSceneNode()->setScale(0.3f);
        _possessPanel->getChildPanels()[i * 2 + 1]->setPos(_possessPanel->getSceneNode()->getContentSize()
            * _possessPanel->getSceneNode()->getScaleX() - Vec2(i * (possessAvailable->getSize().width + 20.0f) * _possessPanel->getChildPanels()[1]->getSceneNode()->getScaleX() + 20.0f, 20.0f));
    }
    addChild(_possessPanel->getSceneNode());

    _menuButton = ui::ButtonElement::alloc(0, 0, 0, 0, ui::ButtonState::AVAILABLE);
    _menuButton->setTexture(_assets->get<Texture>("menuButton"));
    _menuButton->getButton()->setAnchor(Vec2::ANCHOR_TOP_LEFT);
    _menuButton->getButton()->setScale(0.75f);
    _menuButton->getButton()->setPosition(15.0f, size.height - 15.0f);
    _menuButton->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            setGameStatus(GameStatus::PAUSED);
        }
        });
    _menuButton->getButton()->activate();
    addChild(_menuButton->getButton());

    // Create Menu Panel
    std::shared_ptr<Texture> pausedBackground = _assets->get<Texture>("pausedBackground");
    _menuPanel = ui::PanelElement::alloc(size.width / 2, size.height / 2, 0, pausedBackground);
    _menuPanel->getSceneNode()->setScale(max(size.width / pausedBackground->getSize().width, size.height / pausedBackground->getSize().height));
    _menuPanel->setVisible(false);
    _menuPanel->createChildButton(0, 60, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("resume"), Color4f::WHITE,"resume");
    _menuPanel->getChildButtons()["resume"]->getButton()->setName("resume");
    _menuPanel->getChildButtons()["resume"]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            if (_showTutorialText > 0&& _tutorialAnimation != nullptr) {
                _tutorialAnimation->setVisible(true);
            }
            _levelBoardPanel->getChildPanels()[0]->setVisible(false);
            setGameStatus(GameStatus::RUNNING);
        }
        });
    _menuPanel->createChildButton(0, -45, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("restartLevel"), Color4f::WHITE,"restart");
    _menuPanel->getChildButtons()["restart"]->getButton()->setName("restart");
    _menuPanel->getChildButtons()["restart"]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _reset = true;
        }
        });
    _menuPanel->createChildButton(0, -150, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("exit"), Color4f::WHITE,"exit");
    _menuPanel->getChildButtons()["exit"]->getButton()->setName("exit");
    _menuPanel->getChildButtons()["exit"]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _backToMenu = true;
        }
        });
    _menuPanel->createChildButton((size.width - 100.0f) / 2.0f / _menuPanel->getSceneNode()->getScaleX(), (size.height - 100.0f) / 2.0f / _menuPanel->getSceneNode()->getScaleY(), 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("unmute"), Color4f::WHITE, "muteButton");
    _menuPanel->getChildButtons()["muteButton"]->getButton()->setScale(Vec2(0.8f, 0.8f));
    _menuPanel->getChildButtons()["muteButton"]->getButton()->setName("muteButton");
    _menuPanel->getChildButtons()["muteButton"]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            _gameMuted = true;
        }
        });
    _menuPanel->getChildButtons()["muteButton"]->getButton()->setVisible(true);
    _menuPanel->getChildButtons()["muteButton"]->getButton()->activate();
    _menuPanel->createChildButton((size.width - 100.0f) / 2.0f / _menuPanel->getSceneNode()->getScaleX(), (size.height - 100.0f) / 2.0f / _menuPanel->getSceneNode()->getScaleY(), 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("mute"), Color4f::WHITE, "unmuteButton");
    _menuPanel->getChildButtons()["unmuteButton"]->getButton()->setScale(Vec2(0.8f, 0.8f));
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

    // Create Win Panel
    winPanel = _assets->get<Texture>("levelCompleteBG");
    _winPanel = ui::PanelElement::alloc(size.width / 2, size.height / 2, 0, winPanel);
    _winPanel->getSceneNode()->setScale(max(size.width / winPanel->getSize().width, size.height / winPanel->getSize().height));
    _winPanel->setVisible(false);
    _winPanel->createChildButton((size.width - 300) / 2.0f / _winPanel->getSceneNode()->getScaleX(), (-size.height + 150) / 2.0f / _winPanel->getSceneNode()->getScaleY(), 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("nextLevel"), Color4f::WHITE,"next");
    _winPanel->getChildButtons()["next"]->getButton()->setScale(1.5f);
    _winPanel->getChildButtons()["next"]->getButton()->setName("next");
    _winPanel->getChildButtons()["next"]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _nextLevel = true;
            _victoryPage = false;
            if (!_gameMuted) {
                std::shared_ptr<AudioQueue> audioQueue = AudioEngine::get()->getMusicQueue();
                audioQueue->setVolume(1.0f);
            }
        }
        });
    _winPanel->createChildButton((-size.width + 300) / 2.0f / _winPanel->getSceneNode()->getScaleX(), (-size.height + 150) / 2.0f / _winPanel->getSceneNode()->getScaleY(), 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("tryAgain"), Color4f::WHITE,"retry");
    _winPanel->getChildButtons()["retry"]->getButton()->setScale(1.5f);
    _winPanel->getChildButtons()["retry"]->getButton()->setName("retry");
    _winPanel->getChildButtons()["retry"]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _reset = true;
            _victoryPage = false;
            if (!_gameMuted) {
                std::shared_ptr<AudioQueue> audioQueue = AudioEngine::get()->getMusicQueue();
                audioQueue->setVolume(1.0f);
            }
        }
        });
    _winPanel->createChildButton((-size.width + 300) / 2.0f / _winPanel->getSceneNode()->getScaleX(), (size.height - 150) / 2.0f / _winPanel->getSceneNode()->getScaleY(), 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("returnToMenu"), Color4f::WHITE,"toMenu");
    _winPanel->getChildButtons()["toMenu"]->getButton()->setScale(1.5f);
    _winPanel->getChildButtons()["toMenu"]->getButton()->setName("toMenu");
    _winPanel->getChildButtons()["toMenu"]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _backToMenu = true;
            _victoryPage = false;
            if (!_gameMuted) {
                std::shared_ptr<AudioQueue> audioQueue = AudioEngine::get()->getMusicQueue();
                audioQueue->setVolume(1.0f);
            }
        }
        });
    addChild(_winPanel->getSceneNode());

    // Create Lose Panel
    losePanel = _assets->get<Texture>("levelFailedBG");
    _losePanel = ui::PanelElement::alloc(size.width / 2, size.height / 2, 0, losePanel);
    _losePanel->getSceneNode()->setScale(max(size.width / losePanel->getSize().width, size.height / losePanel->getSize().height));
    _losePanel->setVisible(false);
    _losePanel->createChildButton(230, -350, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("retry"), Color4f::WHITE,"retry");
    _losePanel->getChildButtons()["retry"]->getButton()->setName("retry");
    _losePanel->getChildButtons()["retry"]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _reset = true;
        }
        });
    _losePanel->createChildButton(230, -850, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("menu"), Color4f::WHITE,"toMenu");
    _losePanel->getChildButtons()["toMenu"]->getButton()->setName("toMenu");
    _losePanel->getChildButtons()["toMenu"]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _backToMenu = true;
        }
        });
    addChild(_losePanel->getSceneNode());

    // create the level indicator panel
    _levelBoardPanel = ui::PanelElement::alloc(size.width / 2, size.height / 2, 0, nullptr);
    _levelBoardPanel->getSceneNode()->setScale(1.0f);
    _levelBoardPanel->createChildPanel(0, size.height / 2.0f - 50.0f,0, _assets->get<Texture>("level"+to_string(_levelIndex + 1)+"Board"));
    _levelBoardPanel->getChildPanels()[0]->getSceneNode()->setScale(0.8f);
    _levelBoardPanel->getChildPanels()[0]->setVisible(false);
    _levelBoardPanel->createChildPanel(50.0f, size.height / 2.0f - 100.0f, 0, _assets->get<Texture>("level" + to_string(_levelIndex + 1) + "Board"));
    _levelBoardPanel->getChildPanels()[1]->getSceneNode()->setScale(1.0f);
    _levelBoardPanel->getChildPanels()[1]->setVisible(false);
    _levelBoardPanel->createChildText(0, size.height / 2.0f - 100.0f, 100, 50, "Level " + to_string(_levelIndex + 1), _assets->get<Font>("futura"));
    _levelBoardPanel->getChildTexts()[0]->getLabel()->setVisible(false);
    _levelBoardPanel->getChildTexts()[0]->getLabel()->setScale(0.9f);
    addChild(_levelBoardPanel->getSceneNode());
    // Initialize input manager
    _inputManager->init(_player, _rootScene, getBounds());
    _rootScene->setScale(Vec2(0.6f, 0.6f));

    if (_showTutorialText == 1) {
        addChild(_tutorialAnimation);
    }
    else if (_showTutorialText == 2) {
        _rootScene->addChild(_tutorialAnimation);
        _tutorialAnimation->setPriority(100);
    }
    centerCamera();
}


void GameplayMode::checkDoors() {
//CULog("player pos %f",_player->getSceneNode()->getWorldPosition().y);
    for (shared_ptr<Door> door : _doors) {
        bool doorState = door->getIsOpen();
#ifdef CU_MOBILE
        if ((door->getLevel() == _player->getLevel() || _player->getCurrentDen() + _player->getCurrentDoor() >= 1) &&
            abs(_player->getSceneNode()->getWorldPosition().x - screenToWorldCoords(_inputManager->getTapPos()).x + 25) < 350.0f * _inputManager->getRootSceneNode()->getScaleX() &&
            abs(screenToWorldCoords(_inputManager->getTapPos()).y - door->getSceneNode()->getWorldPosition().y + 25) < 270.0f * _inputManager->getRootSceneNode()->getScaleY() &&
            abs(screenToWorldCoords(_inputManager->getTapPos()).x - door->getSceneNode()->getWorldPosition().x) < 95.0f * _inputManager->getRootSceneNode()->getScaleX()) {
            _inputManager->clearDoubleTapReg();
        }
#endif
        if (_enemyController->getPossessed() != nullptr && _inputManager->getTapPos() != Vec2::ZERO) {
            if (abs(_enemyController->getPossessed()->getSceneNode()->getWorldPosition().x - door->getSceneNode()->getWorldPosition().x) < 170.0f * _inputManager->getRootSceneNode()->getScaleX() &&
                abs(screenToWorldCoords(_inputManager->getTapPos()).y - door->getSceneNode()->getWorldPosition().y + 25) < 270.0f * _inputManager->getRootSceneNode()->getScaleY() &&
                _enemyController->getPossessed()->getLevel() == door->getLevel() &&
                abs(screenToWorldCoords(_inputManager->getTapPos()).x - door->getSceneNode()->getWorldPosition().x) < 95.0f * _inputManager->getRootSceneNode()->getScaleX()) {
                AudioEngine::get()->play("doorOpen", _assets->get<Sound>("useDoor"), false, 1.0f, true);
                _hasControl = false;
                std::shared_ptr<Texture> EnemyOpeningDoor = _assets->get<Texture>("EnemyOpeningDoor");
                _enemyController->getPossessed()->getSceneNode()->setVisible(false);
                _rootScene->removeChild(_player->getSceneNode());
                _player->SetSceneNode(Player::alloc(150, _enemyController->getPossessed()->getLevel(), 0, 9, EnemyOpeningDoor)->getSceneNode());
                _player->setLevel(_enemyController->getPossessed()->getLevel());
                CULog("level: %d", _player->getLevel());
                _player->getSceneNode()->setPosition(_player->getPos(), _enemyController->getPossessed()->getLevel() * FLOOR_HEIGHT + FLOOR_OFFSET);
                if (_enemyController->getPossessed()->getMovingRight()) {
                    _player->getSceneNode()->setScale(0.63, 0.63);
                }
                else {
                    _player->getSceneNode()->setScale(-0.63, 0.63);
                }
                _player->PossessAnimation(2);

                std::function<bool()> openDoor = [&]() {
                    _player->getSceneNode()->setVisible(false);
                    _enemyController->getPossessed()->getSceneNode()->setVisible(true);
                    //_enemyController->getPossessed()->setPos(_enemyController->getPossessed()->getPos() + 130);
                    _hasControl = true;
                    return false;
                };

                std::vector<int> v1 = _enemyController->getPossessed()->getKeys();
                std::vector<int> v2 = door->getKeys();
                /*std::sort(v1.begin(), v1.end());
                std::sort(v2.begin(), v2.end());*/
                std::vector<int> key_intersection;

                std::set_intersection(v1.begin(), v1.end(),
                    v2.begin(), v2.end(),
                    std::back_inserter(key_intersection));
                if (!key_intersection.empty()|| v2.size() ==0 || door->getUnlocked()) {
                    door->getDoorLock()->setVisible(false);
                    door->setUnlocked(true);
                    door->setDoor(!doorState);
                    if (_showTutorialText == 1) {
                        _tutorialText->setText("Nice work! Touch the caged animal in cat form to win. Double tap anywhere to unpossess!");
                        _tutorialText->setPositionX(35);
                        if (_tutorialAnimation->getPositionX() != 800 && _showTutorialText == 1) {
                            removeFromParentByName("tutorialAnimation");
                            addChild(_tutorialAnimation);
                            _tutorialAnimation->setPosition(800, 300);
                        }
                    }
                    else if (_showTutorialText == 2 && _tutorialAnimation->getPositionX() != 1175) {
                        _tutorialText2->setText("The staircase door is similar to the cat den but can only be used while possessing.");
                        _tutorialText2->setPositionX(100);
                        removeFromParentByName("tutorialAnimation");
                        std::shared_ptr<Texture> tapHand = _assets->get<Texture>("tapHand");
                        _tutorialAnimation = scene2::AnimationNode::alloc(tapHand, 1, 5);
                        _tutorialAnimation->setName("tutorialAnimation");
                        _tutorialAnimation->setPosition(1175, 250);
                        _tutorialAnimation->setScale(0.25, 0.25);
                        _rootScene->addChild(_tutorialAnimation);
                        tutMaxFrame = 4;
                    }
                    else if (_showTutorialText == 4) {
                        _tutorialText->setText("Locked doors will become unlocked and stay unlocked once opened.");
                        _tutorialText->setPosition(Vec2(150, 20));
                    }
                }
                cugl::Application::get()->schedule(openDoor, 500);
                _rootScene->addChild(_player->getSceneNode());
                std::vector<int> intersect;

            }
        }
    }

}

void GameplayMode::checkEnemyPossession() {
    for (shared_ptr<Enemy> enemy : _enemyController->getEnemies()) {
        if (_enemyController->getPossessed() == nullptr) {
            CULog("player position: %f", enemy->getSceneNode()->getWorldPosition().x);
            //CULog("Whether it is true %d", abs(_player->getPos() - enemy->getSceneNode()->getWorldPosition().x) < 170.0f * _inputManager->getRootSceneNode()->getScaleX());
            if (abs(_player->getPos() - enemy->getSceneNode()->getWorldPosition().x) < 270.0f * _inputManager->getRootSceneNode()->getScaleX() &&
                abs(screenToWorldCoords(_inputManager->getTapPos()).y - enemy->getSceneNode()->getWorldPosition().y + 25) < 270.0f * _inputManager->getRootSceneNode()->getScaleY() &&
                _player->getLevel() == enemy->getLevel() &&
                abs(screenToWorldCoords(_inputManager->getTapPos()).x - enemy->getSceneNode()->getWorldPosition().x) < 150.0f * _inputManager->getRootSceneNode()->getScaleX()) {
                //AudioEngine::get()->play("doorOpen", _assets->get<Sound>("useDoor"), false, 1.0f, true);
                if (attemptPossess()) {
                    AudioEngine::get()->play("possess", _assets->get<Sound>("possess"));
                    //_possessPanel->getChildTexts()[0]->setText("x "+to_string(_player->get_nPossess()));
                    for (int i = _player->get_nPossess(); i < _possessPanel->getChildPanels().size() / 2; i++) {
                        _possessPanel->getChildPanels()[i * 2 + 1]->setVisible(false);
                    }

                }
            }
        }
        else if (abs(screenToWorldCoords(_inputManager->getTapPos()).y - enemy->getSceneNode()->getWorldPosition().y + 25) < 270.0f * _inputManager->getRootSceneNode()->getScaleY() &&
            _player->getLevel() == enemy->getLevel() &&
            abs(screenToWorldCoords(_inputManager->getTapPos()).x - enemy->getSceneNode()->getWorldPosition().x) < 310.0f * _inputManager->getRootSceneNode()->getScaleX()) {
            //AudioEngine::get()->play("doorOpen", _assets->get<Sound>("useDoor"), false, 1.0f, true);
            unpossess();
        }
    }

}

void GameplayMode::checkStaircaseDoors() {

    bool visibility;

    if (_enemyController->getPossessed() != nullptr && _inputManager->getTapPos() != Vec2::ZERO) {
        visibility = _enemyController->getPossessed()->getSceneNode()->isVisible();
        for (shared_ptr<StaircaseDoor> staircaseDoor : _staircaseDoors) {
            
#ifdef CU_MOBILE
            if ((staircaseDoor->getLevel() == _player->getLevel() || _player->getCurrentDen() + _player->getCurrentDoor() >= 1) &&
            abs(_player->getSceneNode()->getWorldPosition().x - screenToWorldCoords(_inputManager->getTapPos()).x + 25) < 350.0f * _inputManager->getRootSceneNode()->getScaleX() &&
                abs(screenToWorldCoords(_inputManager->getTapPos()).y - staircaseDoor->getSceneNode()->getWorldPosition().y - 20) < 270.0f * _inputManager->getRootSceneNode()->getScaleY() &&
                abs(screenToWorldCoords(_inputManager->getTapPos()).x - staircaseDoor->getSceneNode()->getWorldPosition().x) < 95.0f * _inputManager->getRootSceneNode()->getScaleX()) {
                _inputManager->clearDoubleTapReg();
            }
#endif
            std::vector<int> intersect;
            std::vector<int> v1 = _enemyController->getPossessed()->getKeys();
            std::vector<int> v2 = staircaseDoor->getKeys();
            /*std::sort(v1.begin(), v1.end());
            std::sort(v2.begin(), v2.end());*/

            std::vector<int> key_intersection;

            std::set_intersection(v1.begin(), v1.end(),
                v2.begin(), v2.end(),
                std::back_inserter(key_intersection));
            bool StaircasedoorState = staircaseDoor->getIsOpen();
            if (visibility && abs(_enemyController->getPossessed()->getSceneNode()->getWorldPosition().x - staircaseDoor->getSceneNode()->getWorldPosition().x) < 170.0f * _inputManager->getRootSceneNode()->getScaleX() &&
                abs(screenToWorldCoords(_inputManager->getTapPos()).y - staircaseDoor->getSceneNode()->getWorldPosition().y - 20) < 270.0f * _inputManager->getRootSceneNode()->getScaleY() &&
                _enemyController->getPossessed()->getLevel() == staircaseDoor->getLevel() &&

                abs(screenToWorldCoords(_inputManager->getTapPos()).x - staircaseDoor->getSceneNode()->getWorldPosition().x) < 95.0f * _inputManager->getRootSceneNode()->getScaleX() 
                ) {
                //!key_intersection.empty()
                if (_showTutorialText == 2) {
                    _tutorialText2->setText("Pinch to zoom in or out and get a better view of the level.");
                    _tutorialText2->setPositionX(250);
                    removeFromParentByName("tutorialAnimation");
                    //TODO pinching animation
                    std::shared_ptr<Texture> pinchHand = _assets->get<Texture>("pinchHand");
                    _tutorialAnimation = scene2::AnimationNode::alloc(pinchHand, 1, 6);
                    _tutorialAnimation->setPosition(Vec2(850, 200));
                    _tutorialAnimation->setScale(0.25, 0.25);
                    _tutorialAnimation->setName("tutorialAnimation");
                    FRAME_SWITCH = 7;
                    tutMaxFrame = 6;
                    addChild(_tutorialAnimation);
                }
                else if (_showTutorialText == 3) {
                    _tutorialText->setText("While possessing, enemies can only detect you from the back.");
                    _tutorialText->setPositionX(250);
                }

                _hasControl = false;
                std::shared_ptr<Texture> EnemyOpeningDoor = _assets->get<Texture>("EnemyOpeningDoor");
                _enemyController->getPossessed()->getSceneNode()->setVisible(false);
                _rootScene->removeChild(_player->getSceneNode());
                _player->setPos(staircaseDoor->getPos().x);
                _player->SetSceneNode(Player::alloc(_enemyController->getPossessed()->getPos(), _enemyController->getPossessed()->getLevel(), 0, 9, EnemyOpeningDoor)->getSceneNode());
                _player->setLevel(_enemyController->getPossessed()->getLevel());
                _player->getSceneNode()->setPosition(_player->getPos(), _enemyController->getPossessed()->getLevel() * FLOOR_HEIGHT + FLOOR_OFFSET);
                if (_enemyController->getPossessed()->getMovingRight()) {
                    _player->getSceneNode()->setScale(0.63, 0.63);
                }
                else {
                    _player->getSceneNode()->setScale(-0.63, 0.63);
                }
                _player->PossessAnimation(2);
                _rootScene->addChild(_player->getSceneNode());

                std::function<bool()> openDoor = [&]() {
                    _player->getSceneNode()->setVisible(false);
                    _enemyController->getPossessed()->getSceneNode()->setVisible(false);
                    //_enemyController->getPossessed()->setPos(_enemyController->getPossessed()->getPos() + 130);
                    _hasControl = true;
                    return false;
                };
                cugl::Application::get()->schedule(openDoor, 500);
                staircaseDoor->setDoor(!staircaseDoor->getIsOpen());
                _player->setCurrentDoor(staircaseDoor->getConnectedDoors());
                AudioEngine::get()->play("staircaseOpen", _assets->get<Sound>("useDoor"), false, 1.0f, true);
                //std::dynamic_pointer_cast<scene2::AnimationNode>(staircaseDoor->getSceneNode())->setFrame(4);
                break;
            }

            else if (!visibility &&
                abs(screenToWorldCoords(_inputManager->getTapPos()).y - staircaseDoor->getSceneNode()->getWorldPosition().y - 20) < 170.0f * _inputManager->getRootSceneNode()->getScaleY() &&
                abs(screenToWorldCoords(_inputManager->getTapPos()).x - staircaseDoor->getSceneNode()->getWorldPosition().x) < 95.0f * _inputManager->getRootSceneNode()->getScaleX() &&
                staircaseDoor->getConnectedDoors() == _player->getCurrentDoor()) {
                _player->setCurrentDoor(0);
                _player->setPos(staircaseDoor->getPos().x);
                _enemyController->getPossessed()->setPos(staircaseDoor->getPos().x);
                _enemyController->getPossessed()->setLevel(staircaseDoor->getLevel());
                _enemyController->getPossessed()->getKeyCircle()->setPriority(staircaseDoor->getLevel() + 0.1f);
                AudioEngine::get()->play("staircaseClose", _assets->get<Sound>("useDoor"), false, 1.0f, true);
                //ChangeDrawOrder();
                //ChangeDrawOrder();
                _hasControl = false;
                std::shared_ptr<Texture> EnemyOpeningDoor = _assets->get<Texture>("EnemyOpeningDoor");
                _enemyController->getPossessed()->getSceneNode()->setVisible(false);
                _rootScene->removeChild(_player->getSceneNode());
                _player->SetSceneNode(Player::alloc(_enemyController->getPossessed()->getPos(), _enemyController->getPossessed()->getLevel(), 0, 9, EnemyOpeningDoor)->getSceneNode());
                _player->setLevel(_enemyController->getPossessed()->getLevel());
                _player->getSceneNode()->setPosition(_enemyController->getPossessed()->getPos(), _enemyController->getPossessed()->getLevel() * FLOOR_HEIGHT + FLOOR_OFFSET);
                //TODO: BUGFIX
                _rootScene->setPositionY(_rootScene->getHeight() + (576 - _rootScene->getHeight()) / 2 - _rootScene->getScaleY() * _player->getSceneNode()->getPositionY());
                if (_enemyController->getPossessed()->getMovingRight()) {
                    _player->getSceneNode()->setScale(0.63, 0.63);
                }
                else {
                    _player->getSceneNode()->setScale(-0.63, 0.63);
                }
                _player->PossessAnimation(2);
                _rootScene->addChild(_player->getSceneNode());

                std::function<bool()> openDoor = [&]() {
                    _player->getSceneNode()->setVisible(false);
                    _enemyController->getPossessed()->getSceneNode()->setVisible(true);
                    //_enemyController->getPossessed()->setPos(_enemyController->getPossessed()->getPos() + 130);
                    _hasControl = true;

                    return false;
                };

                cugl::Application::get()->schedule(openDoor, 500);

                staircaseDoor->setDoor(!staircaseDoor->getIsOpen());

                break;
            }
        }
    }
}

void GameplayMode::checkCatDens() {

    bool visibility;

    if (_enemyController->getPossessed() == nullptr && _inputManager->getTapPos() != Vec2::ZERO) {
        visibility = _player->getSceneNode()->isVisible();
        for (shared_ptr<CatDen> catDen : _catDens) {
#ifdef CU_MOBILE
            if (abs(screenToWorldCoords(_inputManager->getTapPos()).y - catDen->getSceneNode()->getWorldPosition().y) < 120.0f * _inputManager->getRootSceneNode()->getScaleY() &&
                abs(screenToWorldCoords(_inputManager->getTapPos()).x - catDen->getSceneNode()->getWorldPosition().x) < 95.0f * _inputManager->getRootSceneNode()->getScaleX()) {
                _inputManager->clearDoubleTapReg();
            }
#endif
            bool StaircasedoorState = catDen->getIsOpen();
            if (visibility && abs(_player->getSceneNode()->getWorldPosition().x - catDen->getSceneNode()->getWorldPosition().x) < 170.0f * _inputManager->getRootSceneNode()->getScaleX() &&
                abs(screenToWorldCoords(_inputManager->getTapPos()).y - catDen->getSceneNode()->getWorldPosition().y) < 120.0f * _inputManager->getRootSceneNode()->getScaleY() &&
                _player->getLevel() == catDen->getLevel() &&
                abs(screenToWorldCoords(_inputManager->getTapPos()).x - catDen->getSceneNode()->getWorldPosition().x) < 95.0f * _inputManager->getRootSceneNode()->getScaleX()) {
                _hasControl = false;
                std::shared_ptr<Texture> CatJumpCatDen = _assets->get<Texture>("CatJumpCatDen");
                _rootScene->removeChild(_player->getSceneNode());
                _player->SetSceneNode(Player::alloc(150, 0, 0, 8, CatJumpCatDen)->getSceneNode());
                if (_player->getPos() - catDen->getPos().x <=0) {
                    /*if (_player->getPos() - catDen->getPos().x <=0) {*/
                    _player->getSceneNode()->setScale(0.15, 0.15);
                    _player->getSceneNode()->setPosition(_player->getPos() + 50, _player->getLevel() * FLOOR_HEIGHT + FLOOR_OFFSET);
                }
                else {
                    _player->getSceneNode()->setScale(-0.15, 0.15);
                    _player->getSceneNode()->setPosition(_player->getPos() -50, _player->getLevel() * FLOOR_HEIGHT + FLOOR_OFFSET);
                }
                _rootScene->addChild(_player->getSceneNode());
                _player->PossessAnimation(0);
                std::function<bool()> setPossessed = [&]() {
                    _player->getSceneNode()->setVisible(false);
                    
                    _hasControl = true;
                    return false;
                };
                cugl::Application::get()->schedule(setPossessed, 300);
                //_player->getSceneNode()->setVisible(!visibility); 
                _player->setCurrentDen(catDen->getConnectedDens());
                if (_showTutorialText == 2 && _player->getPos() < 300) {
                    _tutorialText2->setText("Swipe to pan the camera. The camera will snap back if you are on the screen. Swipe left!");
                    _tutorialText2->setPositionX(50);
                    removeFromParentByName("tutorialAnimation");
                    std::shared_ptr<Texture> leftRightHand = _assets->get<Texture>("lrHand");
                    _tutorialAnimation = scene2::AnimationNode::alloc(leftRightHand, 1, 13);
                    _tutorialAnimation->setPosition(Vec2(850, 200));
                    _tutorialAnimation->setScale(0.25, 0.25);
                    _tutorialAnimation->setName("tutorialAnimation");
                    tutMaxFrame = 12;
                    addChild(_tutorialAnimation);
                }
                //std::dynamic_pointer_cast<scene2::AnimationNode>(staircaseDoor->getSceneNode())->setFrame(4);
                break;
            }
            else if (!visibility &&
                abs(screenToWorldCoords(_inputManager->getTapPos()).y - catDen->getSceneNode()->getWorldPosition().y) < 120.0f * _inputManager->getRootSceneNode()->getScaleY() &&
                abs(screenToWorldCoords(_inputManager->getTapPos()).x - catDen->getSceneNode()->getWorldPosition().x) < 95.0f * _inputManager->getRootSceneNode()->getScaleX() &&
                _player->getCurrentDen() == catDen->getConnectedDens()) {
                _hasControl = false;
                //AudioEngine::get()->play("unpossess", _assets->get<Sound>("jumpOff"));
                _player->setLevel(catDen->getLevel());
                _player->getSceneNode()->setVisible(true);
                /*int cageCollision = collisions::checkForCagedAnimalCollision(_player, _cagedAnimal);
                if (cageCollision != 0) {
                    return;
                }*/
                _player->setCurrentDen(0);
                _player->setPos(catDen->getPos().x);
                //_player->getSceneNode()->setPosition(catDen->getPos());
                _player->getSceneNode()->setPosition(_player->getPos(), _player->getLevel() * FLOOR_HEIGHT + FLOOR_OFFSET );
                _player->PossessAnimation(2);
                std::function<bool()> delayJump = [&]() {
                    int level = _player->getLevel();
                    std::shared_ptr<Texture> catWalking = _assets->get<Texture>("cat-walking");
                    _rootScene->removeChild(_player->getSceneNode());
                    _player->SetSceneNode(Player::alloc(150, 0, 0, 8, catWalking)->getSceneNode());
                    if (_player->getMovingRight()) {
                        _player->setPos(_player->getPos() + 55);
                        _player->getSceneNode()->setScale(0.15, 0.15);
                    }
                    else {
                        _player->setPos(_player->getPos() - 55);
                        _player->getSceneNode()->setScale(-0.15, 0.15);
                    }   
                    _player->setLevel(level);
                    _rootScene->addChild(_player->getSceneNode());
                    _hasControl = true;
                    return false;
                };

                 cugl::Application::get()->schedule(delayJump, 300);

               

                if (_showTutorialText == 2){
                    _tutorialText2->setText("The paws on the top right indicate the number of possessions you have in a level.");
                    _tutorialText2->setPositionX(100);
                    removeFromParentByName("tutorialAnimation");
                    std::shared_ptr<Texture> arrow = _assets->get<Texture>("arrow");
                    _tutorialAnimation = scene2::AnimationNode::alloc(arrow, 1, 2);
                    _tutorialAnimation->setName("tutorialAnimation");
                    _tutorialAnimation->setPosition(900, 425);
                    _tutorialAnimation->setScale(0.25, 0.25);
                    _tutorialAnimation->setAngle(0.785398);
                    addChild(_tutorialAnimation);
                    tutMaxFrame = 2;
                    FRAME_SWITCH = 14;
                    _tutorialAnimation->setPriority(10);
                }
               
                if (_showTutorialText == 1) {
                    _tutorialText->setText("Touch the cage in cat form to release the animals and complete the level");
                    _tutorialText->setPosition(Vec2(200, 420));
                }
                break;
            }
        }
    }
}



vector<Vec2> GameplayMode::closedDoors() {
    vector<Vec2> closedDoors;
    for (auto it = begin(_doors); it != end(_doors); ++it) {
        auto door = it->get();
        if (!door->getIsOpen()) {
            closedDoors.push_back(Vec2(door->getPos().x, door->getLevel()));
        }
    }
    return closedDoors;
}

std::string GameplayMode::getNextLevelID() {
    if (_levelIndex + 1 >= MAX_LEVEL_NUM_PER_LOC) {
        // TODO: If greater than the maximum level number, then return a string to tell return to menu (or next scene? undecided)
        CULog("Location Cleared ! Return to Menu");
        return "Location Cleared";
    }
    else {
        // Otherwise return the next level by adding 1 to the levelIndex at the same location
        return to_string(_levelIndex + 1);
    }
}

void GameplayMode::ChangeDrawOrder() {
    int index = 0;
    std::vector<std::shared_ptr<cugl::scene2::SceneNode>>::iterator itr = std::find(_rootScene->getChildren().begin(), _rootScene->getChildren().end(),
        _rootScene->getChildByName("Player"));
    CULog("here", index);
    if (itr != _rootScene->getChildren().cend()) {
        index = std::distance(_rootScene->getChildren().begin(), itr);
    }
    if (index != 0) {
        CULog("the index is %d", index);
    }
}

void GameplayMode::toSaveJson() {
    if (_player != nullptr) { //need some other way to make sure game is initialized (scene is built)
        shared_ptr<JsonValue> playerObject = JsonValue::allocObject();
        shared_ptr<JsonValue> enemyArray = JsonValue::allocArray();
        shared_ptr<JsonValue> decorationsArray = JsonValue::allocArray();
        shared_ptr<JsonValue> staircaseDoorArray = JsonValue::allocArray();
        shared_ptr<JsonValue> doorArray = JsonValue::allocArray();

        //player
        playerObject->appendChild("x_pos", JsonValue::alloc(_player->getPos()));
        playerObject->appendChild("level", JsonValue::alloc((long)_player->getLevel()));
        playerObject->appendChild("num_possessions", JsonValue::alloc((long)_player->get_nPossess()));

        //enemies

        vector<shared_ptr<Enemy>> enemies = _enemyController->getEnemies();
        for (auto it = begin(enemies); it != end(enemies); ++it) {
            shared_ptr<JsonValue> tempObject = JsonValue::allocObject(); //for enemy
            tempObject->appendChild("x_pos", JsonValue::alloc(it->get()->getPos()));
            tempObject->appendChild("level", JsonValue::alloc((long)it->get()->getLevel()));
            tempObject->appendChild("patrol_start", JsonValue::alloc((long)it->get()->getPatrol().x));
            tempObject->appendChild("patrol_end", JsonValue::alloc((long)it->get()->getPatrol().y));
            shared_ptr<JsonValue> tempArray = JsonValue::allocArray(); //for keys
            vector<int> keys = it->get()->getKeys();
            for (auto it2 = begin(keys); it2 != end(keys); ++it2) {
                tempArray->appendChild(JsonValue::alloc((long)*it2));
            }
            if (it->get()->isPossessed()) {
                tempObject->appendChild("possessed", JsonValue::alloc(true));
            }
            tempObject->appendChild("keyInt", tempArray);
            enemyArray->appendChild(tempObject);
        }
        for (auto it = begin(_catDens); it != end(_catDens); ++it) {
            shared_ptr<JsonValue> tempObject = JsonValue::allocObject();
            tempObject->appendChild("x_pos", JsonValue::alloc(it->get()->getPos().x));
            tempObject->appendChild("level", JsonValue::alloc((long)it->get()->getLevel()));
            tempObject->appendChild("connection", JsonValue::alloc((long)it->get()->getConnectedDens()));
            tempObject->appendChild("isDen", JsonValue::alloc(true));
            staircaseDoorArray->appendChild(tempObject);
        }
        for (auto it = begin(_staircaseDoors); it != end(_staircaseDoors); ++it) {
            shared_ptr<JsonValue> tempObject = JsonValue::allocObject();
            tempObject->appendChild("x_pos", JsonValue::alloc(it->get()->getPos().x));
            tempObject->appendChild("level", JsonValue::alloc((long)it->get()->getLevel()));
            tempObject->appendChild("connection", JsonValue::alloc((long)it->get()->getConnectedDoors()));
            tempObject->appendChild("isDen", JsonValue::alloc(false));
            staircaseDoorArray->appendChild(tempObject);
        }
        for (auto it = begin(_doors); it != end(_doors); ++it) {
            shared_ptr<JsonValue> tempObject = JsonValue::allocObject();
            tempObject->appendChild("x_pos", JsonValue::alloc(it->get()->getPos().x));
            tempObject->appendChild("level", JsonValue::alloc((long)it->get()->getLevel()));
            shared_ptr<JsonValue> tempArray = JsonValue::allocArray(); //for keys
            vector<int> keys = it->get()->getKeys();
            for (auto it2 = begin(keys); it2 != end(keys); ++it2) {
                tempArray->appendChild(JsonValue::alloc((long)*it2));
            }
            tempObject->appendChild("keyInt", tempArray);
            tempObject->appendChild("isOpen", JsonValue::alloc(it->get()->getIsOpen()));
            doorArray->appendChild(tempObject);
        }
        //TODO IF WE EVER GET MORE THAN 1 DECORATIONS
        shared_ptr<JsonValue> tempObject = JsonValue::allocObject();
        tempObject->appendChild("x_pos", JsonValue::alloc((long)_cagedAnimal->getPos()));
        tempObject->appendChild("level", JsonValue::alloc((long)_cagedAnimal->getLevel()));
        tempObject->appendChild("objective", JsonValue::alloc((long)1));
        tempObject->appendChild("texture", JsonValue::alloc("caged-animal"));
        decorationsArray->appendChild(tempObject);

        shared_ptr<JsonValue> result = JsonValue::allocObject();
        result->appendChild("player", playerObject);
        result->appendChild("enemy", enemyArray);
        result->appendChild("decorations", decorationsArray);
        result->appendChild("staircase-door", staircaseDoorArray);
        result->appendChild("door", doorArray);
        result->appendChild("floor", JsonValue::alloc((long)_numFloors));
        result->appendChild("level", JsonValue::alloc((long)_levelIndex));
        shared_ptr<JsonWriter> writer = JsonWriter::alloc(Application::get()->getSaveDirectory() + "save.json");

        writer->writeJson(result);
        writer->close();
    }
}

void GameplayMode::centerCamera() {
    if (!_inputManager->getPivot() && (_player->getCurrentDen() + _player->getCurrentDoor() == 0)) {
        _rootScene->setPositionX(_rootScene->getWidth() + (1024 - _rootScene->getWidth()) / 2 - _rootScene->getScaleX() * _player->getSceneNode()->getPositionX());
        _rootScene->setPositionY(_rootScene->getHeight() + (576 - _rootScene->getHeight()) / 2 - _rootScene->getScaleY() * _player->getLevel()*FLOOR_HEIGHT + CAMERA_OFFSET);
    }
}

void GameplayMode::removeFromParentByName(string name) {
    if (_rootScene->getChildByName(name) != nullptr) {
        _rootScene->removeChild(_rootScene->getChildByName(name));
    }
    else if (getChildByName(name) != nullptr) {
        removeChild(getChildByName(name));
    }
}