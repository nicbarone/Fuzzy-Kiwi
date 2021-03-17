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
#include <cugl/base/CUBase.h>

// Add support for simple random number generation
#include <cstdlib>
#include <ctime>

// This keeps us from having to write cugl:: all the time
using namespace cugl;

// The number of frames before moving the logo to a new position
#define TIME_STEP 60
// This is adjusted by screen aspect ratio to get the height
#define GAME_WIDTH 1024

vector<Vec2> level1Floor = { Vec2(1100.0f, 0.0f),Vec2(0.0f, 0.0f), Vec2(0.0f, 60.0f), Vec2(1100.0f, 60.0f) };

vector<Vec2> level1Door = { Vec2(150.0f, 0.0f),Vec2(0.0f, 0.0f), Vec2(0.0f, 62.0f), Vec2(150.0f, 62.0f) };

vector<Vec2> level2Floor = { Vec2(1100.0f, 0.0f),Vec2(0.0f, 0.0f), Vec2(0.0f, 60.0f), Vec2(1100.0f, 60.0f) };

vector<Vec2> level2Door = { Vec2(150.0f, 0.0f),Vec2(0.0f, 0.0f), Vec2(0.0f, 62.0f), Vec2(150.0f, 62.0f) };



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
    size *= GAME_WIDTH/size.width;
    
    // Create a scene graph the same size as the window
    _scene = Scene2::alloc(size.width, size.height);
    // Create a sprite batch (and background color) to render the scene
    _batch = SpriteBatch::alloc();
    setClearColor(Color4(229,229,229,255));
    
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
    CULog("Safe Area %sx%s",bounds.origin.toString().c_str(),
                            bounds.size.toString().c_str());

    bounds = getSafeBounds();
    CULog("Safe Area %sx%s",bounds.origin.toString().c_str(),
                            bounds.size.toString().c_str());

    bounds = getDisplayBounds();
    CULog("Full Area %sx%s",bounds.origin.toString().c_str(),
                            bounds.size.toString().c_str());

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
    // Read input controller input
    _inputManager.readInput();


  
    if (_enemyController->closestEnemy() != nullptr && _player->canPossess()) {
        //very temporary modification to test whether it works, dont want to work with highlight right now
        _enemyController->closestEnemy()->getSceneNode()->setAngle(3.14159265358979f);
    }
    if (_inputManager.getTapPos().x != 0) {
        /*CULog("x: %f, y: %f", _inputManager.getTapPos().x, _inputManager.getTapPos().x);
        CULog("x: %f, y: %f", _level1Door->getPos().x, _level1Door->getPos().y);*/
        //CULog("x: %f", abs(_player->getPos().x - _level1Door->getPos().x));
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
            _possessButton->getButton()->setColor(Color4f(1.0f, 1.0f, 1.0f,0.5f));
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
    

    //CULog("%i", _level1Door->getSceneNode()->getPositionX());
    //CULog("%d", _level2Door->getPos().x);
    checkStaircaseDoors();
  
    
    /**possess code works a bit better when movement is processed last (scene node position is updated here)
        else you get one frame of wrong position*/
    _player->move(_inputManager.getForward());
    // Enemy movement
    _enemyController->moveEnemies(_inputManager.getForward());
    _enemyController->findClosest(_player->getPos());
}

bool GameplayMode::attemptPossess() {
    std::shared_ptr<Enemy> enemy = _enemyController->closestEnemy();
    if (enemy != nullptr) {
        _player->getSceneNode()->setVisible(false);
        _player->setPossess(true);
        _enemyController->updatePossessed(enemy);
        enemy->setPossessed();
        enemy->getSceneNode()->setAngle(0);
        return true;
    }
    return false;
}

