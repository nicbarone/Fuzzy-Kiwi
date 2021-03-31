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
// This is adjusted by screen aspect ratio to get the height
#define GAME_WIDTH 1024

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
void GameplayMode::onStartup() {
    Size size = getDisplaySize();
    size *= GAME_WIDTH / size.width;

    // Create a scene graph the same size as the window
    _scene = Scene2::alloc(size.width, size.height);
    _rootScene = scene2::SceneNode::alloc();
    _rootScene->setAnchor(Vec2::ANCHOR_CENTER);
    _rootScene->setContentSize(getSafeBounds().size);

    // Create a sprite batch (and background color) to render the scene
    _batch = SpriteBatch::alloc();
    setClearColor(Color4(229, 229, 229, 255));

    // Create an asset manager to load all assets
    _assets = AssetManager::alloc();

    // You have to attach the individual loaders for each asset type
    _assets->attach<Texture>(TextureLoader::alloc()->getHook());
    _assets->attach<Font>(FontLoader::alloc()->getHook());

    // This reads the given JSON file and uses it to load all other assets
    _assets->loadDirectory("json/assets.json");

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
    Application::onStartup();

    // Report the safe area
    Rect bounds = Display::get()->getSafeBounds();
    // CULog("Safe Area %sx%s",bounds.origin.toString().c_str(),
     //                        bounds.size.toString().c_str());

    bounds = getSafeBounds();
    //CULog("Safe Area %sx%s",bounds.origin.toString().c_str(),
    //                        bounds.size.toString().c_str());

    bounds = getDisplayBounds();
    //CULog("Full Area %sx%s",bounds.origin.toString().c_str(),
    //                        bounds.size.toString().c_str());

}

/**
 * The method called when the application is ready to quit.
 *
 * This is the method to dispose of all resources allocated by this
 * application.  As a rule of thumb, everything created in onStartup()
 * should be deleted here.
 *
 * When overriding this method, you should call the parent method as the
 * very last line.  This ensures that the state will transition to NONE,
 * causing the application to be deleted.
 */
