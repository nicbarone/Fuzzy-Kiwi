#include "App.h"
using namespace cugl;

#pragma mark -
#pragma mark Gameplay Control

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
void App::onStartup() {
    _assets = AssetManager::alloc();
    _batch = SpriteBatch::alloc();
    setClearColor(Color4(229, 229, 229, 255));
    auto cam = OrthographicCamera::alloc(getDisplaySize());

    // Start-up basic input
#ifdef CU_MOBILE
    Input::activate<Touchscreen>();
    Input::activate<Accelerometer>();
#else
    Input::activate<Mouse>();
    Input::get<Mouse>()->setPointerAwareness(Mouse::PointerAwareness::DRAG);
    Input::activate<Keyboard>();
#endif

    _assets->attach<Texture>(TextureLoader::alloc()->getHook());
    _assets->attach<Sound>(SoundLoader::alloc()->getHook());
    _assets->attach<Font>(FontLoader::alloc()->getHook());

    // Create a "loading" screen
    _loaded = false;
    _loading.init(_assets);

    // Queue up the other assets
    _assets->loadDirectory("json/assets.json");

    //Input manager
    _inputManager = std::shared_ptr<InputManager>(new InputManager());
    _inputManager->init(nullptr, scene2::OrderedNode::allocWithOrder(scene2::OrderedNode::Order::ASCEND), getSafeBounds());
    AudioEngine::start();
    Application::onStartup(); // YOU MUST END with call to parent
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
void App::onShutdown() {
    _gameplay.toSaveJson();
    _loading.dispose();
    _gameplay.dispose();
    _assets = nullptr;
    _batch = nullptr;

    // Shutdown input
#ifdef CU_MOBILE
    Input::deactivate<Touchscreen>();
    Input::deactivate<Accelerometer>();
#else
    Input::deactivate<Mouse>();
    Input::deactivate<Keyboard>();
#endif
    AudioEngine::get()->pause();
    AudioEngine::get()->getMusicQueue()->clear();
    AudioEngine::stop();

    Application::onShutdown();  // YOU MUST END with call to parent
}

/**
 * The method called when the application is suspended and put in the background.
 *
 * When this method is called, you should store any state that you do not
 * want to be lost.  There is no guarantee that an application will return
 * from the background; it may be terminated instead.
 *
 * If you are using audio, it is critical that you pause it on suspension.
 * Otherwise, the audio thread may persist while the application is in
 * the background.
 */
void App::onSuspend() {
    AudioEngine::get()->pause();
}

/**
 * The method called when the application resumes and put in the foreground.
 *
 * If you saved any state before going into the background, now is the time
 * to restore it. This guarantees that the application looks the same as
 * when it was suspended.
 *
 * If you are using audio, you should use this method to resume any audio
 * paused before app suspension.
 */
void App::onResume() {
    AudioEngine::get()->resume();
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
void App::update(float timestep) {
    if (!_inGameplay) _inputManager->readInput();
    if (!_loaded && counter > 0) {
        _menu.init(_assets);
        _levelSelect.init(_assets);
        _loading.update(0.01f);
        counter=0;
        if (USE_LEVEL_EDITOR) {
            CULog("si");
            _inEditor = true;
            _levelEditor.init(_assets, _inputManager);
            _loaded = true;
        }
    }
    else if (_inEditor) {
        _levelEditor.update(timestep);
    }
    else if (!_loaded && !_inEditor){
        _loading.dispose(); // Disables the input listeners in this mode
        
        _loaded = true;
        _inMenu = true;
        
    }
    else if (_inLevelSelect) {
        _levelSelect.update(timestep);
        if (_levelSelect.getBackPressed()) {
            _levelSelect.setBackPressed(false);
            _levelSelect.deactivateButtons();
            _menu.activateButtons();
            _menu.setGameLoaded(false);
            _menu.setPlayPressed(false);
            _inLevelSelect = false;
            _inMenu = true;
        }
        else if (_levelSelect.getLevelSelected()) {
            // Load the level
            std::string level = _levelSelect.getLevelSelectID();
            _gameplay = _menu.getGameScene(level, _inputManager);
            _gameplay.reset();
            _inLevelSelect = false;
            _inGameplay = true;
            _levelSelect.clearLevelSelectID();
            _levelSelect.setLevelSelected(false);
            _levelSelect.deactivateButtons();
        }
    }
    else if (_inMenu){
        _menu.update(timestep);
        if (_menu.getPlayPressed()) {
            _menu.setPlayPressed(false);
            _menu.deactivateButtons();
            _levelSelect.init(_assets);
            CULog("si");
            _inMenu = false;
            _inLevelSelect = true;
            _levelSelect.updateLevelIcon();
        }
        else if (_menu.getGameLoaded()) {
            _menu.setGameLoaded(false);
            _menu.deactivateButtons();
            _levelSelect.setLevelSelected(false);
            CULog(Application::getSaveDirectory().c_str());
            shared_ptr<JsonReader> reader = JsonReader::alloc(Application::getSaveDirectory() + "save.json");
            _gameplay.init(_assets, 1, reader->readJson(), _inputManager);
            _gameplay.reset();
            _inMenu = false;
            _inGameplay = true;
        }
    }
    else {
        _gameplay.update(timestep);
        if (_gameplay.getBackToMenu()) {
            _gameplay.toSaveJson();
            _gameplay.setBackToMenu(false);
            _levelSelect.activateButtons();
            _levelSelect.setLevelSelected(false);
            _levelSelect.setBackPressed(false);
            _inGameplay = false;
            _inLevelSelect = true;
            _levelSelect.updateLevelIcon();
        }
        else if (_gameplay.getNextLevel()) {
            _gameplay.setNextlevel(false);
            std::string level = _gameplay.getNextLevelID();
            if (level == "Location Cleared") {
                _levelSelect.activateButtons();
                _levelSelect.setBackPressed(false);
                _levelSelect.setLevelSelected(false);
                _levelSelect.updateLevelIcon();
                _inLevelSelect = true;
                _inGameplay = false;
            }
            else {
                _gameplay = _menu.getGameScene(_gameplay.getNextLevelID(),_inputManager);
                _gameplay.reset();
            }
        }
    }
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
void App::draw() {
    if (!_loaded) {
        _loading.render(_batch);
    }
    else if (_inEditor) {
        _levelEditor.render(_batch);
    }
    else if (_inMenu) {
        _menu.render(_batch);
    }
    else if (_inLevelSelect) {
        _levelSelect.render(_batch);
    }
    else {
        _gameplay.render(_batch);
    }
}