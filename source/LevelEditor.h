#pragma once
//
//  GLLoadingScene.h
//  Programming Lab
//
//  This module provides a very barebones loading screen.  Most of the time you
//  will not need a loading screen, because the assets will load so fast.  But
//  just in case, this is a simple example you can use in your games.
//
//  We know from 3152 that you all like to customize this screen.  Therefore,
//  we have kept it as simple as possible so that it is easy to modify. In
//  fact, this loading screen uses the new modular JSON format for defining
//  scenes.  See the file "loading.json" for how to change this scene.
//
//  Author: Walker White
//  Version: 2/21/21
//
#ifndef __LEVEL_EDITOR_H__
#define __LEVEL_EDITOR_H__
#include <cugl/cugl.h>
#include "InputManager.h"
#include "Constants.h"


extern const float FLOOR_OFFSET;
extern const float FLOOR_HEIGHT;
/**
 * This class is a simple loading screen for asychronous asset loading.
 *
 * The screen will display a very minimal progress bar that displays the
 * status of the asset manager.  Make sure that all asychronous load requests
 * are issued BEFORE calling update for the first time, or else this screen
 * will think that asset loading is complete.
 *
 * Once asset loading is completed, it will display a play button.  Clicking
 * this button will inform the application root to switch to the gameplay mode.
 */
class LevelEditor : public cugl::Scene2 {
protected:
    /** The parent scene node for a level*/
    std::shared_ptr<cugl::scene2::OrderedNode> _rootScene;
    /** The loaders to (synchronously) load in assets */
    std::shared_ptr<cugl::AssetManager> _assets;
    std::shared_ptr<InputManager> _inputManager;

    /** textures*/
    std::shared_ptr<Texture> catTexture;
    std::shared_ptr<Texture> staircaseDoorTexture;
    std::shared_ptr<Texture> catDenTexture;
    std::shared_ptr<Texture> doorTexture;
    std::shared_ptr<Texture> cageTexture;
    std::shared_ptr<Texture> enemyTexture;

    vector< std::shared_ptr<cugl::scene2::Button>> buttons;
    /** function buttons*/
    std::shared_ptr<cugl::scene2::Button> _clear;
    std::shared_ptr<cugl::scene2::Button> _load;
    std::shared_ptr<cugl::scene2::Button> _save;
    std::shared_ptr<cugl::scene2::Button> _cat;
    std::shared_ptr<cugl::scene2::Button> _staircaseDoor;
    std::shared_ptr<cugl::scene2::Button> _catDen;
    std::shared_ptr<cugl::scene2::Button> _door;
    std::shared_ptr<cugl::scene2::Button> _objective;
    std::shared_ptr<cugl::scene2::Button> _enemy;

    /** text input fields*/
    std::shared_ptr<cugl::scene2::Label> _floorsLabel;
    std::shared_ptr<cugl::scene2::TextField> _floorsField;
    std::shared_ptr<cugl::scene2::Label> _filePathLabel;
    std::shared_ptr<cugl::scene2::TextField> _filePathField;
    std::shared_ptr<cugl::scene2::Label> _numPossessionLabel;
    std::shared_ptr<cugl::scene2::TextField> _numPossessionField;
    std::shared_ptr<cugl::scene2::Label> _doorIDLabel;
    std::shared_ptr<cugl::scene2::TextField> _doorIDField;
    std::shared_ptr<cugl::scene2::Label> _keyLabel;
    std::shared_ptr<cugl::scene2::TextField> _keyField;
    std::shared_ptr<cugl::scene2::Label> _objectiveLabel;
    std::shared_ptr<cugl::scene2::TextField> _objectiveField;

    vector<float> floorHeights;


    bool pendingPlacement = false;
    bool resetButtons = false;
    int enemyPlacement = 0;
    float pathBegin = 0;
    float pathEnd = 0;
    std::shared_ptr<cugl::scene2::PolygonNode> pendingNode;
    std::shared_ptr<cugl::scene2::PolygonNode> pendingEnemy;
    std::shared_ptr<cugl::scene2::WireNode> pendingPath;
    vector <std::shared_ptr<cugl::scene2::PolygonNode>> enemies;
    vector <std::shared_ptr<cugl::scene2::WireNode>> paths;

public:

    /**
     * Creates a new loading mode with the default values.
     *
     * This constructor does not allocate any objects or start the game.
     * This allows us to use the object without a heap pointer.
     */
    LevelEditor() : cugl::Scene2() {}

    /**
     * Disposes of all (non-static) resources allocated to this mode.
     *
     * This method is different from dispose() in that it ALSO shuts off any
     * static resources, like the input controller.
     */
    ~LevelEditor() { dispose(); }

    /**
     * Disposes of all (non-static) resources allocated to this mode.
     */
    void dispose();

    /**
        change all buttons to the up state and remove the pending node
    */
    void releaseButtons();

    /**
        node placement
    */
    void placeNode();

    /**
        convert all scene nodes in _rootScene to a level storage format json
    */
    shared_ptr<JsonValue> toJson();

    /**
        creates nodes and add them to _rootScene based on the given json
    */
    void fromJson(shared_ptr<JsonValue> json);

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
    bool init(const std::shared_ptr<cugl::AssetManager>& assets, std::shared_ptr<InputManager> inputManager);

    /**
        helper function for snapping placement to rows
    */
    float addOffset(float pos, string type);
    /**
        helper function for snapping placement to rows and adding the correct offset
    */
    Vec2 snapToRow(Vec2 pos, string type);

    /**
        helper function for placing stuff after camera pan/zoom
    */
    Vec2 adjustForCamera(Vec2 pos);

    /**
     * The method called to update the game mode.
     *
     * This method updates the progress bar amount.
     *
     * @param timestep  The amount of time (in seconds) since the last frame
     */
    void update(float timestep);

    void buildScene();

    void clearEnemyPlacement();
};

#endif /* __LEVEL_EDITOR_H__ */
