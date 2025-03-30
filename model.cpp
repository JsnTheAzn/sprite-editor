#include "model.h"

Model::Model(QObject* parent) :
    QObject(parent) {

    sprite = new Sprite();
    tool = new Tool();
    sprite = new Sprite();
}

Model::~Model(){
    delete tool;
    delete sprite;
}

Sprite* Model::getSpritePtr() {
    return sprite;
}

Tool* Model::getToolPtr(){
    return tool;
}

//TOOL METHODS
void Model::setTool(Tool::Tools clickedTool) {
    tool->setCurrentTool(clickedTool);
}
void Model::setToolColor(const int (&rgba)[4]) {
    tool->setCurrentColor(rgba);
}

//FILEMANAGER METHODS
void Model::saveSprite(){
    fileManager.saveButtonClicked();
    //fileManager.saveFile(sprite);
}

void Model::loadSprite(){
    fileManager.loadButtonClicked();
}

Tool::Tools Model::getTool() {
    return tool->getCurrentTool();
}

//COLORPALETTE
void Model::addColorToPalette(){

void Model::useTool(Sprite *sprite, int x, int y) {
    tool->useTool(sprite, x, y);
    emit updateSprite();
}

void Model::loadCanvas(int width, vector<int> &frames) {
    sprite->resize(width);
    vector<int*>& spriteFrames = sprite->getFrames();
    spriteFrames.clear();

    int frameSize = (width * width) * 4;

    for(int i = 0; i < frames.size() / frameSize; i++) {
        int *pixelData = new int[frameSize];
        for (int j = 0; j < frameSize; j++) {
            pixelData[j] = frames[i * frameSize + j];
        }
        spriteFrames.push_back(pixelData);
    }
    sprite->setCurrentFrame(0);

    emit updateSprite();
}

Sprite* Model::getSprite() {
    return sprite;
}
