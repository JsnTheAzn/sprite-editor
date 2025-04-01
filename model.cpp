/**
 * @version 3/31/2025
 * @authors Evan Tucker, Max Donaldson, Ariadne Petroulakis, Jason Khong, Ian Weller
 * * checked by Ian Weller
 */

#include "model.h"
#include "mainwindow.h"
#include <QDebug>

Model::Model(QObject* parent) :
    QObject(parent) {

    colorPalette = nullptr;
    fileManager = nullptr;

    sprite = new Sprite();
    tool = new Tool();
    versionHistory = new VersionHistory();
}

Model::~Model(){
    delete tool;
    delete versionHistory;
}

// SPRITE
Sprite* Model::getSpritePointer() {
    return sprite;
}

// COLORPALETTE
void Model::addColorToPalette(){}

// TOOL
Tool* Model::getToolPointer(){
    return tool;
}

void Model::setTool(Tool::Tools clickedTool) {
    tool->setCurrentTool(clickedTool);
    emit toolChanged();
}
void Model::setToolColor(const int (&rgba)[4]) {
    tool->setCurrentColor(rgba);
}

void Model::useTool(Sprite *sprite, int x, int y) {
    tool->useTool(sprite, x, y);
    emit updateSprite();
}

Tool::Tools Model::getTool() {
    return tool->getCurrentTool();
}

//FILEMANAGER
void Model::saveSprite(){
    fileManager->saveButtonClicked();
}

void Model::loadSprite(){
    fileManager->loadButtonClicked();
}

void Model::loadCanvas(int width, vector<int> &frames) {
    sprite->resize(width);
    vector<int*>& spriteFrames = sprite->getFrames();
    spriteFrames.clear();

    int frameSize = (width * width) * 4;

    for(int i = 0; i < (int)frames.size() / frameSize; i++) {
        int *pixelData = new int[frameSize];
        for (int j = 0; j < frameSize; j++) {
            pixelData[j] = frames[i * frameSize + j];
        }
        spriteFrames.push_back(pixelData);
    }
    sprite->setCurrentFrame(0);

    emit updateSprite();
}

//VERSION HISTORY
VersionHistory* Model::getVersionHistoryPointer() {
    return versionHistory;
}

void Model::appendVersion(Sprite* updatedSprite) {
    versionHistory->addHistory(*updatedSprite);
}

void Model::setVersionIndex(int index) {
    versionHistory->setVersionIndex(index);
}

int Model::getVersionIndex() {
    return versionHistory->getVersionIndex();
}

int Model::getHistorySize() {
    return versionHistory->size();
}


