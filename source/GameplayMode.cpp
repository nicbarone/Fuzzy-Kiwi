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

bool GameplayMode::init(const std::shared_ptr<cugl::AssetManager>& assets, int location, int level, std::shared_ptr<InputManager> inputManager) {
    setGameStatus(GameStatus::RUNNING);
    _inputManager = inputManager;
    _locationIndex = location;
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
    _rootScene = scene2::SceneNode::alloc();
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
    buildScene();


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

bool GameplayMode::init(const std::shared_ptr<cugl::AssetManager>& assets, int location, int level, std::shared_ptr<JsonValue> json, std::shared_ptr<InputManager> inputManager) {
    _inputManager = inputManager;
    _locationIndex = location;
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
    _rootScene = scene2::SceneNode::alloc();
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
    _rootScene = scene2::SceneNode::alloc();
    _rootScene->setAnchor(Vec2::ANCHOR_CENTER);
    _rootScene->setContentSize(size);
    _reset = false;
    if (_json != nullptr) {
        buildScene(_json);
    }
    else {
        buildScene();
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

    // Read input controller input
    _inputManager->readInput();
    if (_hasControl) {
        if (_enemyController->closestEnemy() != nullptr && _player->canPossess()) {
            //very temporary modification to test whether it works, dont want to work with highlight right now
            _enemyController->closestEnemy()->setGlow(true);
        }
        if (_inputManager->getTapPos().x != 0) {
            /*CULog("x: %f, y: %f", _inputManager.getTapPos().x, _inputManager.getTapPos().x);
            CULog("x: %f, y: %f", _level1StairDoor->getPos().x, _level1StairDoor->getPos().y);*/
            //CULog("x: %f", abs(_player->getPos().x - _level1StairDoor->getPos().x));
            //CULog("Is possessing: %d", _player->getPossess());
            //CULog("Enemy position: %d",   _enemyController->getPossessed()->getPos().x);

        }    //546 546

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
                    _possessPanel->getChildTexts()[0]->setText("x "+to_string(_player->get_nPossess()));
                    if (_json == nullptr) {
                        _tutorialText->setText("You can open the door while possessing an enemy and can only be detected from the back");
                        _tutorialText->setPosition(Vec2(100, 110));
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
        collisions::checkForDoorCollision(_enemyController->getPossessed(), _enemyController->getEnemies(), _player, _doors);
        int cageCollision = collisions::checkForCagedAnimalCollision(_player, _cagedAnimal);
        if (cageCollision != 0) {
            setGameStatus(GameStatus::WIN);
            // shows win Panel
            _winPanel->setVisible(true);
            _winPanel->getChildButtons()[0]->getButton()->activate();
            _winPanel->getChildButtons()[1]->getButton()->activate();
            _winPanel->getChildButtons()[2]->getButton()->activate();
        }
        collisions::checkInBounds(_enemyController->getPossessed(),_player);

        /**possess code works a bit better when movement is processed last (scene node position is updated here)
            else you get one frame of wrong position*/
            // For now, if possessing, disable cat movement, put it to the same location as the possessed enemy
        if (_player->getPossess()) {
            _player->setPos(_player->get_possessEnemy()->getPos());
        }
        else {
            _player->move(_inputManager->getForward());
        }
        // Enemy movement
        _enemyController->moveEnemies(_inputManager->getForward());
        _enemyController->findClosest(_player->getPos(), _player->getLevel(), closedDoors());

        if (_enemyController->getPossessed() != nullptr) {
            //CULog("%d", _enemyController->getPossessed()->facingRight());
        }
        if (_enemyController->detectedPlayer(_player->getPos(), _player->getLevel(), closedDoors())) {
            if (_player->getSceneNode()->isVisible() || 
                (_enemyController->getPossessed() != nullptr && _enemyController->getPossessed()->getSceneNode()->isVisible())) {
                setGameStatus(GameStatus::LOSE);
                _losePanel->setVisible(true);
                _losePanel->getChildButtons()[0]->getButton()->activate();
                _losePanel->getChildButtons()[1]->getButton()->activate();
                if (_json == nullptr) {
                    _tutorialText->setText("Oh no! You got caught! Press the R key to retry");
                    _tutorialText->setPosition(Vec2(100, 110));
                }
                _hasControl = false;
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
            if (_json == nullptr) {
                _tutorialText->setText("Oh no! You are stuck! Press the R key to retry");
                _tutorialText->setPosition(Vec2(100, 110));
            }
        }
    }

    //resetting
    if (_inputManager->didReset()) {
        reset();
    }
}

bool GameplayMode::attemptPossess() {
    std::shared_ptr<Enemy> enemy = _enemyController->closestEnemy();
    if (enemy != nullptr) {
        vector<Vec2> doors = closedDoors();

        //code used for cat jumping animation, incomplete and not activated in our release
       std::shared_ptr<Texture> catJump = _assets->get<Texture>("catPossessing");
        _rootScene->removeChild(_player->getSceneNode());
        _player->SetSceneNode(scene2::AnimationNode::alloc(catJump, 1, 8));
        _player->getSceneNode()->setPosition(_player->getPos(), _player->getLevel() * FLOOR_HEIGHT + FLOOR_OFFSET -45);
        _player->getSceneNode()->setScale(0.15, 0.15);
        _rootScene->addChild(_player->getSceneNode());
        
        _player->getSceneNode()->setVisible(false);
        _player->setPossess(true);
        _player->set_possessEnemy(enemy);
        _enemyController->updatePossessed(enemy);
        _rootScene->removeChild(enemy->getPatrolNode());
        enemy->setGlow(false);
        enemy->setPossessed();
        return true;
    }
    return false;
}

void GameplayMode::unpossess() {
    std::shared_ptr<Enemy> enemy = _enemyController->getPossessed();
    if (enemy == nullptr) return;
    _player->setPos((enemy->getPos()));
    _player->setLevel(enemy->getLevel());
    _player->getSceneNode()->setVisible(true);
    _player->setPossess(false);
    _player->setLevel(_enemyController->getPossessed()->getLevel());
    enemy->getSceneNode()->setVisible(false);
    enemy->dispose();

    
    _enemyController->removeEnemy(enemy);
    _enemyController->updatePossessed(nullptr);
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
    _rootScene = scene2::SceneNode::alloc();
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
void GameplayMode::buildScene() {
    // First clear root scene
    //clearRootSceneNode();
    Size  size = Application::get()->getDisplaySize();
    float scale = GAME_WIDTH / size.width;
    size *= scale;

    _hasControl = true;

    // Placeholder cat
    //std::shared_ptr<Texture> cat = _assets->get<Texture>("cat-walking");
    std::shared_ptr<Texture> cat = _assets->get<Texture>("cat-walking");
    // Create the player
    std::shared_ptr<Texture> catPossessing = _assets->get<Texture>("catPossessing");
    _player = Player::alloc(150, 0, 0, cat);

    //floor texture creation
    std::shared_ptr<Texture> wall = _assets->get<Texture>("levelWall");

    std::shared_ptr<Texture> floor = _assets->get<Texture>("levelFloor");
    //Staircase door texture creation
    std::shared_ptr<Texture> staircaseDoor = _assets->get<Texture>("staircaseDoor");
    //Door texture creation
    std::shared_ptr<Texture> door = _assets->get<Texture>("door");
    
    std::shared_ptr<Texture> doorFrame = _assets->get<Texture>("doorFrame");
    std::shared_ptr<Texture> catDen = _assets->get<Texture>("catDen");

    //caged animal
    int s = 1;
    std::shared_ptr<Texture> cagedAnimal = _assets->get<Texture>("cagedAnimal");
    _level1Wall = Wall::alloc(550, 0, Vec2(s, s), 0, cugl::Color4::WHITE, 1,1, wall);
    _level2Wall = Wall::alloc(550, 0, Vec2(s, s), 1, cugl::Color4::WHITE, 1, 1, wall);
    _level1CatDenLeft = CatDen::alloc(800, 0, Vec2(0.05, 0.05), 0, cugl::Color4::WHITE, 1, 1, catDen);
    _level1CatDenRight = CatDen::alloc(150, 0, Vec2(0.05, 0.05), 0, cugl::Color4::WHITE, 1, 1, catDen);
    _catDens = { _level1CatDenLeft,_level1CatDenRight };
    _level1Floor = Floor::alloc(555, 0, Vec2(s, s), 0, cugl::Color4::WHITE, 1, 1, floor);
    _level2Floor = Floor::alloc(555, 0, Vec2(s, s), 1, cugl::Color4::WHITE, 1, 1, floor);
    _level1StairDoor = StaircaseDoor::alloc(950, 0, Vec2(1, 1), 0, cugl::Color4::WHITE, { 1 }, 1, 8, staircaseDoor);
    _level2StairDoor = StaircaseDoor::alloc(550, 0, Vec2(1, 1), 1, cugl::Color4::WHITE, { 1 }, 1, 8, staircaseDoor);
    _staircaseDoors = { _level1StairDoor, _level2StairDoor};
    _level1Door = Door::alloc(590,0, Vec2(1, 1), 0,cugl::Color4::WHITE, { 1 }, 1, 8, door);
    _level1DoorFrame = DoorFrame::alloc(515, 0, Vec2(1.0, 1), 0, cugl::Color4::WHITE, { 1 }, 1, 8, doorFrame);

    _doors = { _level1Door};
    _cagedAnimal = CagedAnimal::alloc(820, 0, Vec2(0.3, 0.3), 1, cugl::Color4::WHITE, { 1 }, 1, 1, cagedAnimal);
    // Enemy creation
    _enemyController = make_shared<EnemyController>();
    enemyTexture = _assets->get<Texture>("enemy");
    std::shared_ptr<Texture> altTexture = _assets->get<Texture>("possessed-enemy");
    enemyHighlightTexture = _assets->get<Texture>("enemy-glow");

    _enemyController->addEnemy(400, 0,  0, { 1 }, 400, 400, enemyTexture, altTexture, enemyHighlightTexture);
    _enemyController->addEnemy(650, 0, 0, { 1 }, 300, 900,  enemyTexture, altTexture, enemyHighlightTexture);


    //std::shared_ptr<Texture> altTexture = _assets->get<Texture>("possessed-enemy");
    //_enemyController->addEnemy(50, 1, 300, 800, 0, enemyTexture, altTexture);
    //_enemyController->addEnemy(50, 0, 50, 600, 0, enemyTexture, altTexture);

   
    // Find the safe area, adapting to the iPhone X
    Rect safe = Application::get()->getSafeBounds();
    safe.origin *= scale;
    safe.size *= scale;

    // Get the right and bottom offsets.
    float bOffset = safe.origin.y;
    float rOffset = (size.width) - (safe.origin.x + safe.size.width);
    
    // Text labels
    std::shared_ptr<Font> font = _assets->get<Font>("felt");
    _tutorialText = scene2::Label::alloc("Possess enemies to get past them, don't get spotted!", font);
    _tutorialText->setForeground(Color4::WHITE);
    _tutorialText->setScale(Vec2(0.5, 0.5));
    _tutorialText->setPosition(Vec2(60, 110));

    // Add the logo and button to the scene graph
    addChild(_rootScene);
    _rootScene->addChild(_level1Wall->getSceneNode());
    _rootScene->addChild(_level2Wall->getSceneNode());

    _rootScene->addChild(_cagedAnimal->getSceneNode());
    _rootScene->addChild(_level1Floor->getSceneNode());
    _rootScene->addChild(_level1CatDenLeft->getSceneNode());
    _rootScene->addChild(_level1CatDenRight->getSceneNode());
    _rootScene->addChild(_level1Door->getSceneNode());
    vector<std::shared_ptr<Enemy>> enemies = _enemyController->getEnemies();
    _rootScene->addChild(_level2Floor->getSceneNode());

    _rootScene->addChild(_level1StairDoor->getSceneNode());
    _rootScene->addChild(_level2StairDoor->getSceneNode());
    for (auto it = begin(enemies); it != end(enemies); ++it) {
        _rootScene->addChild(it->get()->getSceneNode());
        _rootScene->addChild(it->get()->getPatrolNode());
    }
    _rootScene->addChild(_level1DoorFrame->getSceneNode());

    _rootScene->addChild(_player->getSceneNode());
    //_rootScene->getChildren()[]
    //every time the level changes draw the player than draw the door frame 


    _rootScene->addChild(_tutorialText);

    // make possess panel
    _possessPanel = ui::PanelElement::alloc(0, 0, 0, _assets->get<Texture>("possessCounter"));
    _possessPanel->getSceneNode()->setAnchor(Vec2::ANCHOR_TOP_RIGHT);
    _possessPanel->getSceneNode()->setScale(0.15f);
    _possessPanel->getSceneNode()->setPosition(size);
    _possessPanel->createChildText(200, 0, 10, 10, "x " + to_string(_player->get_nPossess()), font);
    _possessPanel->getChildTexts()[0]->setScale(Vec2(5.0f, 5.0f));
    addChild(_possessPanel->getSceneNode());

    _menuButton = ui::ButtonElement::alloc(0, 0, 0, 0, ui::ButtonState::AVAILABLE);
    _menuButton->setTexture(_assets->get<Texture>("menuButton"));
    _menuButton->getButton()->setAnchor(Vec2::ANCHOR_TOP_LEFT);
    _menuButton->getButton()->setScale(0.3f);
    _menuButton->getButton()->setPosition(0,size.height);
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
    _menuPanel = ui::PanelElement::alloc(size.width / 2, size.height / 2, 0, _assets->get<Texture>("levelCompleteBG"));
    _menuPanel->getSceneNode()->setScale(0.75f);
    _menuPanel->setVisible(false);
    _menuPanel->createChildPanel(0, 160, 0, _assets->get<Texture>("paused"));
    _menuPanel->getChildPanels()[0]->getSceneNode()->setScale(0.3f);
    _menuPanel->createChildButton(0, -150, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("resume"), Color4f::WHITE);
    _menuPanel->getChildButtons()[0]->getButton()->setName("resume");
    _menuPanel->getChildButtons()[0]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            setGameStatus(GameStatus::RUNNING);
        }
        });
    _menuPanel->createChildButton(0, -220, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("restartLevel"), Color4f::WHITE);
    _menuPanel->getChildButtons()[1]->getButton()->setName("restartLevel");
    _menuPanel->getChildButtons()[1]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _reset = true;
        }
        });
    _menuPanel->createChildButton(0, -300, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("returnToMenu"), Color4f::WHITE);
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
    _winPanel->getSceneNode()->setScale(0.75f);
    _winPanel->setVisible(false);
    _winPanel->createChildPanel(0, 160, 0, _assets->get<Texture>("winIcon"));
    _winPanel->getChildPanels()[0]->getSceneNode()->setScale(0.8f);
    _winPanel->createChildPanel(0, -45, 0, _assets->get<Texture>("winTitle"));
    _winPanel->getChildPanels()[1]->getSceneNode()->setScale(1.2f);
    _winPanel->createChildPanel(135, 10, 0, _assets->get<Texture>("deadOrAlive"));
    _winPanel->getChildPanels()[2]->getSceneNode()->setScale(1.2f);
    _winPanel->createChildButton(0, -160, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("nextLevel"), Color4f::WHITE);
    _winPanel->getChildButtons()[0]->getButton()->setName("nextLevel");
    _winPanel->getChildButtons()[0]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _nextLevel = true;
        }
        });
    _winPanel->createChildButton(0, -220, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("retry"), Color4f::WHITE);
    _winPanel->getChildButtons()[1]->getButton()->setName("retry");
    _winPanel->getChildButtons()[1]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _reset = true;
        }
        });
    _winPanel->createChildButton(0, -280, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("menu"), Color4f::WHITE);

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
    _losePanel->getSceneNode()->setScale(0.75f);
    _losePanel->setVisible(false);
    _losePanel->createChildPanel(0, 160, 0, _assets->get<Texture>("loseIcon"));
    _losePanel->getChildPanels()[0]->getSceneNode()->setScale(0.8f);
    _losePanel->createChildPanel(0, -45, 0, _assets->get<Texture>("loseTitle"));
    _losePanel->getChildPanels()[1]->getSceneNode()->setScale(1.2f);
    _losePanel->createChildPanel(0, 100, 0, _assets->get<Texture>("wasted"));
    _losePanel->getChildPanels()[2]->getSceneNode()->setScale(0.5f);
    _losePanel->createChildButton(0, -170, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("retry"), Color4f::WHITE);
    _losePanel->getChildButtons()[0]->getButton()->setName("retry");
    _losePanel->getChildButtons()[0]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _reset = true;
        }
        });
    _losePanel->createChildButton(0, -230, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("menu"), Color4f::WHITE);
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
}