void GameplayMode::onShutdown() {
    // Delete all smart pointers
    _scene = nullptr;
    _batch = nullptr;
    _assets = nullptr;

    // Deativate input
#if defined CU_TOUCH_SCREEN
    Input::deactivate<Touchscreen>();
#else
    Input::deactivate<Mouse>();
#endif
    Application::onShutdown();
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
    // update camera
    _scene->getCamera()->update();
    // Read input controller input
    _inputManager.readInput();

    if (_enemyController->closestEnemy() != nullptr && _player->canPossess()) {
        //very temporary modification to test whether it works, dont want to work with highlight right now
        _enemyController->closestEnemy()->setGlow(true);
    }
    if (_inputManager.getTapPos().x != 0) {
        /*CULog("x: %f, y: %f", _inputManager.getTapPos().x, _inputManager.getTapPos().x);
        CULog("x: %f, y: %f", _level1StairDoor->getPos().x, _level1StairDoor->getPos().y);*/
        //CULog("x: %f", abs(_player->getPos().x - _level1StairDoor->getPos().x));
        //CULog("Is possessing: %d", _player->getPossess());
        //CULog("Enemy position: %d",   _enemyController->getPossessed()->getPos().x);

    }    //546 546

    Size  size = getDisplaySize();
    float scale = GAME_WIDTH / size.width;
    size *= scale;
    Rect safe = getSafeBounds();
    safe.origin *= scale;
    safe.size *= scale;
    Size bpsize = possessButton->getSize();
    // Get the right and bottom offsets.
    float bOffset = safe.origin.y;
    float rOffset = (size.width) - (safe.origin.x + safe.size.width);
    // Check if we need to set the possess button to grey, or turn the button to colored
    if (_possessButton->getButtonState() == ui::ButtonState::POSSESS) {
        if (_enemyController->closestEnemy() == nullptr || !_player->canPossess()) {
            // turn the button to grey
            _possessButton->getButton()->setColor(Color4f(1.0f, 1.0f, 1.0f, 0.5f));
            _possessButton->setButtonState(ui::ButtonState::UNAVAILABLE);
            
        }
    }
    else if (_possessButton->getButtonState() == ui::ButtonState::UNAVAILABLE) {
        if (_enemyController->closestEnemy() != nullptr && _player->canPossess()) {
            // turn the button to colored
            _possessButton->getButton()->setColor(Color4f::WHITE);
            _possessButton->setButtonState(ui::ButtonState::POSSESS);
        }
    }
    // Any clicks on Unavailable buttons should be ignored
    if (_possessButton->getButtonState() == ui::ButtonState::UNAVAILABLE) {
        _possessButton->setClicked(false);
    }
    // Check if possess button is clicked
    if (_possessButton->getClicked()) {
        _possessButton->getButton()->deactivate();
        _scene->removeChild(_possessButton->getButton());
        if (_possessButton->getButtonState() == ui::ButtonState::POSSESS) {
            if (attemptPossess()) {
                _possessButton->setTexture(unpossessButton);
                _possessButton->setButtonState(ui::ButtonState::UNPOSSESS);
                _tutorialText->setText("You can open the door while possessing an enemy and can only be detected from the back");
                _tutorialText->setPosition(Vec2(100, 200));
            }
        }
        else {
            unpossess();
            _possessButton->setTexture(possessButton);
            _possessButton->setButtonState(ui::ButtonState::POSSESS);
        }
        _possessButton->getButton()->setName("possess");
        _possessButton->getButton()->addListener([=](const std::string& name, bool down) {
            // Only quit when the button is released
            if (!down) {
                //CULog("Clicking on possess button!");
                // Mark this button as clicked, proper handle will take place in update()
                _possessButton->setClicked(true);
            }
            });
        _possessButton->getButton()->setAnchor(Vec2::ANCHOR_CENTER);
        _possessButton->setPos(Vec2(size.width - (bpsize.width + rOffset) / 2, (bpsize.height + bOffset) / 2));
        _scene->addChild(_possessButton->getButton());
        _possessButton->getButton()->activate();
        _possessButton->setClicked(false);
    }


    checkStaircaseDoors();
    checkDoors();
    collisions::checkForDoorCollision(_enemyController->getPossessed(), _enemyController->getEnemies(), _player, _doors);
    int cageCollision = collisions::checkForCagedAnimalCollision(_player, _cagedAnimal);
    if (cageCollision != 0) {
        // shows win Panel
        _winPanel->setVisible(true);
    }
    collisions::checkInBounds(_enemyController->getPossessed(),_player);
    string numPossessions = to_string(_player->get_nPossess());
    _numberOfPossessions->setText("Number of possessions left : "+ numPossessions);

    /**possess code works a bit better when movement is processed last (scene node position is updated here)
        else you get one frame of wrong position*/
        // For now, if possessing, disable cat movement, put it to the same location as the possessed enemy
    if (_player->getPossess()) {
        _player->setPos(_player->get_possessEnemy()->getPos());
    }
    else {
        _player->move(_inputManager.getForward());
    }
    // Enemy movement
    _enemyController->moveEnemies(_inputManager.getForward());
    _enemyController->findClosest(_player->getPos(), _player->getLevel(), closedDoors());

    if (_enemyController->getPossessed() != nullptr) {
        //CULog("%d", _enemyController->getPossessed()->facingRight());
    }
    if (_enemyController->detectedPlayer(_player->getPos(), _player->getLevel(), closedDoors())) {
        _player->getSceneNode()->setAngle(3.14159265358979f);
        if (_enemyController->getPossessed() != nullptr) {
            _enemyController->getPossessed()->getSceneNode()->setAngle(3.14159265358979f);
        }
        
    }
    else {
        _player->getSceneNode()->setAngle(0);
        if (_enemyController->getPossessed() != nullptr) {
            _enemyController->getPossessed()->getSceneNode()->setAngle(0);
        }
    }

}

