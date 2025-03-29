#include "model.h"
#include "filemanager.h"

Model::Model(QObject* parent) :
    QObject(parent) {

}


    // void fillToolCall (x, y, color[4]) {
    //     Find the current color at x, y;

    //     Call a recursive method and pass through (x, y, color, and old color);
    // }

    // void recursive (x, y, const ref newColor, const ref oldColor) {
    //     Use the sprite setPixel method with (x, y, color);
    //     Check the pixel above. If all the rgb values match with oldColor, call recursive(x, y-1, newColor, oldColor);
    //     Check the pixel right. If all the rgb values match with oldColor, call recursive(x+1, y, newColor, oldColor);
    //     Check the pixel below. If all the rgb values match with oldColor, call recursive(x, y+1, newColor, oldColor);
    //     Check the pixel left. If all the rgb values match with oldColor, call recursive(x-1, y, newColor, oldColor);
    // }
// }