void GameplayMode::buildScene(std::shared_ptr<JsonValue> json) {
    //clearRootSceneNode();
    Size  size = Application::get()->getDisplaySize();
    float scale = GAME_WIDTH / size.width;
    size *= scale;

    _hasControl = true;
    _doors.clear();
    _staircaseDoors.clear();
    _catDens.clear();

    std::shared_ptr<Texture> cat = _assets->get<Texture>("cat-walking");
    //floor texture creation
    std::shared_ptr<Texture> wall = _assets->get<Texture>("levelWall");

    std::shared_ptr<Texture> floor = _assets->get<Texture>("levelFloor");
    //Staircase door texture creation
    std::shared_ptr<Texture> staircaseDoor = _assets->get<Texture>("staircaseDoor");
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

    //JSON PROCESSING
    shared_ptr<JsonValue> playerJSON = json->get("player");
    shared_ptr<JsonValue> enemiesJSON = json->get("enemy");
    shared_ptr<JsonValue> staircaseDoorJSON = json->get("staircase-door");
    shared_ptr<JsonValue> doorJSON = json->get("door");
    shared_ptr<JsonValue> decorationsJSON = json->get("decorations");
    shared_ptr<JsonValue> objectTemp;
    if (playerJSON != nullptr) {
        string numPossessions = to_string(playerJSON->getInt("num_possessions"));
        _player = Player::alloc(playerJSON->getFloat("x_pos"), playerJSON->getInt("level"), 0, cat);

    }
    if (enemiesJSON != nullptr) {
        for (int i = 0; i < enemiesJSON->size(); i++) {
            objectTemp = enemiesJSON->get(i);
            shared_ptr<JsonValue> keyArray = objectTemp->get("keys");
            vector<int> key = {};
            if (!keyArray->isNull()) {
                for (int j = 0; j < keyArray->size(); j++) {
                    key.push_back(stoi(keyArray->get(j)->toString()));
                }
            }
            _enemyController->addEnemy(objectTemp->getFloat("x_pos"), objectTemp->getInt("level"), 0, 
                key, objectTemp->getFloat("patrol_start"), objectTemp->getFloat("patrol_end"), enemyTexture, altTexture, enemyHighlightTexture);
        }
    }
    if (staircaseDoorJSON != nullptr) {
        for (int i = 0; i < staircaseDoorJSON->size(); i++) {
            objectTemp = staircaseDoorJSON->get(i);
            _staircaseDoors.push_back(StaircaseDoor::alloc(objectTemp->getFloat("x_pos"), 0, Vec2(1,1), objectTemp->getInt("level"),
                cugl::Color4::WHITE, {1}, 1, 8, staircaseDoor));
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
            _level1DoorFrame = DoorFrame::alloc(objectTemp->getFloat("x_pos")-75, 0, Vec2(1.0, 1), 1, cugl::Color4::WHITE, { 1 }, 1, 8, doorFrame);
        }
    }
    if (decorationsJSON != nullptr) {
        for (int i = 0; i < decorationsJSON->size(); i++) {
            objectTemp = decorationsJSON->get(i);
            _cagedAnimal = CagedAnimal::alloc(objectTemp->getFloat("x_pos"), 0, Vec2(0.3, 0.3), objectTemp->getInt("level"),
                cugl::Color4::WHITE, { }, 1, 1, cagedAnimal);
            
        }
    }

    int s = 1.4;
    _level1Wall = Wall::alloc(550, 0, Vec2(s, s), 0, cugl::Color4::WHITE, 1, 1, wall);
    _level2Wall = Wall::alloc(550, 0, Vec2(s, s), 1, cugl::Color4::WHITE, 1, 1, wall);
    _level1Floor = Floor::alloc(555, 0, Vec2(s, s), 0, cugl::Color4::WHITE, 1, 1, floor);
    _level2Floor = Floor::alloc(555, 0, Vec2(s, s), 1, cugl::Color4::WHITE, 1, 1, floor);


    Rect safe = Application::get()->getSafeBounds();
    safe.origin *= scale;
    safe.size *= scale;


    float bOffset = safe.origin.y;
    float rOffset = (size.width) - (safe.origin.x + safe.size.width);


    vector<std::shared_ptr<Enemy>> enemies = _enemyController->getEnemies();

    _player->getSceneNode()->setName("Player");

    _rootScene->addChild(_level1Wall->getSceneNode());
    _rootScene->addChild(_level2Wall->getSceneNode());
    _level1Floor->getSceneNode()->setName("Level1Floor");
    _level2Floor->getSceneNode()->setName("Level2Floor");
    _rootScene->addChild(_level1Floor->getSceneNode());
    _rootScene->addChild(_level2Floor->getSceneNode());

    for (auto it = begin(_staircaseDoors); it != end(_staircaseDoors); ++it) {
        _rootScene->addChild(it->get()->getSceneNode());
    }
    for (auto it = begin(_doors); it != end(_doors); ++it) {
        _rootScene->addChild(it->get()->getSceneNode());
    }
    for (auto it = begin(enemies); it != end(enemies); ++it) {
        _rootScene->addChild(it->get()->getSceneNode());
        _rootScene->addChild(it->get()->getPatrolNode());
    }
    _rootScene->addChild(_cagedAnimal->getSceneNode());
    _rootScene->addChild(_player->getSceneNode());
    _rootScene->addChild(_level1DoorFrame->getSceneNode());




    std::shared_ptr<Font> font = _assets->get<Font>("felt");

    addChild(_rootScene);

    // make possess panel
    _possessPanel = ui::PanelElement::alloc(0, 0, 0, _assets->get<Texture>("possessCounter"));
    _possessPanel->getSceneNode()->setAnchor(Vec2::ANCHOR_TOP_RIGHT);
    _possessPanel->getSceneNode()->setScale(0.15f);
    _possessPanel->getSceneNode()->setPosition(size);
    _possessPanel->createChildText(200, 0, 10, 10, "x " + to_string(_player->get_nPossess()), font);
    _possessPanel->getChildTexts()[0]->setScale(Vec2(5.0f, 5.0f));
    addChild(_possessPanel->getSceneNode());

    _menuButton = ui::ButtonElement::alloc(0, 0, 0, 0, ui::ButtonState::AVAILABLE);
    _menuButton->setTexture(_assets->get<Texture>("menuButton"));
    _menuButton->getButton()->setAnchor(Vec2::ANCHOR_TOP_LEFT);
    _menuButton->getButton()->setScale(0.3f);
    _menuButton->getButton()->setPosition(0, size.height);
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
    _menuPanel = ui::PanelElement::alloc(size.width / 2, size.height / 2, 0, _assets->get<Texture>("levelCompleteBG"));
    _menuPanel->getSceneNode()->setScale(0.75f);
    _menuPanel->setVisible(false);
    _menuPanel->createChildPanel(0, 160, 0, _assets->get<Texture>("paused"));
    _menuPanel->getChildPanels()[0]->getSceneNode()->setScale(0.3f);
    _menuPanel->createChildButton(0, -150, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("resume"), Color4f::WHITE);
    _menuPanel->getChildButtons()[0]->getButton()->setName("resume");
    _menuPanel->getChildButtons()[0]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            setGameStatus(GameStatus::RUNNING);
        }
    });
    _menuPanel->createChildButton(0, -220, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("restartLevel"), Color4f::WHITE);
    _menuPanel->getChildButtons()[1]->getButton()->setName("restartLevel");
    _menuPanel->getChildButtons()[1]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _reset = true;
        }
    });
    _menuPanel->createChildButton(0, -300, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("returnToMenu"), Color4f::WHITE);
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
    _winPanel->getSceneNode()->setScale(0.75f);
    _winPanel->setVisible(false);
    _winPanel->createChildPanel(0, 160, 0, _assets->get<Texture>("winIcon"));
    _winPanel->getChildPanels()[0]->getSceneNode()->setScale(0.8f);
    _winPanel->createChildPanel(0, -45, 0, _assets->get<Texture>("winTitle"));
    _winPanel->getChildPanels()[1]->getSceneNode()->setScale(1.2f);
    _winPanel->createChildPanel(135, 10, 0, _assets->get<Texture>("deadOrAlive"));
    _winPanel->getChildPanels()[2]->getSceneNode()->setScale(1.2f);
    _winPanel->createChildButton(0, -160, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("nextLevel"), Color4f::WHITE);
    _winPanel->getChildButtons()[0]->getButton()->setName("nextLevel");
    _winPanel->getChildButtons()[0]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _nextLevel = true;
        }
        });
    _winPanel->createChildButton(0, -220, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("retry"), Color4f::WHITE);
    _winPanel->getChildButtons()[1]->getButton()->setName("retry");
    _winPanel->getChildButtons()[1]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _reset = true;
        }
        });
    _winPanel->createChildButton(0, -280, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("menu"), Color4f::WHITE);

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
    _losePanel->getSceneNode()->setScale(min(size.width / losePanel->getSize().width, size.height / losePanel->getSize().height));
    _losePanel->setVisible(false);
    _losePanel->createChildButton(0, -170, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("retry"), Color4f::WHITE);
    _losePanel->getChildButtons()[0]->getButton()->setName("retry");
    _losePanel->getChildButtons()[0]->getButton()->setScale(8.0f);
    _losePanel->getChildButtons()[0]->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _reset = true;
        }
        });
    _losePanel->createChildButton(0, -230, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("menu"), Color4f::WHITE);
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
}


