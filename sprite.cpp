#include "sprite.h"

Sprite::Sprite(QObject* parent) :
    QObject(parent),
    width (16),
    frames({new int[width * width * 4]{}}),
    currFrame(0) {}

Sprite::~Sprite() {

}

void Sprite::resize(int newWidth) {
    int* newImage = new int[newWidth * newWidth * 4]{};

    resizeCropping(frames[currFrame], newImage, width, newWidth);
    delete[] frames[currFrame];
    frames[currFrame] = newImage;
    width = newWidth;
}

void Sprite::setCurrentFrame(int frame) {
    currFrame = frame;
}

void Sprite::updatePixel(int x, int y, int red, int green, int blue, int alpha) {
    int offset = (y * width + x) * 4;
    int* currImage = frames[currFrame];

    currImage[offset + 0] = red;
    currImage[offset + 1] = green;
    currImage[offset + 2] = blue;
    currImage[offset + 3] = alpha;
}

vector<int*>& Sprite::getFrames() {
    return frames;
}

void Sprite::resizeCropping(const int oldImage[], int newImage[], int oldWidth, int newWidth) {
    // int buffers = newWidth - oldWidth;
    int newEditIndex;
    int oldAccessIndex;
    for (unsigned char row = 0; row < oldWidth; row++) {
        for (unsigned char col = 0; col < oldWidth; col++) {
            newEditIndex = (col * 4) + (row * newWidth * 4);
            oldAccessIndex = (col * 4) + (row * oldWidth * 4);

            newImage[newEditIndex + 0] = oldImage[oldAccessIndex + 0];
            newImage[newEditIndex + 1] = oldImage[oldAccessIndex + 1];
            newImage[newEditIndex + 2] = oldImage[oldAccessIndex + 2];
            newImage[newEditIndex + 3] = oldImage[oldAccessIndex + 3];
        }
    }
}
