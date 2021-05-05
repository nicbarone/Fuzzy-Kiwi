//
//  HelloApp.cpp
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

bool GameplayMode::init(const std::shared_ptr<cugl::AssetManager>& assets, int level, std::shared_ptr<InputManager> inputManager) {
    setGameStatus(GameStatus::RUNNING);
    _inputManager = inputManager;
    _levelIndex = level;
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

bool GameplayMode::init(const std::shared_ptr<cugl::AssetManager>& assets, int level, std::shared_ptr<JsonValue> json, std::shared_ptr<InputManager> inputManager) {
    _showTutorialText = false;
    _inputManager = inputManager;
    _levelIndex = level;
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
    _winPanel->getChildButtons()[0]->getButton()->deactivate();
    _winPanel->getChildButtons()[1]->getButton()->deactivate();
    _winPanel->getChildButtons()[2]->getButton()->deactivate();
    _losePanel->getChildButtons()[0]->getButton()->deactivate();
    _losePanel->getChildButtons()[1]->getButton()->deactivate();
    _menuPanel->getChildButtons()[0]->getButton()->deactivate();
    _menuPanel->getChildButtons()[1]->getButton()->deactivate();
    _menuPanel->getChildButtons()[2]->getButton()->deactivate();
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
        shared_ptr<JsonReader> reader = JsonReader::allocWithAsset("levels\\level" + std::to_string(_levelIndex+1) + ".json");
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
    if (getGameStatus() == GameStatus::PAUSED) {
        _tutorialAnimation->setVisible(false);
        _menuPanel->setVisible(true);
        _menuPanel->getChildButtons()[0]->getButton()->activate();
        _menuPanel->getChildButtons()[1]->getButton()->activate();
        _menuPanel->getChildButtons()[2]->getButton()->activate();
    }
    else {
        _menuPanel->setVisible(false);
        _menuPanel->getChildButtons()[0]->getButton()->deactivate();
        _menuPanel->getChildButtons()[1]->getButton()->deactivate();
        _menuPanel->getChildButtons()[2]->getButton()->deactivate();
    }
    if (getGameStatus() != GameStatus::RUNNING) {
        return;
    }
    if (_showTutorialText == 1) {
        if (!_player->canPossess() && !_player->getPossess() && !_doors.at(0)->getIsOpen()) {
            _tutorialText->setText("Oh no! You are stuck! Use the pause button on the top left to retry this level.");
            _tutorialText->setPositionX(0);
        }
        if (tutFrameSwitch > 0) {
            tutFrameSwitch--;
        }
        else {
            tutFrameSwitch = 7;
            tutFrame++;
            tutFrame = tutFrame % tutMaxFrame;
            _tutorialAnimation->setFrame(tutFrame);
        }
    }
    if (_showTutorialText == 2) {
        if (_player->getLevel() == 0 && _player->getPos() > 950) {
            _tutorialText2->setText("Staircase doors work similarly to cat dens but can only be used while possessing. Tap on them to enter and leave!");
            _tutorialText2->setPositionX(0);
        }

        if (_player->getLevel() == 1) {
            _tutorialText2->setText("Nice work! Touch the cage in the cat form to complete the level.");
            _tutorialText2->setPosition(Vec2(100, 420));
            _tutorialText2->setPriority(3);
        }
        if (!_player->canPossess() && !_player->getPossess() && _player->getLevel() == 0) {
            _tutorialText2->setText("Oh no! You are stuck! Use the pause button on the top left to retry this level.");
            _tutorialText2->setPositionX(0);
        }
    }
    // Read input controller input
    _inputManager->readInput();
    if (_hasControl) {
        if (_enemyController->closestEnemy() != nullptr && _player->canPossess()) {
            CULog("has control %d", _hasControl);
            _enemyController->closestEnemy()->setGlow(true);
            if (_showTutorialText == 1) {
                if (USE_TAP_POSSESS) {
                    _tutorialText->setText("When in range to possess, the enemy will be highlighted. Double tap to possess!");
                    _tutorialText->setPositionX(0);
                }

                if (!USE_TAP_POSSESS) {
                    if (_tutorialAnimation->getPositionX() != 800) {
                        std::shared_ptr<Texture> upDownHand = _assets->get<Texture>("upDownHand");
                        _tutorialAnimation->setTexture(upDownHand);
                        _tutorialAnimation->setPosition(800, 300);
                    }
                }
                else {
                    if (_rootScene->getChildByName("tutorialAnimation") == nullptr) {
                        removeChild(_tutorialAnimation);
                        std::shared_ptr<Texture> tapHand = _assets->get<Texture>("tapHand");
                        _tutorialAnimation = scene2::AnimationNode::alloc(tapHand, 1, 5);
                        _tutorialAnimation->setName("tutorialAnimation");
                        _rootScene->addChild(_tutorialAnimation);
                        _tutorialAnimation->setPosition(410, 200);
                        _tutorialAnimation->setScale(0.25, 0.25);
                        tutMaxFrame = 4;
                        _tutorialAnimation->setPriority(10);
                    }

                }



            }
            else if (_showTutorialText == 2) {
                _tutorialText2->setText("The paws on the top right indicate the number of possessions you have in a level. Use them wisely!");
                _tutorialText2->setPositionX(0);
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
                        _tutorialText->setText("You can open doors by clicking on them. You can only open doors while possessing an enemy and near the door.");
                        _tutorialText->setPosition(Vec2(100, 110));
                        _tutorialAnimation->removeFromParent();
                    }

                }
            }
        }
        else if (_inputManager->getAndResetUnpossessPressed()) {
            if (_player->get_possessEnemy() != nullptr) {
                unpossess();

            }
        }

        checkStaircaseDoors();
        checkDoors();
        checkCatDens();
        //checkEnemyPossession();
        collisions::checkForDoorCollision(_enemyController->getPossessed(), _enemyController->getEnemies(), _player, _doors);
        int cageCollision = collisions::checkForCagedAnimalCollision(_player, _cagedAnimal);
        if (_hasControl && cageCollision != 0) {
            _hasControl = false;
            
                    setGameStatus(GameStatus::WIN);
                    AudioEngine::get()->play("win", _assets->get<Sound>("winCondition"));
                    // shows win Panel
                    _winPanel->setVisible(true);
                    _winPanel->getChildButtons()[0]->getButton()->activate();
                    _winPanel->getChildButtons()[1]->getButton()->activate();
                    _winPanel->getChildButtons()[2]->getButton()->activate();
                  
              

            
            collisions::checkInBounds(_enemyController->getPossessed(), _player);

            /**possess code works a bit better when movement is processed last (scene node position is updated here)
                else you get one frame of wrong position*/
                // For now, if possessing, disable cat movement, put it to the same location as the possessed enemy
            if (_player->getPossess()) {
                _player->setPos(_player->get_possessEnemy()->getPos());
            }
            else if (_hasControl && _player->getSceneNode()->isVisible()) {
                _player->move(_inputManager->getForward());
            }
#ifdef CU_MOBILE
            // move joystick for visualization
            _joystickPanel->getChildPanels()[0]->setPos(_joystickPanel->getSceneNode()->getPosition() / _joystickPanel->getSceneNode()->getScaleX() - _inputManager->getJoystickPosition() / 2.0f);
#endif
            // Enemy movement
            _enemyController->moveEnemies(_inputManager->getForward());
            if (_hasControl) {
                _enemyController->findClosest(_player->getPos(), _player->getLevel(), closedDoors());

            }

            if (_enemyController->getPossessed() != nullptr) {
                //CULog("%d", _enemyController->getPossessed()->facingRight());
            }
            if (_hasControl && _enemyController->detectedPlayer(_player->getPos(), _player->getLevel(), closedDoors())) {
                if (_player->getSceneNode()->isVisible() ||
                    (_enemyController->getPossessed() != nullptr && _enemyController->getPossessed()->getSceneNode()->isVisible())) {
                    _hasControl = false;
                    std::shared_ptr<Texture> CagedCat = _assets->get<Texture>("CagedCat");
                    if (_enemyController->getPossessed() != nullptr) {
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


                        std::function<bool()> color = [&]() {
                            _enemyController->colorDetectingPlayer(_player->getPos(), _player->getLevel(), closedDoors());
                            return false;
                        };
                        std::function<bool()> losing = [&]() {

                            _player->getSceneNode()->setVisible(false);
                            setGameStatus(GameStatus::LOSE);
                            if (_showTutorialText == 1) {
                                _tutorialAnimation->setVisible(false);
                            }
                            _losePanel->setVisible(true);
                            _losePanel->getChildButtons()[0]->getButton()->activate();
                            _losePanel->getChildButtons()[1]->getButton()->activate();
                            if (_showTutorialText == 1) {
                                _tutorialText->setText("Oh no! You got caught!");
                                _tutorialText->setPosition(Vec2(100, 110));
                                _tutorialText2->setVisible(false);
                            }
                            _hasControl = false;
                            return false;
                        };
                        cugl::Application::get()->schedule(color, 500);
                        cugl::Application::get()->schedule(losing, 1500);
                    }
                    else {
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
                        std::function<bool()> color = [&]() {
                            _enemyController->colorDetectingPlayer(_player->getPos(), _player->getLevel(), closedDoors());
                            return false;
                        };
                        std::function<bool()> losing = [&]() {
                            _player->getSceneNode()->setVisible(false);
                            setGameStatus(GameStatus::LOSE);
                            _losePanel->setVisible(true);
                            _losePanel->getChildButtons()[0]->getButton()->activate();
                            _losePanel->getChildButtons()[1]->getButton()->activate();
                            if (_showTutorialText == 1) {
                                _tutorialText->setText("Oh no! You got caught!");
                                _tutorialText->setPosition(Vec2(100, 110));
                                _tutorialText2->setVisible(false);
                            }
                            _hasControl = false;
                            return false;
                        };
                        cugl::Application::get()->schedule(color, 500);

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
}

bool GameplayMode::attemptPossess() {
    std::shared_ptr<Enemy> enemy = _enemyController->closestEnemy();
    if (_hasControl && enemy != nullptr) {
        _hasControl = false;
        _player->setPossess(true);
        _player->set_possessEnemy(_enemyController->closestEnemy());
        _enemyController->updatePossessed(_enemyController->closestEnemy());
//        _rootScene->removeChild(_enemyController->closestEnemy()->getPatrolNode());
        _enemyController->closestEnemy()->setGlow(false);

        std::shared_ptr<Texture> catJump = _assets->get<Texture>("catPossessing");
        _rootScene->removeChild(_player->getSceneNode());
        _player->SetSceneNode(Player::alloc(0, 0, 0, 8, catJump)->getSceneNode());
        if (_player->getMovingRight()) {
            _player->getSceneNode()->setPosition(_player->getPos()+70, _player->getLevel() * FLOOR_HEIGHT + FLOOR_OFFSET);
            _player->getSceneNode()->setScale(0.15, 0.15);
        }
        else {
            _player->getSceneNode()->setPosition(_player->getPos()-70, _player->getLevel() * FLOOR_HEIGHT + FLOOR_OFFSET);
            _player->getSceneNode()->setScale(-0.15, 0.15);
        }

        _rootScene->addChild(_player->getSceneNode());
        _player->PossessAnimation(0);

        std::function<bool()> setPossessed = [&]() {
            _player->getSceneNode()->setVisible(false);
            _enemyController->closestEnemy()->setAsPossessed();
            _enemyController->closestEnemy()->getSceneNode()->removeAllChildren();
            _hasControl = true;
            return false;
        };
        cugl::Application::get()->schedule(setPossessed, 500);
        return true;

        //code used for cat jumping animation, incomplete and not activated in our release
    }
    return false;
}

void GameplayMode::unpossess() {
    if (_hasControl) {
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
        /*std::shared_ptr<Texture> EnemyDying = _assets->get<Texture>("EnemyDying");
        int enemyPos = enemy->getPos();
        int enemyLevel = enemy->getLevel();
        _rootScene->removeChild(enemy->getSceneNode());
        enemy->SetSceneNode(Enemy::alloc(enemyPos, enemyLevel, 0, {}, 0, 0, 9,EnemyDying,
            EnemyDying, EnemyDying)->getSceneNode());*/
            //enemy->enemyDyingAnimation();
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
    }
  
    //_player->getSceneNode()->setFrame(7);
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

    std::shared_ptr<Texture> doorFrame = _assets->get<Texture>("doorFrame");
    //caged animal
    std::shared_ptr<Texture> cagedAnimal = _assets->get<Texture>("cagedAnimal");
    // Enemy creation
    _enemyController = make_shared<EnemyController>();
    enemyTexture = _assets->get<Texture>("enemy");
    std::shared_ptr<Texture> altTexture = _assets->get<Texture>("possessed-enemy");
    enemyHighlightTexture = _assets->get<Texture>("enemy-glow");
    tableTexture = _assets->get<Texture>("lab-table");

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
                key, objectTemp->getFloat("patrol_start"), objectTemp->getFloat("patrol_end"),5, enemyTexture, altTexture, enemyHighlightTexture, tableTexture);
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
            _doors.push_back(Door::alloc(objectTemp->getFloat("x_pos"), 0, Vec2(1, 1), objectTemp->getInt("level"),
                cugl::Color4::WHITE, {1}, 1, 8, door));
            _doorFrames.push_back(DoorFrame::alloc(objectTemp->getFloat("x_pos")-77, 0, Vec2(1.0, 1), objectTemp->getInt("level"), cugl::Color4::WHITE, { 1 }, 1, 8, doorFrame));
        }
    }
    if (decorationsJSON != nullptr) {
        for (int i = 0; i < decorationsJSON->size(); i++) {
            objectTemp = decorationsJSON->get(i);
            _cagedAnimal = Player::alloc(objectTemp->getFloat("x_pos"), objectTemp->getInt("level"), 0, 1, cagedAnimal);
            _cagedAnimal->getSceneNode()->setScale(0.3, 0.3);

            //_cagedAnimal = CagedAnimal::alloc(objectTemp->getFloat("x_pos"), 0, Vec2(0.3, 0.3), objectTemp->getInt("level"), cugl::Color4::WHITE, { }, 1, 1, 1, cagedAnimal);

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
    _tutorialText = scene2::Label::alloc("Use the bottom left joystick to move! Pinch in and out to change zoom levels and swipe with one or two fingers to pan.", font);
    _tutorialText->setForeground(Color4::WHITE);
    _tutorialText->setScale(Vec2(0.5, 0.5));
    _tutorialText->setPosition(Vec2(-100, 110));
    _tutorialText->setVisible(_showTutorialText == 1);
    _rootScene->addChild(_tutorialText);
    _tutorialText2 = scene2::Label::alloc("The holes in the wall are cat dens and can be used to travel to different cat dens while in cat form. Simply click on them to enter and leave!", font);
    _tutorialText2->setForeground(Color4::WHITE);
    _tutorialText2->setScale(Vec2(0.5, 0.5));
    _tutorialText2->setPosition(Vec2(-200, 110));
    _tutorialText2->setVisible(_showTutorialText == 2);
    _rootScene->addChild(_tutorialText2);
    std::shared_ptr<Texture> leftRightHand = _assets->get<Texture>("lrHand");
    _tutorialAnimation = scene2::AnimationNode::alloc(leftRightHand, 1, 13);
    _tutorialAnimation->setPosition(Vec2(100, 175));
    _tutorialAnimation->setScale(0.25, -0.25);
    _tutorialAnimation->setName("tutorialAnimation");

    _tutorialAnimation->setVisible(_showTutorialText == 1);

    tutMaxFrame = 12;
    addChild(_rootScene);

#ifdef CU_MOBILE
    // make joystick panel
    _joystickPanel = ui::PanelElement::alloc(0, 0, 0, _assets->get<Texture>("joystickBorder"));
    _joystickPanel->getSceneNode()->setScale(1.0f);
    _joystickPanel->getSceneNode()->setPosition(_assets->get<Texture>("joystickBorder")->getWidth()* _joystickPanel->getSceneNode()->getScaleX() / 2.0f, _assets->get<Texture>("joystickBorder")->getHeight()* _joystickPanel->getSceneNode()->getScaleY() / 2.0f);
    _joystickPanel->createChildPanel(0, 0, 0, _assets->get<Texture>("joystick"));
    addChild(_joystickPanel->getSceneNode());
#endif

    // make possess panel
    _possessPanel = ui::PanelElement::alloc(0, 0, 0, _assets->get<Texture>("possessCounter"));
    _possessPanel->getSceneNode()->setAnchor(Vec2::ANCHOR_TOP_RIGHT);
    _possessPanel->getSceneNode()->setScale(1.0f);
    _possessPanel->getSceneNode()->setPosition(size);
    // TODO: get max possess number
    std::shared_ptr<Texture> possessUsed = _assets->get<Texture>("usedPossession");
    std::shared_ptr<Texture> possessAvailable = _assets->get<Texture>("availablePossession");
    for (int i = 0; i < _player->get_nPossess(); i++) {
        _possessPanel->createChildPanel(0, 0, 0, possessUsed);
        _possessPanel->getChildPanels()[i * 2]->getSceneNode()->setAnchor(Vec2::ANCHOR_TOP_RIGHT);
        _possessPanel->getChildPanels()[i * 2]->getSceneNode()->setScale(0.5f);
        _possessPanel->getChildPanels()[i * 2]->setPos(_possessPanel->getSceneNode()->getContentSize()
            * _possessPanel->getSceneNode()->getScaleX() - Vec2(i * (possessAvailable->getSize().width + 20.0f) * _possessPanel->getChildPanels()[0]->getSceneNode()->getScaleX() + 20.0f, 20.0f));

        _possessPanel->createChildPanel(0, 0, 0, possessAvailable);
        _possessPanel->getChildPanels()[i * 2 + 1]->getSceneNode()->setAnchor(Vec2::ANCHOR_TOP_RIGHT);
        _possessPanel->getChildPanels()[i * 2 + 1]->getSceneNode()->setScale(0.5f);
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
    _menuPanel->createChildButton(0, 60, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("resume"), Color4f::WHITE);
    _menuPanel->getChildButtons()[0]->getButton()->setName("resume");
    _menuPanel->getChildButtons()[0]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            if (_showTutorialText == 1) {
                _tutorialAnimation->setVisible(true);
            }
            setGameStatus(GameStatus::RUNNING);
        }
    });
    _menuPanel->createChildButton(0, -45, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("restartLevel"), Color4f::WHITE);
    _menuPanel->getChildButtons()[1]->getButton()->setName("restartLevel");
    _menuPanel->getChildButtons()[1]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _reset = true;
        }
    });
    _menuPanel->createChildButton(0, -150, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("exit"), Color4f::WHITE);
    _menuPanel->getChildButtons()[2]->getButton()->setName("returnToMenu");
    _menuPanel->getChildButtons()[2]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _backToMenu = true;
        }
    });
    addChild(_menuPanel->getSceneNode());

    // Create Win Panel
    winPanel = _assets->get<Texture>("levelCompleteBG");
    _winPanel = ui::PanelElement::alloc(size.width / 2, size.height / 2, 0, winPanel);
    _winPanel->getSceneNode()->setScale(max(size.width / winPanel->getSize().width, size.height / winPanel->getSize().height));
    _winPanel->setVisible(false);
    _winPanel->createChildButton(1700, -1000, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("nextLevel"), Color4f::WHITE);
    _winPanel->getChildButtons()[0]->getButton()->setScale(1.5f);
    _winPanel->getChildButtons()[0]->getButton()->setName("nextLevel");
    _winPanel->getChildButtons()[0]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _nextLevel = true;
        }
        });
    _winPanel->createChildButton(-1700, -1000, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("tryAgain"), Color4f::WHITE);
    _winPanel->getChildButtons()[1]->getButton()->setScale(1.5f);
    _winPanel->getChildButtons()[1]->getButton()->setName("retry");
    _winPanel->getChildButtons()[1]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _reset = true;
        }
        });
    _winPanel->createChildButton(-1700, 1000, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("returnToMenu"), Color4f::WHITE);
    _winPanel->getChildButtons()[2]->getButton()->setScale(1.5f);
    _winPanel->getChildButtons()[2]->getButton()->setName("menu");
    _winPanel->getChildButtons()[2]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _backToMenu = true;
        }
        });
    addChild(_winPanel->getSceneNode());

    // Create Lose Panel
    losePanel = _assets->get<Texture>("levelFailedBG");
    _losePanel = ui::PanelElement::alloc(size.width / 2, size.height / 2, 0, losePanel);
    _losePanel->getSceneNode()->setScale(max(size.width / losePanel->getSize().width, size.height / losePanel->getSize().height));
    _losePanel->setVisible(false);
    _losePanel->createChildButton(230, -350, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("retry"), Color4f::WHITE);
    _losePanel->getChildButtons()[0]->getButton()->setName("retry");
    _losePanel->getChildButtons()[0]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _reset = true;
        }
        });
    _losePanel->createChildButton(230, -850, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("menu"), Color4f::WHITE);
    _losePanel->getChildButtons()[1]->getButton()->setName("menu");
    _losePanel->getChildButtons()[1]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _backToMenu = true;
        }
        });
    addChild(_losePanel->getSceneNode());
    // Initialize input manager
    _inputManager->init(_player, _rootScene, getBounds());
    _rootScene->setScale(Vec2(0.6f, 0.6f));
    addChild(_tutorialAnimation);
}


