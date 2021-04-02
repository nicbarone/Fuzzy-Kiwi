#include "LevelEditor.h"

using namespace cugl;

/** This is the ideal size of the logo */
#define SCENE_SIZE  1024

#pragma mark -
#pragma mark Constructors

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
bool LevelEditor::init(const std::shared_ptr<AssetManager>& assets) {
    // Initialize the scene to a locked width
    Size dimen = Application::get()->getDisplaySize();
    // Lock the scene to a reasonable resolution
    if (dimen.width > dimen.height) {
        dimen *= SCENE_SIZE / dimen.width;
    }
    else {
        dimen *= SCENE_SIZE / dimen.height;
    }
    if (assets == nullptr) {
        return false;
    }
    else if (!Scene2::init(dimen)) {
        return false;
    }


    _assets = assets;
    _rootScene = scene2::SceneNode::alloc();
    _rootScene->setAnchor(Vec2::ANCHOR_CENTER);
    _rootScene->setContentSize(Application::get()->getSafeBounds().size);

    buildScene();
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void LevelEditor::dispose() {
    _assets = nullptr;
}


#pragma mark -
#pragma mark Progress Monitoring
/**
 * The method called to update the game mode.
 *
 * This method updates the progress bar amount.
 *
 * @param timestep  The amount of time (in seconds) since the last frame
 */
void LevelEditor::update(float progress) {

}

void LevelEditor::buildScene() {
    addChild(_rootScene);
    std::shared_ptr<Font> font = _assets->get<Font>("felt");
    shared_ptr<scene2::Label> _clearText = scene2::Label::alloc("Clear All", font);
    _clear = scene2::Button::alloc(_clearText);
    addChild(_clear);
    CULog("yeah");
}

