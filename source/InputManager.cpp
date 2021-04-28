#include "InputManager.h"

using namespace cugl;
#define LISTENER_KEY      3
#define ACCEL_KEY      2
/** How close we need to be for a multi touch */
#define NEAR_TOUCH      100
/** This defines the how far the left joystick has to move horizontally to be recognized*/
#define JSTICK_XDIFF_MIN  15
/** This defines the how far the right joystick has to move to be recognized*/
#define JSTICK_DIFF_MIN  15
/** This defines the joystick radial size (for reseting the anchor) */
#define JSTICK_RADIUS    25
/** How far to display the virtual joystick above the finger */
#define JSTICK_OFFSET    80
/** This how far before we no longer regard a touch on screen as a click */
#define TAP_THRESHOLD    10
/** This defines the minimum valid swipe distance */
#define SWIPE_THRESHOLD    100
/** This tells how sensitive the camera is to zooming */
#define ZOOM_SENSITIVITY    0.1f
/** This tells how fast the camera is to moving */
#define CAMERA_MAX_SPEED    23.0f
#define CAM_CENTER_OFFSET_Y -100.0f

/**
 * Creates a new input controller with the default settings
 *
 * To use this controller, you will need to initialize it first
 */
InputManager::InputManager() {
    _forward = 0;
    _keyForward = 0;
    _player = nullptr;
    _rootSceneNode = nullptr;
    _camMovement = false;
    _valid_tap = false;
}