void GameplayMode::checkDoors() {
    for (shared_ptr<Door> door : _doors) {
        bool doorState = door->getIsOpen();
        if (_enemyController->getPossessed() != nullptr) {
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
                if (!key_intersection.empty()) {
                    door->setDoor(!doorState);
                    if (_showTutorialText == 1) {
                        _tutorialText->setText("Nice work! To complete the level, touch the caged animal in cat form. Double tap to unpossess!");
                        _tutorialText->setPosition(Vec2(100, 110));
                    }
                    else if(_showTutorialText == 2){
                        _tutorialText2->setText("Time your movement to reach the staircase door! The enemy ahead can only detect you when you are possessing if he sees your back.");
                        _tutorialText2->setPosition(Vec2(-200, 110));
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
                abs(screenToWorldCoords(_inputManager->getTapPos()).y - enemy->getSceneNode()->getWorldPosition().y+25) < 270.0f * _inputManager->getRootSceneNode()->getScaleY() &&
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

    if (_enemyController->getPossessed() != nullptr) {
        visibility = _enemyController->getPossessed()->getSceneNode()->isVisible();
        for (shared_ptr<StaircaseDoor> staircaseDoor : _staircaseDoors) {
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
                abs(screenToWorldCoords(_inputManager->getTapPos()).y - staircaseDoor->getSceneNode()->getWorldPosition().y-20) < 270.0f * _inputManager->getRootSceneNode()->getScaleY() &&
                _enemyController->getPossessed()->getLevel() == staircaseDoor->getLevel() &&

                abs(screenToWorldCoords(_inputManager->getTapPos()).x - staircaseDoor->getSceneNode()->getWorldPosition().x) < 95.0f * _inputManager->getRootSceneNode()->getScaleX()&&
                !key_intersection.empty()) {
                _hasControl = false;
                std::shared_ptr<Texture> EnemyOpeningDoor = _assets->get<Texture>("EnemyOpeningDoor");
                _enemyController->getPossessed()->getSceneNode()->setVisible(false);
                _rootScene->removeChild(_player->getSceneNode());
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
                abs(screenToWorldCoords(_inputManager->getTapPos()).y - staircaseDoor->getSceneNode()->getWorldPosition().y-20) < 170.0f * _inputManager->getRootSceneNode()->getScaleY() &&
                abs(screenToWorldCoords(_inputManager->getTapPos()).x - staircaseDoor->getSceneNode()->getWorldPosition().x) < 95.0f * _inputManager->getRootSceneNode()->getScaleX()&&
                staircaseDoor->getConnectedDoors()== _player->getCurrentDoor()) {
                _player->setCurrentDoor(0);
                _enemyController->getPossessed()->setPos(staircaseDoor->getPos().x);
                _enemyController->getPossessed()->setLevel(staircaseDoor->getLevel());
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
                if (_showTutorialText == 2) {
                    _tutorialText->setText("Touch the cage in cat form to release the animals and complete the level");
                    _tutorialText->setPosition(Vec2(200, 420));
                }

                if (_showTutorialText == 2) {
                    _tutorialText2->setText("Congrats, you did it!");
                    _tutorialText2->setPosition(Vec2(300, 420));
                }
                break;
            }
        }
    }
}

void GameplayMode::checkCatDens() {

    bool visibility;

    if (_enemyController->getPossessed() == nullptr) {
        visibility = _player->getSceneNode()->isVisible();
        for (shared_ptr<CatDen> catDen : _catDens) {
            bool StaircasedoorState = catDen->getIsOpen();
            if (visibility && abs(_player->getSceneNode()->getWorldPosition().x - catDen->getSceneNode()->getWorldPosition().x) < 170.0f * _inputManager->getRootSceneNode()->getScaleX() &&
                abs(screenToWorldCoords(_inputManager->getTapPos()).y - catDen->getSceneNode()->getWorldPosition().y) < 120.0f * _inputManager->getRootSceneNode()->getScaleY() &&
                _player->getLevel() == catDen->getLevel() &&
                abs(screenToWorldCoords(_inputManager->getTapPos()).x - catDen->getSceneNode()->getWorldPosition().x) < 95.0f * _inputManager->getRootSceneNode()->getScaleX()) {

                _player->getSceneNode()->setVisible(!visibility);
                _player->setCurrentDen(catDen->getConnectedDens());
                //std::dynamic_pointer_cast<scene2::AnimationNode>(staircaseDoor->getSceneNode())->setFrame(4);
                break;
            }

            else if (!visibility &&
                abs(screenToWorldCoords(_inputManager->getTapPos()).y - catDen->getSceneNode()->getWorldPosition().y) < 120.0f * _inputManager->getRootSceneNode()->getScaleY() &&
                abs(screenToWorldCoords(_inputManager->getTapPos()).x - catDen->getSceneNode()->getWorldPosition().x) < 95.0f * _inputManager->getRootSceneNode()->getScaleX()&&
                _player->getCurrentDen() == catDen->getConnectedDens()){
                _player->setCurrentDen(0);
                _player->getSceneNode()->setVisible(!visibility);
                _player->setPos(catDen->getPos().x);
                _player->setLevel(catDen->getLevel());
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
