#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__
#include <cugl/cugl.h>
#include "Player.h"
#include "UIElement.h"
/** The portion of the screen used for the left zone */
#define LEFT_ZONE       0.35f
/** The portion of the screen used for the right zone */
#define RIGHT_ZONE      0.35f
class InputManager {
private:

    /** If go right, 1; if go left, -1; if stationary, 0 */
    int  _forward;
    int _keyForward;
    bool _camMovement;
    Vec2 _camMoveDirection;
    /** Position of single tap on screen, updated upon leaving, if not exist then Vec2::ZERO */
    Vec2 _tap_pos;
    Vec2 _mouse_rightJoystick_startPos;
    /** If pressed possess button then true, otherwise false */
    std::shared_ptr<Player> _player;
    std::shared_ptr<ui::ButtonElement> _possessButton;
    std::shared_ptr<ui::ButtonElement> _unPossessButton;
    std::shared_ptr<cugl::scene2::SceneNode> _rootSceneNode;
    //std::vector<std::shared_ptr<Enemy>> _enemies;
    //std::vector<std::shared_ptr<Interactables>> _interactable_objects;
    //std::vector<std::shared_ptr<Blocker>> _blocking_objects;
    //std::shared_ptr<PrimaryObjective> _target;

    /** The bounds of the entire game screen (in touch coordinates) */
    cugl::Rect _tbounds;
    /** The bounds of the entire game screen (in scene coordinates) */
    cugl::Rect _sbounds;
    /** The bounds of the left touch zone */
    cugl::Rect _lzone;
    /** The bounds of the right touch zone */
    cugl::Rect _rzone;

public:
    // Zone Definition
    /** Enumeration identifying a zone for the current touch */
    enum class Zone {
        /** The touch was not inside the screen bounds */
        UNDEFINED,
        /** The touch was in the left zone */
        LEFT,
        /** The touch was in the right zone */
        RIGHT,
        /** The touch was in the middle zone */
        MID
    };
    /** Information representing a single "touch" (possibly multi-finger) */
    struct TouchInstance {
        /** The anchor touch position (on start) */
        cugl::Vec2 position;
        /** The current touch time */
        cugl::Timestamp timestamp;
        /** The touch id(s) for future reference */
        std::unordered_set<Uint64> touchids;
    };

    /**
     * Defines the zone boundaries, so we can quickly categorize touches.
     */
    void createZones();

    /**
     * Returns the correct zone for the given position.
     *
     * See the comments above for a description of how zones work.
     *
     * @param  pos  a position in screen coordinates
     *
     * @return the correct zone for the given position.
     */
    Zone getZone(const cugl::Vec2 pos) const;
    /**
     * Creates a new input manager with the default settings
     *
     * To use this controller, you will need to initialize it first
     */

    InputManager();

    /**
     * Disposses this input manager, releasing all resources.
     */
    ~InputManager();

    /**
     * Initializes a new input controller for the specified player.
     *
     * The game supports two players working against each other in hot seat mode.
     * We need a separate input controller for each player. In keyboard, this is
     * WASD vs. Arrow keys.  Doing this on mobile requires you to get a little
     * creative.
     *
     * @param id Player id number (0..1)
     * @param bounds    the scene graph bounds
     *
     * @return true if the player was initialized correctly
     */
    bool init(std::shared_ptr<Player> player, std::shared_ptr<cugl::scene2::SceneNode> rootNode, cugl::Rect bounds);

    /**
     * Reads the input for this player and converts the result into game logic.
     *
     * This is an example of polling input.  Instead of registering a listener,
     * we ask the controller about its current state.  When the game is running,
     * it is typically best to poll input instead of using listeners.  Listeners
     * are more appropriate for menus and buttons (like the loading screen).
     */
    void readInput();

    /**
     * Returns the scene location of a touch
     *
     * Touch coordinates are inverted, with y origin in the top-left
     * corner. This method corrects for this and scales the screen
     * coordinates down on to the scene graph size.
     *
     * @return the scene location of a touch
     */
    cugl::Vec2 touch2Screen(const cugl::Vec2 pos) const;

    /**
     * Processes movement for the floating joystick.
     *
     * This will register movement as left or right (or neither).  It
     * will also move the joystick anchor if the touch position moves
     * too far.
     *
     * @param  pos  the current joystick position
     */
    void processLeftJoystick(const cugl::Vec2 pos);

    /**
     * Processes movement for the floating joystick.
     *
     * This will register movement as left or right (or neither).  It
     * will also move the joystick anchor if the touch position moves
     * too far.
     *
     * @param  pos  the current joystick position
     */
    void processRightJoystick(const cugl::Vec2 pos);

#pragma mark Touch and Mouse Callbacks
    /**
     * Callback for the beginning of a touch event
     *
     * @param event The associated event
     * @param focus	Whether the listener currently has focus
     *
     */
    void touchBeganCB(const cugl::TouchEvent& event, bool focus);

    /**
     * Callback for the end of a touch event
     *
     * @param event The associated event
     * @param focus	Whether the listener currently has focus
     */
    void touchEndedCB(const cugl::TouchEvent& event, bool focus);

    /**
     * Callback for a mouse release event.
     *
     * @param event The associated event
     * @param previous The previous position of the touch
     * @param focus	Whether the listener currently has focus
     */
    void touchesMovedCB(const cugl::TouchEvent& event, const cugl::Vec2& previous, bool focus);

    /**
     * Populates the initial values of the TouchInstances
     */
    void clearTouchInstance(TouchInstance& touchInstance);

    /**
     * Getter for the forward variable
     */
    float getForward() {
        return _forward;
    }

    /**
     * Getter for the tap position variable
     */
    Vec2 getTapPos() {
        return _tap_pos;
    }

    /**
     * Getter for the scene camera
     */
    std::shared_ptr<cugl::scene2::SceneNode> getRootSceneNode() {
        return _rootSceneNode;
    }

    /**
     * Getter for the scene camera
     */
    void setRootSceneNode(std::shared_ptr<cugl::scene2::SceneNode> rootNode) {
        _rootSceneNode = rootNode;
    }

private:
    /** The current touch location for the the entire screen */
    TouchInstance _stouch;
    /** The current touch location for the left zone */
    TouchInstance _ltouch;
    /** The current touch location for the right zone */
    TouchInstance _rtouch;
    /** The current touch location for the bottom zone */
    TouchInstance _mtouch;
    /** Whether the virtual joystick is active */
    bool _leftJoystick;
    /** The position of the virtual joystick */
    cugl::Vec2 _leftJoycenter;
    /** Whether the virtual joystick is active */
    bool _rightJoystick;
    /** The position of the virtual joystick */
    cugl::Vec2 _rightJoycenter;
};

#endif /* __INPUT_MANAGER_H__ */