bool GameplayMode::attemptPossess() {
    std::shared_ptr<Enemy> enemy = _enemyController->closestEnemy();
    if (enemy != nullptr) {
        vector<Vec2> doors = closedDoors();
        _player->getSceneNode()->setVisible(false);
        _player->setPossess(true);
        _player->set_possessEnemy(enemy);
        _enemyController->updatePossessed(enemy);
        enemy->setPossessed();
        enemy->setGlow(false);
        return true;
    }
    return false;
}

void GameplayMode::unpossess() {
    std::shared_ptr<Enemy> enemy = _enemyController->getPossessed();
    _player->setPos((enemy->getPos()));
    _player->setLevel(enemy->getLevel());
    _player->getSceneNode()->setVisible(true);
    _player->setPossess(false);
    _player->setLevel(_enemyController->getPossessed()->getLevel());
    enemy->getSceneNode()->setVisible(false);
    enemy->dispose();

    //may want to remove the enemy from the vector in enemy controller eventually, seems good for now
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
    _scene->render(_batch);
}

/**
 * Internal helper to build the scene graph.
 *
 * Scene graphs are not required.  You could manage all scenes just like
 * you do in 3152.  However, they greatly simplify scene management, and
 * have become standard in most game engines.
 */
void GameplayMode::buildScene() {
    Size  size = getDisplaySize();
    float scale = GAME_WIDTH / size.width;
    size *= scale;



    // Placeholder cat
    //std::shared_ptr<Texture> cat = _assets->get<Texture>("cat-walking");
    std::shared_ptr<Texture> cat = _assets->get<Texture>("cat-walking");
    // Create the player

    _player = Player::alloc(150, 0, 0, cat);


    //floor texture creation
    std::shared_ptr<Texture> floor = _assets->get<Texture>("levelFloors");
    //Staircase door texture creation
    std::shared_ptr<Texture> staircaseDoor = _assets->get<Texture>("staircaseDoor");
    //Door texture creation
    std::shared_ptr<Texture> door = _assets->get<Texture>("door");
    //caged animal
    std::shared_ptr<Texture> cagedAnimal = _assets->get<Texture>("cagedAnimal");
    _level1Floor = Floor::alloc(550, 0, Vec2(0.2, 0.2), 0, cugl::Color4::WHITE, 1,1, floor);
    _level2Floor = Floor::alloc(550, 0, Vec2(0.2, 0.2), 1, cugl::Color4::WHITE, 1, 1, floor);
    _level1StairDoor = StaircaseDoor::alloc(950, 0, Vec2(0.55, 0.55), 0, cugl::Color4::WHITE, 1, 8, staircaseDoor);
    _level2StairDoor = StaircaseDoor::alloc(550, 0, Vec2(0.55, 0.55), 1, cugl::Color4::WHITE, 1, 8, staircaseDoor);
    _staircaseDoors = { _level1StairDoor, _level2StairDoor };
    _level1Door = Door::alloc(590, 0, Vec2(0.65, 0.65), 0,cugl::Color4::WHITE, { 1 }, 1, 11, door);
    _doors = { _level1Door};
    _cagedAnimal = Door::alloc(820, 0, Vec2(0.3, 0.3), 1, cugl::Color4::WHITE, { 1 }, 1, 1, cagedAnimal);
    // Enemy creation
    _enemyController = make_shared<EnemyController>();
    enemyTexture = _assets->get<Texture>("enemy");
    std::shared_ptr<Texture> altTexture = _assets->get<Texture>("possessed-enemy");
    enemyHighlightTexture = _assets->get<Texture>("enemy-glow");
    _enemyController->addEnemy(400, 0,  0, { 1 }, 200, 200, enemyTexture, altTexture, enemyHighlightTexture);
    _enemyController->addEnemy(650, 0, 0, { 2 }, 300, 900,  enemyTexture, altTexture, enemyHighlightTexture);
    //std::shared_ptr<Texture> altTexture = _assets->get<Texture>("possessed-enemy");
    //_enemyController->addEnemy(50, 1, 300, 800, 0, enemyTexture, altTexture);
    //_enemyController->addEnemy(50, 0, 50, 600, 0, enemyTexture, altTexture);

    // Create a button.  A button has an up image and a down image
    possessButton = _assets->get<Texture>("possess-button");
    unpossessButton = _assets->get<Texture>("unpossess-button");
    Size pbsize = possessButton->getSize();
    // set up the ui element of possess button
    _possessButton = ui::ButtonElement::alloc(0, 0, 0, 0, ui::ButtonState::POSSESS);
    _possessButton->setTexture(possessButton);
    // Create a callback function for the button
    _possessButton->getButton()->setName("possess");
    _possessButton->getButton()->addListener([=](const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            //CULog("Clicking on possess button!");
            // Mark this button as clicked, proper handle will take place in update()
            _possessButton->setClicked(true);
        }
        });
    // Find the safe area, adapting to the iPhone X
    Rect safe = getSafeBounds();
    safe.origin *= scale;
    safe.size *= scale;

    // Get the right and bottom offsets.
    float bOffset = safe.origin.y;
    float rOffset = (size.width) - (safe.origin.x + safe.size.width);

    _possessButton->getButton()->setAnchor(Vec2::ANCHOR_CENTER);
    _possessButton->setPos(Vec2(size.width - (pbsize.width + rOffset) / 2 - 20, (pbsize.height + bOffset) / 2 + 60));
    
    // Text labels
    std::shared_ptr<Font> font = _assets->get<Font>("felt");
    _tutorialText = scene2::Label::alloc("Possess enemies to get past them, don't get spotted!", font);
    _tutorialText->setScale(Vec2(0.5, 0.5));
    _tutorialText->setPosition(Vec2(60, 200));
    string numPossessions = to_string(_player->get_nPossess());
    _numberOfPossessions = scene2::Label::alloc("Number of possessions left: "+ numPossessions, font);
    _numberOfPossessions->setScale(Vec2(0.5, 0.5));
    _numberOfPossessions->setPosition(Vec2(20, 540));

    // Add the logo and button to the scene graph
    _scene->addChild(_rootScene);
    _rootScene->addChild(_level1Floor->getSceneNode());
    _rootScene->addChild(_level1StairDoor->getSceneNode());
    _rootScene->addChild(_level2Floor->getSceneNode());
    _rootScene->addChild(_level2StairDoor->getSceneNode());
    _rootScene->addChild(_level1Door->getSceneNode());
   /* _rootScene->addChild(_leftWall->getSceneNode());
    _rootScene->addChild(_rightWall->getSceneNode());*/
    _rootScene->addChild(_cagedAnimal->getSceneNode());
    //_rootScene->addChild(_level2Door->getSceneNode());
    /*_rootScene->addChild(_numberOfPosessions->);*/
    _scene->addChild(_possessButton->getButton());
    _rootScene->addChild(_player->getSceneNode());
    _rootScene->addChild(_tutorialText);
    _scene->addChild(_numberOfPossessions);
    vector<std::shared_ptr<Enemy>> enemies = _enemyController->getEnemies();

    for (auto it = begin(enemies); it != end(enemies); ++it) {
        _rootScene->addChild(it->get()->getSceneNode());
        _rootScene->addChild(it->get()->getPatrolNode());
    }



    // We can only activate a button AFTER it is added to a scene
    _possessButton->getButton()->activate();

    // Create Win-Lose Panels
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
    _winPanel->createChildButton(0, -160, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("nextLevel"));
    _winPanel->createChildButton(0, -220, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("retry"));
    _winPanel->createChildButton(0, -280, 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("menu"));
    _scene->addChild(_winPanel->getSceneNode());

    // Initialize input manager
    _inputManager = InputManager();
    _inputManager.init(_player, _rootScene, _scene->getBounds());
}


