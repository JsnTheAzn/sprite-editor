#include "model.h"

Model::Model(QObject* parent) :
    QObject(parent) {

    tool = new Tool();
    sprite = new Sprite();
}

Tool::Tools Model::getTool() {
    return tool->getCurrentTool();
}

void Model::setTool(Tool::Tools clickedTool) {
    tool->setCurrentTool(clickedTool);
}

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
