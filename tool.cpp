#include "tool.h"
#include "sprite.h"

Tool::Tool(QObject* parent) :
    QObject(parent) {

}

Tool::Tools Tool::getCurrentTool() {
    return currentTool;
}

void Tool::setCurrentTool(Tools tool) {
    currentTool = tool;
}

void Tool::useTool(Sprite *sprite, int x, int y, const int *rgba) {
    //TODO: Fill in with multiple cases, based on which tool is being used,
    //and then its functionality.
}

void Tool::fillToolCall (Sprite *sprite, int x, int y, const int *rgba) {
    int width = sprite->getWidth();
    //Check for out-of-bounds.
    if(x < 0 || x >= width || y < 0 || y >= width) {
        return;
    }

    //Create an array to hold 4 integer values for RGBA.
    int oldColor[4];
    //Find current color at x, y
    sprite->getPixel(x, y, oldColor);

    //Call recursive method.
    recursiveFill(sprite, x, y, rgba, oldColor);
}

void Tool::recursiveFill (Sprite *sprite, int x, int y, const int (*newColor), const int (*oldColor)) {
    int width = sprite->getWidth();

    if(x < 0 || x >= width || y < 0 || y >= width) {
        return;
    }

    int currentColor[4];
    sprite->getPixel(x, y, currentColor);

    //Check if all rgba colors match the old color.
    if(currentColor[0] != oldColor[0] || currentColor[1] != oldColor[1] ||
        currentColor[2] != oldColor[2] || currentColor[3] != oldColor[3]) {
        return;
    }

    int newColors[4] = {newColor[0], newColor[1], newColor[2], newColor[3]};
    sprite->updatePixel(x, y, newColors);

    //Recursive fills.
    recursiveFill(sprite, x, y - 1, newColor, oldColor);
    recursiveFill(sprite, x + 1, y, newColor, oldColor);
    recursiveFill(sprite, x, y + 1, newColor, oldColor);
    recursiveFill(sprite, x - 1, y, newColor, oldColor);
}

