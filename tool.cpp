#include "tool.h"
#include "sprite.h"

Tool::Tool(QObject* parent) :
    QObject(parent) {
    currentTool = Tools::Pen;
    for (int i = 0; i < 4; i++)
        rgba[i] = 0;
}

Tool::Tools Tool::getCurrentTool() {
    return currentTool;
}

void Tool::setCurrentTool(Tools tool) {
    currentTool = tool;
}

void Tool::useTool(Sprite *sprite, int x, int y, const int (&rgba)[4]) {
    int erase[4] = {0, 0, 0, 0};
    switch(currentTool) {
    case Tools::Pen:
        sprite->updatePixel(x, y, rgba);
        break;
    case Tools::Eraser:
        sprite->updatePixel(x, y, erase);
        break;
    case Tools::PaintBucket:
        fillToolCall(sprite, x, y, rgba);
        break;
    }
    
void Tool::setCurrentColor(const int (&newColor)[4]) {
    rgba[0] = newColor[0];
    rgba[1] = newColor[1];
    rgba[2] = newColor[2];
    rgba[3] = newColor[3];
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
    recursiveFill(sprite, x, y, rgba, oldColor);
}

void Tool::recursiveFill (Sprite *sprite, int x, int y, const int (*newColor), const int (*oldColor)) {
    int width = sprite->getWidth();

    if(x < 0 || x >= width || y < 0 || y >= width) {
        return;
    }

    int currentColor[4];
    sprite->getPixel(x, y, currentColor);
    //Base case
    if (currentColor[0] == newColor[0] && currentColor[1] == newColor[1] &&
        currentColor[2] == newColor[2] && currentColor[3] == newColor[3]) {
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

