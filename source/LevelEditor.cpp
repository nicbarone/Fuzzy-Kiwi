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

#if defined (CU_TOUCH_SCREEN)
    Input::activate<Touchscreen>();
#else
    Input::activate<Mouse>();
    Input::get<Mouse>()->setPointerAwareness(Mouse::PointerAwareness::ALWAYS);
    Input::activate<Keyboard>();
    Input::activate<TextInput>();
#endif

    for (int i = 0; i < 2; i++) {
        floorHeights.push_back(FLOOR_OFFSET + FLOOR_HEIGHT * i);
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
    _rootScene = nullptr;
}



void LevelEditor::releaseButtons() {
    resetButtons = true;
    for (auto it = begin(buttons); it != end(buttons); ++it) {
        it->get()->setDown(false);
    }
    if (pendingNode != nullptr) {
        _rootScene->removeChild(pendingNode);
    }
    pendingPlacement = false;
    resetButtons = false;
}

float closestNum(vector<float> arr, float num) {
    float candidate;
    float diff = INT_MAX;
    for (auto it = begin(arr); it != end(arr); ++it) {
        if (abs(*it-num) < diff) {
            candidate = *it;
            diff = abs(*it - num);
        }
    }
    return candidate;
}

Vec2 LevelEditor::snapToRow(Vec2 pos) {
    Size dimen = Application::get()->getDisplaySize();
    pos.y = dimen.height - pos.y;
    pos.y = closestNum(floorHeights, pos.y);
    return pos;
}

void LevelEditor::placeNode() {
    if (pendingNode != nullptr) { //only true if the button clicked places nodes 
        if (!pendingPlacement && Input::get<Mouse>()->buttonReleased().hasLeft()) { //the frame that a button is clicked
            pendingPlacement = true;
        }
        pendingNode->setPosition(snapToRow(Input::get<Mouse>()->pointerPosition()));
        if (pendingPlacement && Input::get<Mouse>()->buttonPressed().hasLeft()) { 
            pendingNode = nullptr;
            pendingPlacement = false;
            releaseButtons();
        }
    }
}

shared_ptr<JsonValue> LevelEditor::toJson() {
    shared_ptr<JsonValue> result = JsonValue::allocNull();
    vector<std::shared_ptr<scene2::SceneNode>> children = _rootScene->getChildren();
    for (auto it = begin(children); it != end(children); ++it) {
        auto currentNode = it->get();
        if (currentNode->getName() == "cat") {
            result->append("player", "test");
        }
        else {
            int returnID = MessageBox(
                NULL,
                "Find Tony if you can replicate this problem",
                "Something went wrong",
                MB_ICONWARNING | MB_OK | MB_DEFBUTTON1
            );
        }
    }
    return result;
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

    placeNode();
    if (Input::get<Mouse>()->buttonPressed().hasRight()) {
        releaseButtons();
    }
}

void LevelEditor::buildScene() {
    addChild(_rootScene);
    std::shared_ptr<Font> font = _assets->get<Font>("small-felt");

    //Text Fields
    _doorID = scene2::TextField::alloc("Door ID", font);
    _doorID->setPosition(0,550);
    _doorID->activate();
    addChild(_doorID);
    _filePath = scene2::TextField::alloc("Enter File Name", font);
    _filePath->setPosition(600, 550);
    _filePath->activate();
    addChild(_filePath);

    //Clear Button
    shared_ptr<scene2::Label> _clearText = scene2::Label::alloc("Clear All", font);
    shared_ptr<scene2::Label> _confirmText = scene2::Label::alloc("Are you sure?", font);
    _clearText->setBackground(Color4::WHITE);
    _confirmText->setBackground(Color4::WHITE);
    _clear = scene2::Button::alloc(_clearText, _confirmText);
    _clear->setToggle(true);
    _clear->activate();
    _clear->addListener([=](const std::string& name, bool down) {
        if (!down && !resetButtons) {
            _rootScene->removeAllChildren();
        }
    });
    buttons.push_back(_clear);
    addChild(_clear);

    //Cat Button
    std::shared_ptr<Texture> cat = _assets->get<Texture>("cat-walking");
    shared_ptr<scene2::Label> _catText = scene2::Label::alloc("Cat", font);
    _catText->setBackground(Color4::WHITE);
    _cat = scene2::Button::alloc(_catText, Color4::GRAY);
    _cat->setToggle(true);
    _cat->activate();
    _cat->setPosition(150, 0);
    _cat->addListener([=](const std::string& name, bool down) {
            if (down) {
                pendingNode = scene2::AnimationNode::alloc(cat, 1, 8);
                pendingNode->setScale(0.15, 0.15);
                pendingNode->setName("cat");
                _rootScene->addChild(pendingNode);
            }
        });
    buttons.push_back(_cat);
    addChild(_cat);

    //Save Button
    shared_ptr<scene2::Label> _saveText = scene2::Label::alloc("Save", font);
    _saveText->setBackground(Color4::WHITE);
    _save = scene2::Button::alloc(_saveText, Color4::GRAY);
    _save->activate();
    _save->setPosition(800, 0);
    _save->addListener([=](const std::string& name, bool down) {
        if (down) {
            shared_ptr<JsonWriter> writer = JsonWriter::alloc("levels/test.json");
            writer->writeLine("test");
            //todo actually converting scenenodes into a json
            writer->close();
        }
        });
    buttons.push_back(_save);
    addChild(_save);

    //Load Button
    shared_ptr<scene2::Label> _loadText = scene2::Label::alloc("Load", font);
    _loadText->setBackground(Color4::WHITE);
    _load = scene2::Button::alloc(_loadText, Color4::GRAY);
    _load->activate();
    _load->setPosition(950, 0);
    _load->addListener([=](const std::string& name, bool down) {
        if (down) {
            if (filetool::file_exists("levels\\tesst.json")) {
                shared_ptr<JsonReader> reader = JsonReader::allocWithAsset("levels\\test.json");
                CULog(reader->readAll().c_str());
                //todo actually reading the json and creating the scenenodes
            }
            else {
                string messageStr = "File ";
                messageStr.append(Application::get()->getAssetDirectory());
                messageStr.append(_filePath->getText());
                messageStr.append(" cannot be found");
                int returnID = MessageBox(
                    NULL,
                    messageStr.c_str(),
                    "File not found",
                    MB_ICONWARNING | MB_OK | MB_DEFBUTTON1
                );
            }
        }
        });
    buttons.push_back(_load);
    addChild(_load);
}