void GameplayMode::checkDoors() {

    for (shared_ptr<Door> door : _doors) {
        bool doorState = door->getIsOpen();
        if (_enemyController->getPossessed() != nullptr) {
            std::vector<int> intersect;
            std::vector<int> v1 = _enemyController->getPossessed()->getKeys();
            std::vector<int> v2 = door->getKeys();
            std::sort(v1.begin(), v1.end());
            std::sort(v2.begin(), v2.end());

            std::vector<int> key_intersection;

            std::set_intersection(v1.begin(), v1.end(),
                v2.begin(), v2.end(),
                std::back_inserter(key_intersection));
            //set_intersection(_enemyController->getPossessed()->getKeys().begin(), _enemyController->getPossessed()->getKeys().end(), door->getKeys().begin(), door->getKeys().end(), std::inserter(intersect, intersect.begin()));
            if (abs(_enemyController->getPossessed()->getSceneNode()->getWorldPosition().x - door->getSceneNode()->getWorldPosition().x) < 110.0f * _inputManager.getRootSceneNode()->getScaleX() &&
                abs(_scene->screenToWorldCoords(_inputManager.getTapPos()).y - door->getSceneNode()->getWorldPosition().y) < 80.0f * _inputManager.getRootSceneNode()->getScaleY() &&
                _enemyController->getPossessed()->getLevel() == door->getLevel() &&
                abs(_scene->screenToWorldCoords(_inputManager.getTapPos()).x - door->getSceneNode()->getWorldPosition().x) < 60.0f * _inputManager.getRootSceneNode()->getScaleX()&&
                !key_intersection.empty()) {
                door->setDoor(!doorState);
                _tutorialText->setText("Click on the staircase door to enter the staircase and click on a connected door to leave");
                _tutorialText->setPosition(Vec2(100, 200));
            }
        }
    }

}

