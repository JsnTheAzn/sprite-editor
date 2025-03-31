#include "sprite.h"

Sprite::Sprite(QObject* parent) :
    QObject(parent),
    width (16),
    frames({new int[width * width * 4]{}}),
    currFrame(0) {}

Sprite::~Sprite() {
    for (int* image : frames) {
        delete[] image;
    }
}

Sprite::Sprite(const Sprite& other) : QObject(other.parent()), width(other.width), currFrame(other.currFrame) {
    frames.reserve(other.frames.size());
    for (int* oldFrame : other.frames) {
        int* newFrame = new int[width * width * 4]; // Allocate new memory
        std::copy(oldFrame, oldFrame + (width * width * 4), newFrame); // Deep copy pixel data
        frames.push_back(newFrame);
    }
}

Sprite& Sprite::operator=(Sprite other) {
    std::swap(width, other.width);
    std::swap(currFrame, other.currFrame);
    std::swap(frames, other.frames);
    return *this; // Copy-and-swap idiom
}

void Sprite::resize(int newWidth) {
    int* newImage = new int[newWidth * newWidth * 4]{};

    resizeCropping(frames[currFrame], newImage, width, newWidth);
    delete[] frames[currFrame];
    frames[currFrame] = newImage;
    width = newWidth;
    emit drawFrame(currFrame);
}

void Sprite::setCurrentFrame(int frame) {
    currFrame = frame;
}

void Sprite::updatePixel(int x, int y, const int (&rgba)[4]) {
    int offset = (y * width + x) * 4;
    int* currImage = frames[currFrame];
    double backPixel[4] = {(double)currImage[offset + 0] / 255, (double)currImage[offset + 1] / 255,
                        (double)currImage[offset + 2] / 255, (double)currImage[offset + 3] / 255};
    double frontPixel[4] = {(double)rgba[0] / 255, (double)rgba[1] / 255,
                        (double)rgba[2] / 255, (double)rgba[3] / 255};
    double finalPixel[4];

    // ALPHA MASK:
    // alpha channel
    finalPixel[3] = 1 - (1 - frontPixel[3]) * (1 - backPixel[3]);
    // red channel
    finalPixel[0] = (frontPixel[0] * frontPixel[3] / finalPixel[3]) +
                    (backPixel[0] * backPixel[3] * (1 - frontPixel[3]) / finalPixel[3]);
    // green channel
    finalPixel[1] = (frontPixel[1] * frontPixel[3] / finalPixel[3]) +
                    (backPixel[1] * backPixel[3] * (1 - frontPixel[3]) / finalPixel[3]);
    // blue channel
    finalPixel[2] = (frontPixel[2] * frontPixel[3] / finalPixel[3]) +
                    (backPixel[2] * backPixel[3] * (1 - frontPixel[3]) / finalPixel[3]);

    currImage[offset + 0] = (int)(finalPixel[0] * 255);
    currImage[offset + 1] = (int)(finalPixel[1] * 255);
    currImage[offset + 2] = (int)(finalPixel[2] * 255);
    currImage[offset + 3] = (int)(finalPixel[3] * 255);

    emit drawFrame(currFrame);
}

void Sprite::clearPixel(int x, int y) {
    int offset = (y * width + x) * 4;
    int* currImage = frames[currFrame];

    currImage[offset + 0] = 0;
    currImage[offset + 1] = 0;
    currImage[offset + 2] = 0;
    currImage[offset + 3] = 0;

    emit drawFrame(currFrame);
}

vector<int*>& Sprite::getFrames() {
    return frames;
}

int Sprite::getFrame() {
    return currFrame;
}

int Sprite::getWidth() {
    return width;
}

void Sprite::getPixel(int x, int y, int (&returnArray)[4]) {
    int offset = (y * width + x) * 4;
    returnArray[0] = frames[currFrame][offset + 0];
    returnArray[1] = frames[currFrame][offset + 1];
    returnArray[2] = frames[currFrame][offset + 2];
    returnArray[3] = frames[currFrame][offset + 3];
}

void Sprite::resizeCropping(const int oldImage[], int newImage[], int oldWidth, int newWidth) {
    // int buffers = newWidth - oldWidth;
    int newEditIndex;
    int oldAccessIndex;
    int minWidth = qMin(oldWidth, newWidth);
    for (unsigned char row = 0; row < minWidth; row++) {
        for (unsigned char col = 0; col < minWidth; col++) {
            newEditIndex = (col * 4) + (row * newWidth * 4);
            oldAccessIndex = (col * 4) + (row * oldWidth * 4);

            newImage[newEditIndex + 0] = oldImage[oldAccessIndex + 0];
            newImage[newEditIndex + 1] = oldImage[oldAccessIndex + 1];
            newImage[newEditIndex + 2] = oldImage[oldAccessIndex + 2];
            newImage[newEditIndex + 3] = oldImage[oldAccessIndex + 3];
        }
    }
}
