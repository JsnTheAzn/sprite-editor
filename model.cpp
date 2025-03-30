#include "model.h"
#include "filemanager.h"

Model::Model(QObject* parent) :
    QObject(parent) {

    sprite = new Sprite();
    tool = new Tool();
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

}
