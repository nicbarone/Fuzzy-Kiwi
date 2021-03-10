#include "InputManager.h"

using namespace cugl;
#define LISTENER_KEY      1
#define ACCEL_KEY      2
/** How close we need to be for a multi touch */
#define NEAR_TOUCH      100
/** This defines the how far the joystick has to move horizontally to be recognized*/
#define JSTICK_XDIFF_MIN  15
/** This defines the joystick radial size (for reseting the anchor) */
#define JSTICK_RADIUS    25
/** How far to display the virtual joystick above the finger */
#define JSTICK_OFFSET    80
/** This how far before we no longer regard a touch on screen as a click */
#define TAP_THRESHOLD    10

/**
 * Creates a new input controller with the default settings
 *
 * To use this controller, you will need to initialize it first
 */
InputManager::InputManager() {
    _player = nullptr;
    _possessButton = nullptr;
    _unPossessButton = nullptr;
}

InputManager::~InputManager()
{
    _player = nullptr;
    _possessButton = nullptr;
    _unPossessButton = nullptr;
}

/**
 * Defines the zone boundaries, so we can quickly categorize touches.
 */
void InputManager::createZones() {
    _lzone = _tbounds;
    _lzone.size.width *= LEFT_ZONE;
    _rzone = _tbounds;
    _rzone.size.width *= RIGHT_ZONE;
    _rzone.origin.x = _tbounds.origin.x + _tbounds.size.width - _rzone.size.width;
}

/**
 * Returns the correct zone for the given position.
 *
 * See the comments above for a description of how zones work.
 *
 * @param  pos  a position in screen coordinates
 *
 * @return the correct zone for the given position.
 */
InputManager::Zone InputManager::getZone(const Vec2 pos) const {
    if (_lzone.contains(pos)) {
        return Zone::LEFT;
    }
    else if (_rzone.contains(pos)) {
        return Zone::RIGHT;
    }
    else if (_tbounds.contains(pos)) {
        return Zone::MID;
    }
    return Zone::UNDEFINED;
}

/**
 * Populates the initial values of the input TouchInstance
 */
void InputManager::clearTouchInstance(TouchInstance& touchInstance) {
    touchInstance.touchids.clear();
    touchInstance.position = Vec2::ZERO;
}

/**
 * Initializes a new input controller for the specified player.
 *
 * The game supports two players working against each other in hot seat mode.
 * We need a separate input controller for each player. In keyboard, this is
 * WASD vs. Arrow keys.  Doing this on mobile requires you to get a little
 * creative.
 *
 * @param id Player id number (0..1)
 *
 * @return true if the player was initialized correctly
 */
bool InputManager::init(std::shared_ptr<Player> player, cugl::Rect bounds) {
    _player = player;
    _sbounds = bounds;
    _tbounds = Application::get()->getDisplayBounds();
    createZones();
    clearTouchInstance(_stouch);
    clearTouchInstance(_ltouch);
    clearTouchInstance(_rtouch);
    clearTouchInstance(_mtouch);
#ifdef CU_MOBILE
    // Touch Screen
    Touchscreen* touch = Input::get<Touchscreen>();
    touch->addBeginListener(LISTENER_KEY, [=](const TouchEvent& event, bool focus) {
        this->touchBeganCB(event, focus);
        });
    touch->addEndListener(LISTENER_KEY, [=](const TouchEvent& event, bool focus) {
        this->touchEndedCB(event, focus);
        });
    touch->addMotionListener(LISTENER_KEY, [=](const TouchEvent& event, const Vec2& previous, bool focus) {
        this->touchesMovedCB(event, previous, focus);
        });
#endif
    return true;
}

/**
 * Returns the scene location of a touch
 *
 * Touch coordinates are inverted, with y origin in the top-left
 * corner. This method corrects for this and scales the screen
 * coordinates down on to the scene graph size.
 *
 * @return the scene location of a touch
 */
Vec2 InputManager::touch2Screen(const Vec2 pos) const {
    float px = pos.x / _tbounds.size.width - _tbounds.origin.x;
    float py = pos.y / _tbounds.size.height - _tbounds.origin.y;
    Vec2 result;
    result.x = px * _sbounds.size.width + _sbounds.origin.x;
    result.y = (1 - py) * _sbounds.size.height + _sbounds.origin.y;
    return result;
}

/**
 * Processes movement for the floating joystick.
 *
 * This will register movement as left or right (or neither).  It
 * will also move the joystick anchor if the touch position moves
 * too far.
 *
 * @param  pos  the current joystick position
 */
void InputManager::processJoystick(const cugl::Vec2 pos) {
    Vec2 diff = _ltouch.position - pos;

    // Reset the anchor if we drifted too far
    if (diff.lengthSquared() > JSTICK_RADIUS * JSTICK_RADIUS) {
        diff.normalize();
        diff *= (JSTICK_RADIUS + JSTICK_XDIFF_MIN) / 2;
        _ltouch.position = pos + diff;
    }
    _joycenter = touch2Screen(_ltouch.position);

    if (std::abs(diff.x) > JSTICK_XDIFF_MIN) {
        _joystick = true;
        if (diff.x > 0) {
            _keyForward = 1;
        }
        else {
            _keyForward = -1;
        }
    }
    else {
        _joystick = false;
        _keyForward = 0;
    }
}

#pragma mark Touch and Mouse Callbacks
/**
 * Callback for the beginning of a touch event
 *
 * @param event The associated event
 * @param focus	Whether the listener currently has focus
 */
