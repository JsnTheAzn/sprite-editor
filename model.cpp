#include "model.h"
#include "filemanager.h"

Model::Model(QObject* parent) :
    QObject(parent),
    sprite(parent) {

    tool = new Tool();
}
<<<<<<< HEAD
=======

Tool::Tools Model::getTool() {
    return tool->getCurrentTool();
}

void Model::setTool(Tool::Tools clickedTool) {
    tool->setCurrentTool(clickedTool);
}

void Model::useTool(Sprite *sprite, int x, int y, const int (&rgba)[4]) {
    tool->useTool(sprite, x, y);
}
>>>>>>> 04e2aeaf1af4285b94247c16031e17cffaf428b9