InputManager::~InputManager()
{
    _forward = 0;
    _keyForward = 0;
    _player = nullptr;
    _rootSceneNode = nullptr;
    _camMovement = false;
    _valid_tap = false;
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
bool InputManager::init(std::shared_ptr<Player> player, std::shared_ptr<cugl::scene2::OrderedNode> rootNode, cugl::Rect bounds) {
    _possessPressed = false;
    _unpossessPressed = false;
    _forward = 0;
    _keyForward = 0;
    _player = player;
    _sbounds = bounds;
    _tbounds = Application::get()->getDisplayBounds();
    _rootSceneNode = rootNode;
    _camMovement = false;
    _camMoveDirection = Vec2::ZERO;
    _camOriginalPos = _rootSceneNode->getWorldPosition();
    _valid_tap = false;
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
#else
    _mouse_rightJoystick_startPos = Vec2::ZERO;
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
    result.x = px * _sbounds.size.width + _sbounds.origin.x - _sbounds.size.width / 2;
    result.y = (1 - py) * _sbounds.size.height + _sbounds.origin.y - _sbounds.size.height / 2 - CAM_CENTER_OFFSET_Y;
    result.x += _player->getPos();
    result.y += _player->getPosY();
    return result;
}

/**
     * Returns the world location of a screen (game) point
     *
     * Touch coordinates are inverted, with y origin in the top-left
     * corner. This method corrects for this and scales the screen
     * coordinates down on to the scene graph size.
     *
     * @return the world location of a screen point
     */
Vec2 InputManager::screen2World(const cugl::Vec2 pos) const {
    float sx = (pos.x - _sbounds.origin.x) / _sbounds.size.width;
    float sy = 1 - (pos.y - _sbounds.origin.y) / _sbounds.size.height;
    Vec2 result;
    result.x = (sx + _tbounds.origin.x) * _tbounds.size.width * _rootSceneNode->getScaleX();
    result.y = (sy + _tbounds.origin.y) * _tbounds.size.height;
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
void InputManager::processLeftJoystick(const cugl::Vec2 pos) {
    Vec2 diff = _ltouch.position - pos;

    // Reset the anchor if we drifted too far
    if (diff.lengthSquared() > JSTICK_RADIUS * JSTICK_RADIUS) {
        diff.normalize();
        diff *= (JSTICK_RADIUS + JSTICK_XDIFF_MIN) / 2;
        _ltouch.position = pos + diff;
    }
    _leftJoycenter = touch2Screen(_ltouch.position);

    if (std::abs(diff.x) > JSTICK_XDIFF_MIN) {
        _leftJoystick = true;
        if (diff.x > 0) {
            _keyForward = -1;
        }
        else {
            _keyForward = 1;
        }
    }
    else {
        _leftJoystick = false;
        _keyForward = 0;
    }
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
void InputManager::processRightSwipe(const cugl::Vec2 pos) {
    _rtouch.position = pos;
    float diff = _rtouch.beginPos.y - _rtouch.position.y;
    // If distant enough, we recognize it as a valid swipe
    if (diff > SWIPE_THRESHOLD) {
        // Possess pressed
        _possessPressed = true;
        _rtouch.touchids.clear();
    }
    else if (diff < -SWIPE_THRESHOLD) {
        // Unpossess pressed
        _unpossessPressed = true;
        _rtouch.touchids.clear();
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
    if (_player == nullptr) {
        return;
    }
    Vec2 pos = event.position;
    Zone zone = getZone(pos);
    // general touch (for tap on screen)
    if (_stouch.touchids.empty()) {
        _stouch.position = event.position;
        _stouch.timestamp.mark();
        _stouch.touchids.insert(event.touch);
        _stouch.isSingleTap[event.touch] = true;
    }
    switch (zone) {
    case Zone::LEFT:
        // Only process if no touch in zone
        if (_ltouch.touchids.empty()) {
            // Left is the floating joystick
            _ltouch.position = event.position;
            _ltouch.timestamp.mark();
            _ltouch.touchids.insert(event.touch);

            _leftJoystick = true;
            _leftJoycenter = touch2Screen(event.position);
            _leftJoycenter.y += JSTICK_OFFSET;
        }
        break;
    case Zone::RIGHT:
        // Check if other touches existed before this one
        if (_prev2Pivots[0].empty()) {
            // If no pivots, then add this one as the first pivot
            _prev2Pivots[0][event.touch] = Vec2(event.position.x, event.position.y);
        }
        else if (_prev2Pivots[1].empty()) {
            // If not empty, check if this one is close enough in terms of time, if so, add in as second pivot
            //CULog("elapsed %i milli seconds", event.timestamp.ellapsedMillis(_rtouch.timestamp, event.timestamp));
            //if (event.timestamp.ellapsedMicros(_rtouch.timestamp, event.timestamp) < 1000) {
            _prev2Pivots[1][event.touch] = Vec2(event.position.x, event.position.y);
            //}
        } // Otherwise all pivots are taken, no other pivots should be inserted
        // Only process if no touch in zone
        if (_rtouch.touchids.empty()) {
            _rtouch.beginPos = event.position;
            _rtouch.position = event.position;
            _rtouch.timestamp.mark();
            _rtouch.touchids.insert(event.touch);
        }
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
    if (_player == nullptr) return;
    // If the touch ended is one of the pivots, remove it
    if (_prev2Pivots[0].count(event.touch) != 0) {
        _prev2Pivots[0].clear();
    }
    else if (_prev2Pivots[1].count(event.touch) != 0) {
        _prev2Pivots[1].clear();
    }
    // Reset all keys that might have been set
    Vec2 pos = event.position;
    if (_stouch.touchids.find(event.touch) != _stouch.touchids.end()) {
        if (_stouch.isSingleTap[event.touch]) {
            _tap_pos = _stouch.position;
            CULog("tap pos %f, %f",_stouch.position.x, _stouch.position.y);
            _valid_tap = true;
        }
        _stouch.isSingleTap.erase(event.touch);
        _stouch.touchids.clear();
    }
    Zone zone = getZone(pos);
    if (_ltouch.touchids.find(event.touch) != _ltouch.touchids.end()) {
        _ltouch.touchids.clear();
        _keyForward = 0;
        _leftJoystick = false;
    }
    else if (_rtouch.touchids.find(event.touch) != _rtouch.touchids.end()) {
        _rtouch.touchids.clear();
        _rightJoystick = false;
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
    if (_player == nullptr) return;
    Vec2 pos = event.position;
    if (_stouch.touchids.find(event.touch) != _stouch.touchids.end()) {
        if ((pos - _rtouch.position).length() > TAP_THRESHOLD) {
            _stouch.isSingleTap[event.touch] = false;
        }
    }
    // Only check for swipes in the main zone if there is more than one finger.
    if (_ltouch.touchids.find(event.touch) != _ltouch.touchids.end()) {
        processLeftJoystick(pos);
    }
    // If both pivots are present, measure the change in their distance and zoom and move of camera
    if (!_prev2Pivots[0].empty() && !_prev2Pivots[1].empty()) {
        // Previous distance
        float prevDist = (_prev2Pivots[0].begin()->second).distance(_prev2Pivots[1].begin()->second);
        // Previous center of two pivots
        Vec2 prevCenter = (_prev2Pivots[0].begin()->second + _prev2Pivots[1].begin()->second) / 2;
        // If one of the pivots is the current event
        if (_prev2Pivots[0].count(event.touch) != 0) {
            _prev2Pivots[0][event.touch] = Vec2(event.position.x, event.position.y);
        }
        else if (_prev2Pivots[1].count(event.touch) != 0) {
            _prev2Pivots[1][event.touch] = Vec2(event.position.x, event.position.y);
        }
        float curDist = (_prev2Pivots[0].begin()->second).distance(_prev2Pivots[1].begin()->second);
        Vec2 curCenter = (_prev2Pivots[0].begin()->second + _prev2Pivots[1].begin()->second) / 2;
        if (abs(prevDist - curDist) > 1.0f) {
            // If passed the threshold of 0.1, then this counts as a dimension change
            float zoomFactor = (curDist - prevDist) / 100.0f;
            //CULog("Zoom factor %f, CurDist %f, prevDist %f", zoomFactor, curDist, prevDist);
            float newScale = min(max(_rootSceneNode->getScale().x + zoomFactor * ZOOM_SENSITIVITY, 0.3f), 1.0f);
            Vec2 oldDist = _rootSceneNode->getWorldPosition() - _camOriginalPos;
            float oldScale = _rootSceneNode->getScaleX();
            _rootSceneNode->setScale(Vec2(newScale, newScale));
            // Handle center of zooming
            _rootSceneNode->setPosition(_rootSceneNode->getWorldPosition() + oldDist * (newScale / oldScale - 1));
        }
        Vec2 diff = curCenter - prevCenter;
        if (diff.length() > 5.0f) {
            _camMoveDirection += Vec2(diff.x, -diff.y);
        }
        //_camMoveDirection = Vec2::ZERO;
    }
    else if (_rtouch.touchids.find(event.touch) != _rtouch.touchids.end()) {
        processRightSwipe(pos);
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
    _forward = _keyForward;
    if (_valid_tap) {
        _valid_tap = false;
    }
    else {
        _tap_pos = Vec2::ZERO;
    }
    _rootSceneNode->setPosition(_rootSceneNode->getPosition() + _camMoveDirection * 0.6f);
    _camMoveDirection = Vec2::ZERO;
#else
    // Figure out, based on which player we are, which keys
    // control our actions (depends on player).
    KeyCode left, right, camLeft, camRight, camUp, camDown, reset, possess, unpossess;
    left = KeyCode::A;
    right = KeyCode::D;
    camLeft = KeyCode::ARROW_LEFT;
    camRight = KeyCode::ARROW_RIGHT;
    camUp = KeyCode::ARROW_UP;
    camDown = KeyCode::ARROW_DOWN;
    reset = KeyCode::R;
    possess = KeyCode::F;
    unpossess = KeyCode::E;

    // Convert keyboard state into game commands
    _forward = 0;
    _reset = false;

    // Movement left/right
    Keyboard* keys = Input::get<Keyboard>();
    if (keys->keyDown(reset)) {
        _reset = true;
    }
    if (keys->keyDown(right) && !keys->keyDown(left)) {
        //CULog("Going right");
        _forward = 1;
    }
    else if (keys->keyDown(left) && !keys->keyDown(right)) {
        //CULog("Goring left");
        _forward = -1;
    }
    if (keys->keyDown(possess)) {
        _possessPressed = true;
    }
    else if (keys->keyDown(unpossess)) {
        _unpossessPressed = true;
    }
    if (Input::get<Mouse>()->buttonPressed().hasLeft()) {
        _tap_pos = Input::get<Mouse>()->pointerPosition();
        CULog("orignal cam pos is %f, %f", _camOriginalPos.x, _camOriginalPos.y);
        //CULog("current cam pos is %f, %f", touch2Screen(_rootSceneNode->getPosition()).x, touch2Screen(_rootSceneNode->getPosition()).y);
        CULog("sbound is %f, %f", _sbounds.size.width, _sbounds.size.height);
        //CULog("cat pos %f, %f", _player->getPos().x, _player->getPos().y);
        CULog("Clicked at %f, %f", _tap_pos.x, _tap_pos.y);
        //CULog("player world pos %f, %f", _rootSceneNode->nodeToWorldCoords(_player->getPos()).x, _rootSceneNode->nodeToWorldCoords(_player->getPos()).y);
    }
    else {
        _tap_pos = Vec2::ZERO;
    }
    // Handle Zoom in/out
    int zoomFactor = Input::get<Mouse>()->wheelDirection().y;
    float newScale = min(max(_rootSceneNode->getScale().x + zoomFactor * ZOOM_SENSITIVITY, 0.3f), 1.0f);
    Vec2 oldDist = _rootSceneNode->getWorldPosition() - _camOriginalPos;
    float oldScale = _rootSceneNode->getScaleX();
    _rootSceneNode->setScale(Vec2(newScale, newScale));
    // Handle center of zooming
    _rootSceneNode->setPosition(_rootSceneNode->getWorldPosition() + oldDist * (newScale / oldScale - 1));
    // Mimic phone tap on right screen with mouse right click
    if (Input::get<Mouse>()->buttonDown().hasRight()) {
        if (!_rightJoystick) {
            _rightJoystick = true;
            _mouse_rightJoystick_startPos = Input::get<Mouse>()->pointerPosition();
        }
        else {
            _camMoveDirection = (Input::get<Mouse>()->pointerPosition() - _mouse_rightJoystick_startPos);
            _camMoveDirection.x = -_camMoveDirection.x;
        }
    }
    else {
        _rightJoystick = false;
    }
    // Handle Camera Drag
    if (_rightJoystick) {
        _camMoveDirection.normalize();
        _camMoveDirection *= CAMERA_MAX_SPEED;
        _rootSceneNode->setPosition(_rootSceneNode->getPosition() + _camMoveDirection / 5);
    }
    else {
        // Otherwise center on the cat
        _camMoveDirection = Vec2::ZERO;
        //CULog("player world pos %f, %f",_rootSceneNode->nodeToWorldCoords(_player->getPos()).x, _rootSceneNode->nodeToWorldCoords(_player->getPos()).y);
        //CULog("Xval %f", -screen2World(_player->getPos()).x + _tbounds.size.width);
        //_rootSceneNode->setPosition(_rootSceneNode->nodeToParentCoords(_player->getSceneNode()->getPosition()));
    }
#endif
}