void InputManager::touchBeganCB(const TouchEvent& event, bool focus) {
    Vec2 pos = event.position;
    Zone zone = getZone(pos);
    // general touch (for tap on screen)
    if (_stouch.touchids.empty()) {
        _stouch.position = event.position;
        _stouch.timestamp.mark();
        _stouch.touchids.insert(event.touch);
    }
    switch (zone) {
    case Zone::LEFT:
        // Only process if no touch in zone
        if (_ltouch.touchids.empty()) {
            // Left is the floating joystick
            _ltouch.position = event.position;
            _ltouch.timestamp.mark();
            _ltouch.touchids.insert(event.touch);

            _joystick = true;
            _joycenter = touch2Screen(event.position);
            _joycenter.y += JSTICK_OFFSET;
        }
        break;
    case Zone::RIGHT:
        // No actions for right right now
        _rtouch.touchids.insert(event.touch);
        break;
    case Zone::MID:
        // No actions for right right now
        _mtouch.touchids.insert(event.touch);
        break;
    default:
        CUAssertLog(false, "Touch is out of bounds");
        break;
    }
}

/**
 * Callback for the end of a touch event
 *
 * @param event The associated event
 * @param focus	Whether the listener currently has focus
 */
void InputManager::touchEndedCB(const TouchEvent& event, bool focus) {
    // Reset all keys that might have been set
    Vec2 pos = event.position;
    if (_stouch.touchids.find(event.touch) != _rtouch.touchids.end()) {
        _stouch.touchids.clear();
    }
    Zone zone = getZone(pos);
    if (_ltouch.touchids.find(event.touch) != _ltouch.touchids.end()) {
        _ltouch.touchids.clear();
        _keyForward = 0;
        _joystick = false;
    }
    else if (_rtouch.touchids.find(event.touch) != _rtouch.touchids.end()) {
        _rtouch.touchids.clear();
    }
    else if (zone == Zone::MID) {
        _mtouch.touchids.clear();
    }
}

/**
 * Callback for a touch moved event.
 *
 * @param event The associated event
 * @param previous The previous position of the touch
 * @param focus	Whether the listener currently has focus
 */
void InputManager::touchesMovedCB(const TouchEvent& event, const Vec2& previous, bool focus) {
    Vec2 pos = event.position;
    if (_stouch.touchids.find(event.touch) != _rtouch.touchids.end()) {
        if ((pos-_stouch.position).length()>TAP_THRESHOLD) 
            _stouch.touchids.clear();
    }
    // Only check for swipes in the main zone if there is more than one finger.
    if (_ltouch.touchids.find(event.touch) != _ltouch.touchids.end()) {
        processJoystick(pos);
    }
}

/**
 * Reads the input for this player and converts the result into game logic.
 *
 * This is an example of polling input.  Instead of registering a listener,
 * we ask the controller about its current state.  When the game is running,
 * it is typically best to poll input instead of using listeners.  Listeners
 * are more appropriate for menus and buttons (like the loading screen).
 */
void InputManager::readInput() {
    // TODO: mobile controls disabled right now
#ifdef CU_MOBILE
    /**
    // YOU NEED TO PUT SOME CODE HERE
    // if not your turn, don't take inputs
    if (!_yourTurn) {
        return;
    }
    _forward = _turning = 0;
    _didFire = false;
    if (_player == 0) {
        _didFire = _keyFire;
        _turning = _turnAngle;
        _forward = _keyForward ? 1 : 0;
    }
    else {
        Vec3 acc = Input::get<Accelerometer>()->getAcceleration();

        // Measure the "left-right" tilt of the device
        float lrpitch = atan2(-acc.x, sqrt(acc.y * acc.y + acc.z * acc.z));

        // Check if we turned left or right
        if (lrpitch > EVENT_ACCEL_THRESH) _turning = -1;
        else if (lrpitch < -EVENT_ACCEL_THRESH) _turning = 1;
        else _turning = 0;

        // Measure the "forward-backword" tilt of the device
        float fbpitch = atan2(-acc.y, sqrt(acc.x * acc.x + acc.z * acc.z));

        // Check if we turned left or right
        if (fbpitch > EVENT_ACCEL_THRESH) _forward = 1;
        else if (fbpitch < -EVENT_ACCEL_THRESH) _forward = -1;
        else _forward = 0;
        // if stouch is not null, then it is a tap
        if (!_stouch.empty()) {
            _tap_pos = _stouch.position;
        } else {
            _tap_pos = Vec2::ZERO;
        }
        
    } */

#else
    // Figure out, based on which player we are, which keys
    // control our actions (depends on player).
    KeyCode left, right;
    left = KeyCode::A;
    right = KeyCode::D;

    // Convert keyboard state into game commands
    _forward = 0;

    // Movement left/right
    Keyboard* keys = Input::get<Keyboard>();
    if (keys->keyDown(right) && !keys->keyDown(left)) {
        //CULog("Going right");
        _forward = 1;
    }
    else if (keys->keyDown(left) && !keys->keyDown(right)) {
        //CULog("Goring left");
        _forward = -1;
    }
    if (Input::get<Mouse>()->buttonPressed().hasLeft()) {
        _tap_pos = Input::get<Mouse>()->pointerPosition();
        //CULog("Clicked left");
    }
    else {
        _tap_pos = Vec2::ZERO;
    }
#endif
}