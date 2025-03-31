#include "tool.h"
#include "sprite.h"

Tool::Tool(QObject* parent) :
    QObject(parent) {
    currentTool = Tools::Pen;
    for (int i = 0; i < 4; i++)
        currentColor[i] = 0;
}

Tool::Tools Tool::getCurrentTool() {
    return currentTool;
}

void Tool::setCurrentTool(Tools tool) {
    currentTool = tool;
}

void Tool::useTool(Sprite *sprite, int x, int y) {
    switch(currentTool) {
    case Tools::Pen:
        sprite->updatePixel(x, y, currentColor);
        break;
    case Tools::Eraser:
        sprite->clearPixel(x, y);
        break;
    case Tools::PaintBucket:
        fillToolCall(sprite, x, y);
        break;
    }
}
    
void Tool::setCurrentColor(const int (&newColor)[4]) {
    currentColor[0] = newColor[0];
    currentColor[1] = newColor[1];
    currentColor[2] = newColor[2];
    currentColor[3] = newColor[3];
}

void Tool::fillToolCall (Sprite *sprite, int x, int y) {
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
    recursiveFill(sprite, x, y, currentColor, oldColor);
}

void Tool::recursiveFill (Sprite *sprite, int x, int y, const int (*newColor), const int (*oldColor)) {
    int width = sprite->getWidth();

    if(x < 0 || x >= width || y < 0 || y >= width) {
        return;
    }

    int returnColor[4];
    sprite->getPixel(x, y, returnColor);
    //Base case
    if (returnColor[0] == newColor[0] && returnColor[1] == newColor[1] &&
        returnColor[2] == newColor[2] && returnColor[3] == newColor[3]) {
        return;
    }

    int newColors[4] = {newColor[0], newColor[1], newColor[2], newColor[3]};
    sprite->updatePixel(x, y, newColors);

    //Check if all rgba colors match the old color.
    //Recursive fills.
    //Check top pixel.
    if(y > 0) {
        int color[4];
        sprite->getPixel(x, y - 1, color);

        if (color[0] == oldColor[0] && color[1] == oldColor[1] &&
            color[2] == oldColor[2] && color[3] == oldColor[3]) {
            recursiveFill(sprite, x, y - 1, newColor, oldColor);
        }
    }
    //Check right pixel.
    if(x < width - 1) {
        int color[4];
        sprite->getPixel(x + 1, y, color);

        if (color[0] == oldColor[0] && color[1] == oldColor[1] &&
            color[2] == oldColor[2] && color[3] == oldColor[3]) {
            recursiveFill(sprite, x + 1, y, newColor, oldColor);
        }
    }
    //Check bottom pixel.
    if(y < width - 1) {
        int color[4];
        sprite->getPixel(x, y + 1, color);

        if (color[0] == oldColor[0] && color[1] == oldColor[1] &&
            color[2] == oldColor[2] && color[3] == oldColor[3]) {
            recursiveFill(sprite, x, y + 1, newColor, oldColor);
        }
    }
    //Check left pixel.
    if(x > 0) {
        int color[4];
        sprite->getPixel(x - 1, y, color);

        if (color[0] == oldColor[0] && color[1] == oldColor[1] &&
            color[2] == oldColor[2] && color[3] == oldColor[3]) {
            recursiveFill(sprite, x - 1, y, newColor, oldColor);
        }
    }
}
