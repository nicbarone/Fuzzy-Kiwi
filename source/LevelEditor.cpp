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

float LevelEditor::addOffset(float pos, string type) {

    if (type == "cat") {
        pos = pos + PLAYER_OFFSET;
    }
    else if (type == "sta") {
        pos = pos + STAIRCASE_DOOR_OFFSET;
    }
    else if (type == "den") {
        pos = pos + CAT_DEN_OFFSET;
    }
    return pos;
}

Vec2 LevelEditor::snapToRow(Vec2 pos, string type) {
    Size dimen = Application::get()->getDisplaySize();
    pos.y = dimen.height - pos.y;
    pos.y = addOffset(closestNum(floorHeights, pos.y), type);
    return pos;
}

void LevelEditor::placeNode() {
    if (pendingNode != nullptr) { //only true if the button clicked places nodes 
        if (!pendingPlacement && Input::get<Mouse>()->buttonReleased().hasLeft()) { //the frame that a button is clicked
            pendingPlacement = true;
        }
        pendingNode->setPosition(snapToRow(Input::get<Mouse>()->pointerPosition(), pendingNode->getName().substr(0, 3)));
        if (pendingPlacement && Input::get<Mouse>()->buttonPressed().hasLeft()) { 
            pendingNode = nullptr;
            pendingPlacement = false;
            releaseButtons();
        }
    }
}

