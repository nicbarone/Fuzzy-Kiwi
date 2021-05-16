#ifndef __LEVEL_SELECT_MODE_H__
#define __LEVEL_SELECT_MODE_H__
#include <cugl/cugl.h>
#include "GameplayMode.h"
#include "InputManager.h"

class LevelSelectMode : public cugl::Scene2 {
protected:
    std::shared_ptr<cugl::AssetManager> _assets;
    bool _levelSelected = false;
    bool _backButtonPressed = false;
    bool _gameMuted = false;
    bool _toUpdateVolume = false;
    std::string _levelID = "";
    GameplayMode _gameplay;
    std::shared_ptr<ui::PanelElement> _levelSelectPanel;
    std::shared_ptr<ui::ButtonElement> _backButton;
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
    LevelSelectMode() : cugl::Scene2() {}

    /**
     * Disposes of this application, releasing all resources.
     *
     * This destructor is called by main.cpp when the application quits.
     * It simply calls the dispose() method in Application.  There is nothing
     * special to do here.
     */
    ~LevelSelectMode() { }

    /**
 * Initializes the controller contents, and starts the game
 *
 * The constructor does not allocate any objects or memory.  This allows
 * us to have a non-pointer reference to this controller, reducing our
 * memory allocation.  Instead, allocation happens in this method.
 *
 * @param assets    The (loaded) assets for this game mode
 *
 * @return true if the controller is initialized properly, false otherwise.
 */
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);




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

    void updateAudio() {
        std::shared_ptr<AudioQueue> audioQueue = AudioEngine::get()->getMusicQueue();
        if (audioQueue->getVolume() < 0.5f) {
            // Game is muted
            _gameMuted = true;
        }
        else {
            // Game is unmuted
            _gameMuted = false;
        }
        Size  size = Application::get()->getDisplaySize();
        float scale = GAME_WIDTH / size.width;
        size *= scale;
        if (_levelSelectPanel->getChildButtons()["muteButton"] != nullptr) {
            _levelSelectPanel->getChildButtons()["muteButton"]->getButton()->deactivate();
            _levelSelectPanel->getSceneNode()->removeChildByName("muteButton");
        }
        if (_levelSelectPanel->getChildButtons()["unmuteButton"] != nullptr) {
            _levelSelectPanel->getChildButtons()["unmuteButton"]->getButton()->deactivate();
            _levelSelectPanel->getSceneNode()->removeChildByName("unmuteButton");
        }
        if (_gameMuted) {
            audioQueue->setVolume(0);
            _levelSelectPanel->createChildButton((size.width - 100.0f) / 2.0f / _levelSelectPanel->getSceneNode()->getScaleX(), (size.height - 100.0f) / 2.0f / _levelSelectPanel->getSceneNode()->getScaleY(), 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("mute"), Color4f::WHITE, "unmuteButton");
            _levelSelectPanel->getChildButtons()["unmuteButton"]->getButton()->setScale(Vec2(0.8f, 0.8f));
            _levelSelectPanel->getChildButtons()["unmuteButton"]->getButton()->setName("unmuteButton");
            _levelSelectPanel->getChildButtons()["unmuteButton"]->getButton()->addListener([=](const std::string& name, bool down) {
                // Only quit when the button is released
                if (!down) {
                    _gameMuted = false;
                    _toUpdateVolume = true;
                }
                });
            _levelSelectPanel->getChildButtons()["unmuteButton"]->getButton()->setVisible(true);
            _levelSelectPanel->getChildButtons()["unmuteButton"]->getButton()->activate();
        }
        else {
            audioQueue->setVolume(1.0f);
            _levelSelectPanel->createChildButton((size.width - 100.0f) / 2.0f / _levelSelectPanel->getSceneNode()->getScaleX(), (size.height - 100.0f) / 2.0f / _levelSelectPanel->getSceneNode()->getScaleY(), 200, 50, ui::ButtonState::AVAILABLE, _assets->get<Texture>("unmute"), Color4f::WHITE, "muteButton");
            _levelSelectPanel->getChildButtons()["muteButton"]->getButton()->setScale(Vec2(0.8f, 0.8f));
            _levelSelectPanel->getChildButtons()["muteButton"]->getButton()->setName("muteButton");
            _levelSelectPanel->getChildButtons()["muteButton"]->getButton()->addListener([=](const std::string& name, bool down) {
                // Only quit when the button is released
                if (!down) {
                    _gameMuted = true;
                    _toUpdateVolume = true;
                }
                });
            _levelSelectPanel->getChildButtons()["muteButton"]->getButton()->setVisible(true);
            _levelSelectPanel->getChildButtons()["muteButton"]->getButton()->activate();
        }
    }

    void updateLevelIcon();

    bool getMuted() {
        return _gameMuted;
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
    void draw();

    std::string getLevelSelectID() {
        return _levelID;
    }

    void clearLevelSelectID() {
        _levelID = "";
    }

    bool getLevelSelected() {
        return _levelSelected;
    }

    void setLevelSelected(bool selected) {
        _levelSelected = selected;
    }
    
    bool getBackPressed() {
        return _backButtonPressed;
    }

    void setBackPressed(bool pressed) {
        _backButtonPressed = pressed;
    }

    /**
     * Internal helper to build the scene graph.
     *
     * Scene graphs are not required.  You could manage all scenes just like
     * you do in 3152.  However, they greatly simplify scene management, and
     * have become standard in most game engines.
     */
    void buildScene();

    void deactivateButtons();
    void activateButtons();
};

#endif /* __LEVEL_SELECT_MODE_H__ */
