//
//  HelloApp.h
//  Cornell University Game Library (CUGL)
//
//  This is the header for the custom application.  It is necessary so that
//  main.cpp can access your custom class.
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
#ifndef __GAMEPLAY_MODE_H__
#define __GAMEPLAY_MODE_H__
#include <cugl/cugl.h>
#include "Player.h"
#include "Floor.h"
#include "Door.h"
#include "EnemyController.h"
#include "InputManager.h"
#include "CollisionManager.h"
/**
 * Class for a simple Hello World style application
 *
 * The application simply moves the CUGL logo across the screen.  It also
 * provides a button to quit the application.
 */
class GameplayMode : public cugl::Application {
protected:
    /** The parent scene node for a level*/
    std::shared_ptr<cugl::scene2::SceneNode> _rootScene;
    /** The loaders to (synchronously) load in assets */
    std::shared_ptr<cugl::AssetManager> _assets;
    /** A scene graph, used to display our 2D scenes */
    std::shared_ptr<cugl::Scene2> _scene;
    /** A 3152 style SpriteBatch to render the scene */
    std::shared_ptr<cugl::SpriteBatch>  _batch;
    /** A reference to the enemy controller*/
    std::shared_ptr<EnemyController> _enemyController;
    /** A reference to the player*/
    std::shared_ptr<Player> _player;
    /** A reference to the level 1 floor*/
    std::shared_ptr<Floor> _level1Floor;
    /** A reference to the level 1 staircase door*/
    std::shared_ptr<Floor> _level1StairDoor;
    /** A reference to the level 2 floor*/
    std::shared_ptr<Floor> _level2Floor;
    /** A reference to the level 2 staircase door*/
    std::shared_ptr<Floor> _level2StairDoor;
    /** A reference to the list of all staircase doors in the level*/
    std::vector<shared_ptr<Floor>> _staircaseDoors;
    /** A reference to the level 1 door*/
    std::shared_ptr<Door> _level1Door;
    /** A reference to the level 2 door*/
    std::shared_ptr<Door> _level2Door;
    /** A reference to the list of all doors in the level*/
    std::vector<shared_ptr<Door>> _doors;
    InputManager _inputManager;
    /** The button to possess and release*/
    std::shared_ptr<ui::ButtonElement> _possessButton;
    // Create a button.  A button has an up image and a down image
    std::shared_ptr<Texture> possessButton;
    std::shared_ptr<Texture> unpossessButton;
    /** A countdown used to move the logo */
    int  _countdown;
    
    /** 
     * Internal helper to build the scene graph.
     *
     * Scene graphs are not required.  You could manage all scenes just like
     * you do in 3152.  However, they greatly simplify scene management, and
     * have become standard in most game engines.
     */
    void buildScene();
    
public:
    /**
     * Creates, but does not initialized a new application.
     *
     * This constructor is called by main.cpp.  You will notice that, like
     * most of the classes in CUGL, we do not do any initialization in the
     * constructor.  That is the purpose of the init() method.  Separation
     * of initialization from the constructor allows main.cpp to perform
     * advanced configuration of the application before it starts.
     */
    GameplayMode() : Application(), _countdown(-1) {}
    
    /**
     * Disposes of this application, releasing all resources.
     *
     * This destructor is called by main.cpp when the application quits. 
     * It simply calls the dispose() method in Application.  There is nothing
     * special to do here.
     */
    ~GameplayMode() { }
    
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
    void onStartup() override;

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
    void onShutdown() override;
    
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
    void update(float timestep) override;
    
    /** Used to check if there exists current possessable enemy in range */
    bool enemyInPossessRange();

    /** function called when the possess button is clicked
        returns true if an enemy is within range and possession is successful
        should it be in this file? who knows*/
    bool attemptPossess();

    /** function called when the unpossess button is clicked
        does whatever is necessary to unpossess, kind of a mess right now
    should it be in this file? who knows*/
    void unpossess();
    
    /**
     * The method called to draw the application to the screen.
     *
     * This is your core loop and should be replaced with your custom implementation.
     * This method should OpenGL and related drawing calls.
     *
     * When overriding this method, you do not need to call the parent method
     * at all. The default implmentation does nothing.
     */
    void draw() override;

    /*Function called every update to check if the player is trying to close or 
    open a door*/
    void checkDoors();

    /*Function called every update to check if the player is trying to exit
    or enter a staircase door*/
    void checkStaircaseDoors();

    
};

#endif /* __GAMEPLAY_MODE_H__ */