void GameplayMode::checkStaircaseDoors() {

    bool visibility;

    if (_enemyController->getPossessed() != nullptr) {
        visibility = _enemyController->getPossessed()->getSceneNode()->isVisible();
        for (shared_ptr<StaircaseDoor> staircaseDoor : _staircaseDoors) {
            bool StaircasedoorState = staircaseDoor->getIsOpen();
            if (visibility && abs(_enemyController->getPossessed()->getSceneNode()->getWorldPosition().x - staircaseDoor->getSceneNode()->getWorldPosition().x) < 110.0f * _inputManager.getRootSceneNode()->getScaleX() &&
                abs(_scene->screenToWorldCoords(_inputManager.getTapPos()).y - staircaseDoor->getSceneNode()->getWorldPosition().y) < 80.0f * _inputManager.getRootSceneNode()->getScaleY() &&
                _enemyController->getPossessed()->getLevel() == staircaseDoor->getLevel() &&
                abs(_scene->screenToWorldCoords(_inputManager.getTapPos()).x - staircaseDoor->getSceneNode()->getWorldPosition().x) < 60.0f * _inputManager.getRootSceneNode()->getScaleX()) {
                _enemyController->getPossessed()->getSceneNode()->setVisible(!visibility);
                staircaseDoor->setDoor(!staircaseDoor);
                //std::dynamic_pointer_cast<scene2::AnimationNode>(staircaseDoor->getSceneNode())->setFrame(4);
                break;
            }

            else if (!visibility &&
                abs(_scene->screenToWorldCoords(_inputManager.getTapPos()).y - staircaseDoor->getSceneNode()->getWorldPosition().y) < 80.0f * _inputManager.getRootSceneNode()->getScaleY() &&
                abs(_scene->screenToWorldCoords(_inputManager.getTapPos()).x - staircaseDoor->getSceneNode()->getWorldPosition().x) < 60.0f * _inputManager.getRootSceneNode()->getScaleX()) {
                _enemyController->getPossessed()->getSceneNode()->setVisible(!visibility);
                _enemyController->getPossessed()->setPos(staircaseDoor->getPos().x);
                _enemyController->getPossessed()->setLevel(staircaseDoor->getLevel());
                staircaseDoor->setDoor(!staircaseDoor->getIsOpen());
                _tutorialText->setText("Touch the cage in cat form to release the animals and complete the level");
                _tutorialText->setPosition(Vec2(200, 510));
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


