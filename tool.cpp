#include "tool.h"

Tool::Tool(QObject* parent) :
    QObject(parent) {

}

Tool::Tools Tool::getCurrentTool() {
    return currentTool;
}

void Tool::setCurrentTool(Tools tool) {
    currentTool = tool;
}

void Tool::useTool(Sprite *sprite, int x, int y, const int (&rgba)[4]) {

}

void Tool::fillToolCall (Sprite *sprite, int x, int y, const int (&rgba)[4]) {

}

void Tool::recursiveFill (Sprite *sprite, int x, int y, const int (&newColor), const int (&oldColor)) {

}