void GameplayMode::checkDoors() {
    for (shared_ptr<Door> door : _doors) {
        bool doorState = door->getIsOpen();
        if (_enemyController->getPossessed() != nullptr) {
            if (abs(_enemyController->getPossessed()->getSceneNode()->getWorldPosition().x - door->getSceneNode()->getWorldPosition().x) < 110.0f * _inputManager->getRootSceneNode()->getScaleX() &&
                abs(screenToWorldCoords(_inputManager->getTapPos()).y - door->getSceneNode()->getWorldPosition().y+25) < 110.0f * _inputManager->getRootSceneNode()->getScaleY() &&
                _enemyController->getPossessed()->getLevel() == door->getLevel() &&

                abs(screenToWorldCoords(_inputManager->getTapPos()).x - door->getSceneNode()->getWorldPosition().x) < 60.0f * _inputManager->getRootSceneNode()->getScaleX()) {
                std::vector<int> intersect;
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
                    if (_json == nullptr) {
                        _tutorialText->setText("Click on the staircase door to enter the staircase and click on a connected door to leave");
                        _tutorialText->setPosition(Vec2(100, 110));
                    }
                }

            }
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
            if (visibility && abs(_enemyController->getPossessed()->getSceneNode()->getWorldPosition().x - staircaseDoor->getSceneNode()->getWorldPosition().x) < 110.0f * _inputManager->getRootSceneNode()->getScaleX() &&
                abs(screenToWorldCoords(_inputManager->getTapPos()).y - staircaseDoor->getSceneNode()->getWorldPosition().y-20) < 110.0f * _inputManager->getRootSceneNode()->getScaleY() &&
                _enemyController->getPossessed()->getLevel() == staircaseDoor->getLevel() &&

                abs(screenToWorldCoords(_inputManager->getTapPos()).x - staircaseDoor->getSceneNode()->getWorldPosition().x) < 60.0f * _inputManager->getRootSceneNode()->getScaleX()&&
                !key_intersection.empty()) {

                _enemyController->getPossessed()->getSceneNode()->setVisible(!visibility);
                staircaseDoor->setDoor(!staircaseDoor->getIsOpen());
                
                //std::dynamic_pointer_cast<scene2::AnimationNode>(staircaseDoor->getSceneNode())->setFrame(4);
                break;
            }

            else if (!visibility &&
                abs(screenToWorldCoords(_inputManager->getTapPos()).y - staircaseDoor->getSceneNode()->getWorldPosition().y-20) < 110.0f * _inputManager->getRootSceneNode()->getScaleY() &&
                abs(screenToWorldCoords(_inputManager->getTapPos()).x - staircaseDoor->getSceneNode()->getWorldPosition().x) < 60.0f * _inputManager->getRootSceneNode()->getScaleX()) {
                _enemyController->getPossessed()->getSceneNode()->setVisible(!visibility);
                _enemyController->getPossessed()->setPos(staircaseDoor->getPos().x);
                _enemyController->getPossessed()->setLevel(staircaseDoor->getLevel());
                //ChangeDrawOrder();
                //ChangeDrawOrder();
                staircaseDoor->setDoor(!staircaseDoor->getIsOpen());
                if (_json == nullptr) {
                    _tutorialText->setText("Touch the cage in cat form to release the animals and complete the level");
                    _tutorialText->setPosition(Vec2(200, 420));
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
            if (visibility && abs(_player->getSceneNode()->getWorldPosition().x - catDen->getSceneNode()->getWorldPosition().x) < 110.0f * _inputManager->getRootSceneNode()->getScaleX() &&
                abs(screenToWorldCoords(_inputManager->getTapPos()).y - catDen->getSceneNode()->getWorldPosition().y) < 80.0f * _inputManager->getRootSceneNode()->getScaleY() &&
                _player->getLevel() == catDen->getLevel() &&
                abs(screenToWorldCoords(_inputManager->getTapPos()).x - catDen->getSceneNode()->getWorldPosition().x) < 60.0f * _inputManager->getRootSceneNode()->getScaleX()) {
                _player->getSceneNode()->setVisible(!visibility);
                //std::dynamic_pointer_cast<scene2::AnimationNode>(staircaseDoor->getSceneNode())->setFrame(4);
                break;
            }

            else if (!visibility &&
                abs(screenToWorldCoords(_inputManager->getTapPos()).y - catDen->getSceneNode()->getWorldPosition().y) < 80.0f * _inputManager->getRootSceneNode()->getScaleY() &&
                abs(screenToWorldCoords(_inputManager->getTapPos()).x - catDen->getSceneNode()->getWorldPosition().x) < 60.0f * _inputManager->getRootSceneNode()->getScaleX()) {
                _player->getSceneNode()->setVisible(!visibility);
                _player->setPos(catDen->getPos().x);
                _player->setLevel(catDen->getLevel());
                if (_json == nullptr) {
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
        return to_string(_locationIndex) + "_" + to_string(_levelIndex + 1);
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
            tempObject->appendChild("keys", tempArray);
            enemyArray->appendChild(tempObject);
        }
        for (auto it = begin(_catDens); it != end(_catDens); ++it) {
            shared_ptr<JsonValue> tempObject = JsonValue::allocObject();
            tempObject->appendChild("x_pos", JsonValue::alloc(it->get()->getPos().x));
            tempObject->appendChild("level", JsonValue::alloc((long)it->get()->getLevel()));
            tempObject->appendChild("connection", JsonValue::alloc((long)0)); //TODO: HARDCODED BECAUSE CURRENT THERE IS NO FIELD FOR IT
            tempObject->appendChild("isDen", JsonValue::alloc(true));
            staircaseDoorArray->appendChild(tempObject);
        }
        for (auto it = begin(_staircaseDoors); it != end(_staircaseDoors); ++it) {
            shared_ptr<JsonValue> tempObject = JsonValue::allocObject();
            tempObject->appendChild("x_pos", JsonValue::alloc(it->get()->getPos().x));
            tempObject->appendChild("level", JsonValue::alloc((long)it->get()->getLevel()));
            tempObject->appendChild("connection", JsonValue::alloc((long)0)); //TODO: HARDCODED BECAUSE CURRENT THERE IS NO FIELD FOR IT
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
        tempObject->appendChild("x_pos", JsonValue::alloc((long)_cagedAnimal->getPos().x));
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
        result->appendChild("floor", JsonValue::alloc((long)1));

        shared_ptr<JsonWriter> writer = JsonWriter::alloc("levels/save.json");
        writer->writeJson(result);
        writer->close();
    }
}