void GameplayMode::unpossess() {
    std::shared_ptr<Enemy> enemy = _enemyController->getPossessed();
    _player->setPos((enemy->getPos()));
    _player->getSceneNode()->setVisible(true);
    _player->setPossess(false);
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
    Size  size  = getDisplaySize();
    float scale = GAME_WIDTH/size.width;
    size *= scale;
    

    
    // Placeholder cat
    std::shared_ptr<Texture> cat = _assets->get<Texture>("cat-placeholder");

    // Create the player
    _player = Player::alloc(150,128,0,cat);

    //floor texture creation
    std::shared_ptr<Texture> floor = _assets->get<Texture>("floor");
    //door texture creation
    std::shared_ptr<Texture> door = _assets->get<Texture>("door");
    _level1Floor = Floor::alloc(Vec2(550, 30), 0,Vec2(1,1), 1, cugl::Color4::WHITE, level1Floor, floor);

    _level1Door = Floor::alloc(Vec2(550, 130), 4.71239, Vec2(1,1), 1, cugl::Color4::WHITE, level1Door, door);

    

    _level2Floor = Floor::alloc(Vec2(540, 300), 0, Vec2(1, 1), 2, cugl::Color4::WHITE, level2Floor, floor);

    _level2Door = Floor::alloc(Vec2(550, 400), 4.71239, Vec2(1, 1), 2, cugl::Color4::WHITE, level2Door, door);


    // Enemy creation
    _enemyController = make_shared<EnemyController>();
    std::shared_ptr<Texture> enemyTexture = _assets->get<Texture>("enemy-placeholder");
    std::shared_ptr<Texture> altTexture = _assets->get<Texture>("possessed-enemy-placeholder");
    _enemyController->addEnemy(50, 400, 2, 0, enemyTexture, altTexture);
    _enemyController->addEnemy(50, 128, 1,0, enemyTexture, altTexture);

    // Create a button.  A button has an up image and a down image
    possessButton = _assets->get<Texture>("possess-button");
    unpossessButton = _assets->get<Texture>("unpossess-button");
    Size pbsize = possessButton->getSize();
    // set up the ui element of possess button
    _possessButton = ui::ButtonElement::alloc(0,0,0,0,ui::ButtonState::POSSESS);
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
    safe.size   *= scale;
    
    // Get the right and bottom offsets.
    float bOffset = safe.origin.y;
    float rOffset = (size.width)-(safe.origin.x+safe.size.width);

    _possessButton->getButton()->setAnchor(Vec2::ANCHOR_CENTER);
    _possessButton->setPos(Vec2(size.width - (pbsize.width + rOffset) / 2, (pbsize.height + bOffset) / 2));

    // Add the logo and button to the scene graph
    _scene->addChild(_level1Floor->getSceneNode()); 
    _scene->addChild(_level1Door->getSceneNode());
    _scene->addChild(_level2Floor->getSceneNode());
    _scene->addChild(_level2Door->getSceneNode());
    _scene->addChild(_possessButton->getButton());
    _scene->addChild(_player->getSceneNode());

    vector<std::shared_ptr<Enemy>> enemies = _enemyController->getEnemies();

    for (auto it = begin(enemies); it != end(enemies); ++it) {
        _scene->addChild(it->get()->getSceneNode());
    }

    
    
    // We can only activate a button AFTER it is added to a scene
    _possessButton->getButton()->activate();



    // Initialize input manager
    _inputManager = InputManager();
    _inputManager.init(_player, _scene->getBounds());
}

void GameplayMode::checkStaircaseDoors() {
  

    bool visibility;

    if (_enemyController->getPossessed() != nullptr) {
        
        /*CULog("%f", _enemyController->getPossessed()->getPos().y);*/
        if (_inputManager.touch2Screen(_inputManager.getTapPos()).y != 576) {
            CULog("%f", _inputManager.touch2Screen(_inputManager.getTapPos()).y);
       }
        


        visibility = _enemyController->getPossessed()->getSceneNode()->isVisible(); 
        if (visibility && abs(_enemyController->getPossessed()->getPos().x - _level1Door->getPos().x) < 110.0f &&
            abs(_inputManager.touch2Screen(_inputManager.getTapPos()).y - _level1Door->getPos().y) < 80.0f&&
            _enemyController->getPossessed()->getLevel() == _level1Door->getLevel()&&
            abs(_inputManager.touch2Screen(_inputManager.getTapPos()).x - _level1Door->getPos().x) < 60.0f) {
            _enemyController->getPossessed()->getSceneNode()->setVisible(!visibility);
            CULog("1");
        }

        else if (!visibility &&
            abs(_inputManager.touch2Screen(_inputManager.getTapPos()).y - _level2Door->getPos().y) < 80.0f &&
            abs(_inputManager.touch2Screen(_inputManager.getTapPos()).x - _level2Door->getPos().x) < 60.0f) {
            _enemyController->getPossessed()->getSceneNode()->setVisible(!visibility);
            _enemyController->getPossessed()->setPos(_level2Door->getPos());
            _enemyController->getPossessed()->changeFloor();
            _enemyController->getPossessed()->setLevel(_level2Door->getLevel());
            CULog("%d", _enemyController->getPossessed()->getLevel());
            CULog("2");
        }

        else if (!visibility &&
            abs(_inputManager.touch2Screen(_inputManager.getTapPos()).y - _level1Door->getPos().y) < 80.0f &&
            abs(_inputManager.touch2Screen(_inputManager.getTapPos()).x - _level1Door->getPos().x) < 60.0f) {
            _enemyController->getPossessed()->getSceneNode()->setVisible(!visibility);
            _enemyController->getPossessed()->setPos(_level1Door->getPos());
            _enemyController->getPossessed()->changeFloor();
            _enemyController->getPossessed()->setLevel(_level1Door->getLevel());
            CULog("3");
        }
        else if (visibility && 
            abs(_enemyController->getPossessed()->getPos().x - _level2Door->getPos().x) < 110.0f &&
            abs(_inputManager.touch2Screen(_inputManager.getTapPos()).y - _level2Door->getPos().y) < 80.0f &&
            _enemyController->getPossessed()->getLevel() == _level2Door->getLevel() &&
            abs(_inputManager.touch2Screen(_inputManager.getTapPos()).x - _level2Door->getPos().x) < 60.0f) {
            _enemyController->getPossessed()->getSceneNode()->setVisible(!visibility);
            CULog("4");
        }
        }
  //compare possesed enemy level and door level and see if it works 
    }