shared_ptr<JsonValue> LevelEditor::toJson() {
    shared_ptr<JsonValue> result = JsonValue::allocObject();
    shared_ptr<JsonValue> playerObject = JsonValue::allocObject();
    shared_ptr<JsonValue> enemyArray = JsonValue::allocArray();
    shared_ptr<JsonValue> decorationsArray = JsonValue::allocArray();
    shared_ptr<JsonValue> staircaseDoorArray = JsonValue::allocArray();
    shared_ptr<JsonValue> doorArray = JsonValue::allocArray();
    shared_ptr<JsonValue> floorValue = JsonValue::alloc(_floorsField->getText() != "" ? (long)stoi(_floorsField->getText()) : 2);
    vector<std::shared_ptr<scene2::SceneNode>> children = _rootScene->getChildren();
    for (auto it = begin(children); it != end(children); ++it) {
        shared_ptr<JsonValue> tempObject = JsonValue::allocObject();
        auto currentNode = it->get();
        if (currentNode->getName().substr(0, 3) == "cat") {
            playerObject->appendChild("x_pos", JsonValue::alloc(currentNode->getPositionX()));
            playerObject->appendChild("level", JsonValue::alloc((currentNode->getPositionY()-FLOOR_OFFSET-PLAYER_OFFSET)/FLOOR_HEIGHT));
            playerObject->appendChild("num_possessions", JsonValue::alloc((long) stoi(currentNode->getName().substr(3))));
        }
        else if (currentNode->getName().substr(0, 3) == "sta") {
            tempObject->appendChild("x_pos", JsonValue::alloc(currentNode->getPositionX()));
            tempObject->appendChild("level", JsonValue::alloc((currentNode->getPositionY() - FLOOR_OFFSET - STAIRCASE_DOOR_OFFSET) / FLOOR_HEIGHT));
            tempObject->appendChild("connection", JsonValue::alloc((long)stoi(currentNode->getName().substr(3))));
            tempObject->appendChild("isDen", JsonValue::alloc(false));
            staircaseDoorArray->appendChild(tempObject);
        }
        else if (currentNode->getName().substr(0, 3) == "den") {
            tempObject->appendChild("x_pos", JsonValue::alloc(currentNode->getPositionX()));
            tempObject->appendChild("level", JsonValue::alloc((currentNode->getPositionY() - FLOOR_OFFSET - CAT_DEN_OFFSET) / FLOOR_HEIGHT));
            tempObject->appendChild("connection", JsonValue::alloc((long)stoi(currentNode->getName().substr(3))));
            tempObject->appendChild("isDen", JsonValue::alloc(true));
            staircaseDoorArray->appendChild(tempObject);
        }
        else if (currentNode->getName().substr(0, 3) == "doo") {
            tempObject->appendChild("x_pos", JsonValue::alloc(currentNode->getPositionX()));
            tempObject->appendChild("level", JsonValue::alloc((currentNode->getPositionY() - FLOOR_OFFSET) / FLOOR_HEIGHT));
            shared_ptr<JsonValue> tempArray = JsonValue::allocArray();
            string keys = currentNode->getName().substr(3);
            if (keys != "") {
                for (auto it = begin(keys); it != end(keys); ++it) {
                    tempArray->appendChild(JsonValue::alloc((long) (*it - '0')));
                }
            }
            tempObject->appendChild("keyInt", tempArray);
            doorArray->appendChild(tempObject);
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
    result->appendChild("player", playerObject);
    result->appendChild("enemy", enemyArray);
    result->appendChild("decorations", decorationsArray);
    result->appendChild("staircase-door", staircaseDoorArray);
    result->appendChild("door", doorArray);
    result->appendChild("floor", floorValue);
    return result;
}

void LevelEditor::fromJson(shared_ptr<JsonValue> json) {
    _rootScene->removeAllChildren();
    shared_ptr<JsonValue> player = json->get("player");
    shared_ptr<JsonValue> staircaseDoor = json->get("staircase-door");
    shared_ptr<JsonValue> door = json->get("door");
    shared_ptr<JsonValue> objectTemp;
    shared_ptr<scene2::SceneNode> temp;
    //create cat
    if (player != nullptr) {
        temp = scene2::AnimationNode::alloc(catTexture, 1, 8);
        temp->setPosition(player->getFloat("x_pos"), player->getInt("level") * FLOOR_HEIGHT + FLOOR_OFFSET + PLAYER_OFFSET);
        temp->setScale(0.15, 0.15);
        _rootScene->addChildWithName(temp, "cat" + player->getInt("num_possessions"));
    }

    //create staircase doors and dens
    if (staircaseDoor != nullptr) {
        for (int i = 0; i < staircaseDoor->size(); i++) {
            objectTemp = staircaseDoor->get(i);
            if (objectTemp->getBool("isDen")) { //cat den
                temp = scene2::AnimationNode::alloc(catDenTexture, 1, 8);
                temp->setPosition(objectTemp->getFloat("x_pos"), objectTemp->getInt("level") * FLOOR_HEIGHT + FLOOR_OFFSET + CAT_DEN_OFFSET);
                temp->setScale(0.25, 0.25);
                _rootScene->addChildWithName(temp, "den" + objectTemp->getInt("connection"));
            }
            else { //staircase door
                temp = scene2::AnimationNode::alloc(staircaseDoorTexture, 1, 8);
                temp->setPosition(objectTemp->getFloat("x_pos"), objectTemp->getInt("level") * FLOOR_HEIGHT + FLOOR_OFFSET + STAIRCASE_DOOR_OFFSET);
                temp->setScale(0.55, 0.55);
                _rootScene->addChildWithName(temp, "sta" + objectTemp->getInt("connection"));
            }
        }
    }

    if (door != nullptr) {
        for (int i = 0; i < door->size(); i++) {
            objectTemp = door->get(i);
            temp = scene2::AnimationNode::alloc(doorTexture, 1, 11);
            temp->setPosition(objectTemp->getFloat("x_pos"), objectTemp->getInt("level") * FLOOR_HEIGHT + FLOOR_OFFSET);
            temp->setScale(-0.65, 0.65);
            shared_ptr<JsonValue> keys = objectTemp->get("keyInt");
            string buffer = "";
            for (int j = 0; j < keys->size(); j++) {
                buffer.append(keys->get(j)->toString());
            }
            _rootScene->addChildWithName(temp, "doo" + buffer);
        }
    }

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

    //Text Fields and Labels
    //num floors
    _floorsLabel = scene2::Label::alloc("Number of Floors:", font);
    _floorsField = scene2::TextField::alloc(Size(35, 30), font);
    _floorsLabel->setPosition(0, 550);
    _floorsField->setPosition(160, 550);
    _floorsField->setBackground(Color4::WHITE);
    _floorsField->activate();
    addChild(_floorsLabel);
    addChild(_floorsField);

    //file path
    _filePathLabel = scene2::Label::alloc("File Name:", font);
    _filePathField = scene2::TextField::alloc(Size(100,30), font);
    _filePathLabel->setPosition(800, 550);
    _filePathField->setPosition(900, 550);
    _filePathField->setBackground(Color4::WHITE);
    _filePathField->activate();
    addChild(_filePathLabel);
    addChild(_filePathField);

    //num possessions
    _numPossessionLabel = scene2::Label::alloc("Num Possessions:", font);
    _numPossessionField = scene2::TextField::alloc(Size(35, 30), font);
    _numPossessionLabel->setPosition(600, 550);
    _numPossessionField->setPosition(760, 550);
    _numPossessionField->setBackground(Color4::WHITE);
    _numPossessionField->activate();
    addChild(_numPossessionLabel);
    addChild(_numPossessionField);

    //door ID
    _doorIDLabel = scene2::Label::alloc("Door ID:", font);
    _doorIDField = scene2::TextField::alloc(Size(20, 30), font);
    _doorIDLabel->setPosition(200, 550);
    _doorIDField->setPosition(280, 550);
    _doorIDField->setBackground(Color4::WHITE);
    _doorIDField->activate();
    addChild(_doorIDLabel);
    addChild(_doorIDField);

    //keys
    _keyLabel = scene2::Label::alloc("Keys:", font);
    _keyField = scene2::TextField::alloc(Size(80, 30), font);
    _keyLabel->setPosition(320, 550);
    _keyField->setPosition(380, 550);
    _keyField->setBackground(Color4::WHITE);
    _keyField->activate();
    addChild(_keyLabel);
    addChild(_keyField);

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
    catTexture = _assets->get<Texture>("cat-walking");
    shared_ptr<scene2::Label> _catText = scene2::Label::alloc("Cat", font);
    _catText->setBackground(Color4::WHITE);
    _cat = scene2::Button::alloc(_catText, Color4::GRAY);
    _cat->setToggle(true);
    _cat->activate();
    _cat->setPosition(120, 0);
    _cat->addListener([=](const std::string& name, bool down) {
            if (down) {
                pendingNode = scene2::AnimationNode::alloc(catTexture, 1, 8);
                pendingNode->setScale(0.15, 0.15);
                pendingNode->setName("cat" + (_numPossessionField->getText() != "" ? _numPossessionField->getText() : "1"));
                _rootScene->addChild(pendingNode);
            }
        });
    buttons.push_back(_cat);
    addChild(_cat);

    //Staircase Door Button
    staircaseDoorTexture = _assets->get<Texture>("staircaseDoor");
    shared_ptr<scene2::Label> _staircaseDoorText = scene2::Label::alloc("Staircase Door", font);
    _staircaseDoorText->setBackground(Color4::WHITE);
    _staircaseDoor = scene2::Button::alloc(_staircaseDoorText, Color4::GRAY);
    _staircaseDoor->setToggle(true);
    _staircaseDoor->activate();
    _staircaseDoor->setPosition(170, 0);
    _staircaseDoor->addListener([=](const std::string& name, bool down) {
        if (down) {
            pendingNode = scene2::AnimationNode::alloc(staircaseDoorTexture, 1, 8);
            pendingNode->setScale(0.55, 0.55);
            pendingNode->setName("sta" + (_doorIDField->getText() != "" ? _doorIDField->getText() : "0"));
            _rootScene->addChild(pendingNode);
        }
        });
    buttons.push_back(_staircaseDoor);
    addChild(_staircaseDoor);

    //Cat Den Button
    catDenTexture = _assets->get<Texture>("staircaseDoor");
    shared_ptr<scene2::Label> _catDenText = scene2::Label::alloc("Cat Den", font);
    _catDenText->setBackground(Color4::WHITE);
    _catDen = scene2::Button::alloc(_catDenText, Color4::GRAY);
    _catDen->setToggle(true);
    _catDen->activate();
    _catDen->setPosition(330, 0);
    _catDen->addListener([=](const std::string& name, bool down) {
        if (down) {
            pendingNode = scene2::AnimationNode::alloc(catDenTexture, 1, 8);
            pendingNode->setScale(0.25, 0.25);
            pendingNode->setName("den" + (_doorIDField->getText() != "" ? _doorIDField->getText() : "0"));
            _rootScene->addChild(pendingNode);
        }
        });
    buttons.push_back(_catDen);
    addChild(_catDen);

    //Door Button
    doorTexture = _assets->get<Texture>("door");
    shared_ptr<scene2::Label> _doorText = scene2::Label::alloc("Door", font);
    _doorText->setBackground(Color4::WHITE);
    _door = scene2::Button::alloc(_doorText, Color4::GRAY);
    _door->setToggle(true);
    _door->activate();
    _door->setPosition(420, 0);
    _door->addListener([=](const std::string& name, bool down) {
        if (down) {
            pendingNode = scene2::AnimationNode::alloc(doorTexture, 1, 11);
            pendingNode->setScale(-0.65, 0.65);
            pendingNode->setName("doo" + _keyField->getText());
            _rootScene->addChild(pendingNode);
        }
        });
    buttons.push_back(_door);
    addChild(_door);

    //Save Button
    shared_ptr<scene2::Label> _saveText = scene2::Label::alloc("Save", font);
    _saveText->setBackground(Color4::WHITE);
    _save = scene2::Button::alloc(_saveText, Color4::GRAY);
    _save->activate();
    _save->setPosition(800, 0);
    _save->addListener([=](const std::string& name, bool down) {
        if (down) {
            if (_filePathField->getText() == "") {
                shared_ptr<JsonWriter> writer = JsonWriter::alloc("levels/test.json");
                writer->writeJson(toJson());
                writer->close();
            }
            else {
                shared_ptr<JsonWriter> writer = JsonWriter::alloc("levels/"+ _filePathField->getText() +".json");
                writer->writeJson(toJson());
                writer->close();
            }
            
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
            if (_filePathField->getText() == "") {
                int returnID = MessageBox(
                    NULL,
                    "Enter the name of the file to load in the top right corner",
                    "Cannot load empty file",
                    MB_ICONWARNING | MB_OK | MB_DEFBUTTON1
                );
            }
            else if (filetool::file_exists("levels\\"+ _filePathField->getText() +".json")) {
                shared_ptr<JsonReader> reader = JsonReader::allocWithAsset("levels\\" + _filePathField->getText() + ".json");
                fromJson(reader->readJson());
            }
            else {
                string messageStr = "File ";
                messageStr.append(Application::get()->getAssetDirectory());
                messageStr.append(_filePathField->getText());
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