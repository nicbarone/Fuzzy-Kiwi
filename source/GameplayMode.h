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
#include <algorithm>
#include <cugl/cugl.h>
#include "Player.h"
#include "Floor.h"
#include "Door.h"
#include "Wall.h"
#include "DoorFrame.h"
#include "CagedAnimal.h"
#include "StaircaseDoor.h"
#include "CatDen.h"
#include "EnemyController.h"
#include "InputManager.h"
#include "CollisionManager.h"


class GameplayMode : public cugl::Scene2 {
protected:
    bool _reset;
    bool _backToMenu;
    bool _nextLevel;
    int _levelIndex;

    std::shared_ptr<cugl::JsonValue> _json;
    /** The parent scene node for a level*/
    std::shared_ptr<cugl::scene2::OrderedNode> _rootScene;
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
    /** A reference to the player*/
    std::shared_ptr<Door> _possessingCat;

    /** A reference to the level 1 floor*/
    std::shared_ptr<Floor> _level1Floor;
    /** A reference to the level 1 staircase door*/
    std::shared_ptr<StaircaseDoor> _level1StairDoor;
    /** A reference to the level 2 floor*/
    std::shared_ptr<Floor> _level2Floor;
    /** A reference to the level 2 staircase door*/
    std::shared_ptr<StaircaseDoor> _level2StairDoor;
    /** A reference to the level 1 floor*/
    std::shared_ptr<Wall> _level1Wall;
    /** A reference to the level 1 floor*/
    std::shared_ptr<Wall> _level2Wall;
    /** A reference to the list of all staircase doors in the level*/
    std::vector<shared_ptr<StaircaseDoor>> _staircaseDoors;
    /** A reference to the level 1 cat den on the left hand side*/
    std::shared_ptr<CatDen> _level1CatDenLeft;
    /** A reference to the level 1 cat den on the right hand side*/
    std::shared_ptr<CatDen> _level1CatDenRight;
    /** A reference to the list of all cat dens in the level*/
    std::vector<shared_ptr<CatDen>> _catDens;
    /** A reference to the list of all walls in the level*/
    std::vector<shared_ptr<Wall>> _walls;
    /** A reference to the list of all doors in the level*/
    std::vector<shared_ptr<Floor>> _floors;

    /** A reference to the level 1 door*/
    std::shared_ptr<Door> _level1Door;
    /** A reference to the level 1 door frame*/
    std::shared_ptr<DoorFrame> _level1DoorFrame;
    /** A reference to the level 2 door*/
    std::shared_ptr<Door> _level2Door;
    /** A reference to the left wall*/
    std::shared_ptr<Floor> _leftWall;
    /** A reference to the right wall*/
    std::shared_ptr<Floor> _rightWall;
    /** A reference to the cagedAnimal*/
    std::shared_ptr<CagedAnimal> _cagedAnimal;
    /** A reference to the list of all doors in the level*/
    std::vector<shared_ptr<Door>> _doors;
    std::vector<shared_ptr<DoorFrame>> _doorFrames;
    std::shared_ptr<InputManager> _inputManager;
    /** The panel to win level*/
    std::shared_ptr<ui::PanelElement> _winPanel;
    /** The panel to lose level*/
    std::shared_ptr<ui::PanelElement> _losePanel;
    /** The panel to menu*/
    std::shared_ptr<ui::PanelElement> _menuPanel;
    /** The panel to keep track of possess numbers */
    std::shared_ptr<ui::PanelElement> _possessPanel;
    /** The button to open menu */
    std::shared_ptr<ui::ButtonElement> _menuButton;
    // Create panels
    std::shared_ptr<Texture> winPanel;
    std::shared_ptr<Texture> losePanel;

    std::shared_ptr<Texture> enemyHighlightTexture;
    std::shared_ptr<Texture> enemyTexture;
    std::shared_ptr<Texture> tableTexture;
    std::shared_ptr<scene2::Label> _tutorialText;
    std::shared_ptr<scene2::Label> _tutorialText2;

    bool _showTutorialText;

    enum class GameStatus {
        RUNNING,
        PAUSED,
        WIN,
        LOSE
    };
    GameStatus _gameStatus;

    /** whether or not the player has control*/
    bool _hasControl;

    /**
     * Internal helper to build the scene graph.
     *
     * Scene graphs are not required.  You could manage all scenes just like
     * you do in 3152.  However, they greatly simplify scene management, and
     * have become standard in most game engines.
     */
    void buildScene();
    void buildScene(std::shared_ptr<JsonValue>);

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
    GameplayMode() : cugl::Scene2(), _hasControl(false), _reset(false), _backToMenu(false), _nextLevel(false), _showTutorialText(false) {}

    /**
     * Disposes of this application, releasing all resources.
     *
     * This destructor is called by main.cpp when the application quits.
     * It simply calls the dispose() method in Application.  There is nothing
     * special to do here.
     */
    ~GameplayMode() { }

        /**
     * Initializes the controller contents, and starts the game
     *
     * The constructor does not allocate any objects or memory.  This allows
     * us to have a non-pointer reference to this controller, reducing our
     * memory allocation.  Instead, allocation happens in this method.
     *
     * @param assets    The (loaded) assets for this game mode
     * @param level    The number the selected level is within the current location
     *
     * @return true if the controller is initialized properly, false otherwise.
     */
    bool init(const std::shared_ptr<cugl::AssetManager>& assets, int level, std::shared_ptr<InputManager> inputManager);
    bool init(const std::shared_ptr<cugl::AssetManager>& assets, int level, std::shared_ptr<JsonValue> json, std::shared_ptr<InputManager> inputManager);


    /** used to reset the level*/
    void reset();

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
    void draw();

    /*Function called every update to check if the player is trying to close or
    open a door*/
    void checkDoors();

    /*Function called every update to check if the player is trying to exit
    or enter a staircase door*/
    void checkStaircaseDoors();

    /*Function called every update to check if the player is trying to exit
    or enter a cat den*/
    void checkCatDens();

    /** returns a vector of Vec2s representing the <x_pos, level> of closed doors in the level*/
    vector<Vec2> closedDoors();

    bool getBackToMenu() {
        return _backToMenu;
    }

    void setBackToMenu(bool backToMenu) {
        _backToMenu = backToMenu;
    }

    bool getNextLevel() {
        return _nextLevel;
    }

    void setNextlevel(bool next) {
        _nextLevel = next;
    }

    void clearJson() {
        _json = nullptr;
    }

    std::string getNextLevelID();

    void clearRootSceneNode();

    void ChangeDrawOrder();

    void toSaveJson();
    void setGameStatus(GameStatus status) {
        _gameStatus = status;
    }

    GameStatus getGameStatus() {
        return _gameStatus;
    }

    void setShowTutorial(bool boolean) {
        _showTutorialText = boolean;
        _tutorialText->setVisible(boolean);
        _tutorialText2->setVisible(boolean);
    }
};

#endif /* __GAMEPLAY_MODE_H__ */
