#include "sprite.h"

Sprite::Sprite(QObject* parent) :
    QObject(parent),
    width (16),
    frames({new int[width * width * 4]{}}),
    currentFrame(0) {}

Sprite::~Sprite() {
    for (int* image : frames) {
        delete[] image;
    }
}

Sprite::Sprite(const Sprite& other) : QObject(other.parent()), width(other.width), currentFrame(other.currentFrame) {
    frames.reserve(other.frames.size());
    for (int* oldFrame : other.frames) {
        int* newFrame = new int[width * width * 4]; // Allocate new memory
        std::copy(oldFrame, oldFrame + (width * width * 4), newFrame); // Deep copy pixel data
        frames.push_back(newFrame);
    }
}

Sprite& Sprite::operator=(Sprite other) {
    std::swap(width, other.width);
    std::swap(currentFrame, other.currentFrame);
    std::swap(frames, other.frames);
    return *this; // Copy-and-swap idiom
}

void Sprite::resize(int newWidth) {
    int* newImage = new int[newWidth * newWidth * 4]{};

    resizeCropping(frames[currentFrame], newImage, width, newWidth);
    delete[] frames[currentFrame];
    frames[currentFrame] = newImage;
    width = newWidth;
    emit drawFrame(currentFrame);
}

void Sprite::setCurrentFrame(int frame) {
    currentFrame = frame;
}

void Sprite::updatePixel(int x, int y, const int (&rgba)[4]) {
    int offset = (y * width + x) * 4;
    int* currentImage = frames[currentFrame];
    double backPixel[4] = {(double)currentImage[offset + 0] / 255, (double)currentImage[offset + 1] / 255,
                        (double)currentImage[offset + 2] / 255, (double)currentImage[offset + 3] / 255};
    double frontPixel[4] = {(double)rgba[0] / 255, (double)rgba[1] / 255,
                        (double)rgba[2] / 255, (double)rgba[3] / 255};
    double finalPixel[4];

    // ALPHA MASK:
    // alpha channel
    finalPixel[3] = 1 - (1 - frontPixel[3]) * (1 - backPixel[3]);
    if (finalPixel[3] < 0.0001)
        return;
    // red channel
    finalPixel[0] = (frontPixel[0] * frontPixel[3] / finalPixel[3]) +
                    (backPixel[0] * backPixel[3] * (1 - frontPixel[3]) / finalPixel[3]);
    // green channel
    finalPixel[1] = (frontPixel[1] * frontPixel[3] / finalPixel[3]) +
                    (backPixel[1] * backPixel[3] * (1 - frontPixel[3]) / finalPixel[3]);
    // blue channel
    finalPixel[2] = (frontPixel[2] * frontPixel[3] / finalPixel[3]) +
                    (backPixel[2] * backPixel[3] * (1 - frontPixel[3]) / finalPixel[3]);

    currentImage[offset + 0] = (int)(finalPixel[0] * 255);
    currentImage[offset + 1] = (int)(finalPixel[1] * 255);
    currentImage[offset + 2] = (int)(finalPixel[2] * 255);
    currentImage[offset + 3] = (int)(finalPixel[3] * 255);

    emit drawFrame(currentFrame);
}

void Sprite::clearPixel(int x, int y) {
    int offset = (y * width + x) * 4;
    int* currentImage = frames[currentFrame];

    currentImage[offset + 0] = 0;
    currentImage[offset + 1] = 0;
    currentImage[offset + 2] = 0;
    currentImage[offset + 3] = 0;

    emit drawFrame(currentFrame);
}

vector<int*>& Sprite::getFrames() {
    return frames;
}

int Sprite::getFrame() {
    return currentFrame;
}

int Sprite::getWidth() {
    return width;
}

void Sprite::getPixelAtFrame(int frame, int x, int y, int (&returnArray)[4]) {
    int offset = (y * width + x) * 4;
    returnArray[0] = frames[currentFrame][offset + 0];
    returnArray[1] = frames[currentFrame][offset + 1];
    returnArray[2] = frames[currentFrame][offset + 2];
    returnArray[3] = frames[currentFrame][offset + 3];
}

void Sprite::getPixel(int x, int y, int (&returnArray)[4]) {
    getPixelAtFrame(currFrame, x, y, returnArray);
}

void Sprite::resizeCropping(const int oldImage[], int newImage[], int oldWidth, int newWidth) {
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

int Sprite::getNumberOfFrames() {
    return (int)frames.size();
}

void Sprite::deleteCurrentFrame() {
    if ((int)frames.size() == 1)
        return;

    delete frames[currentFrame];
    frames.erase(frames.begin() + currentFrame);

    if (currentFrame > (int)(frames.size() - 1))
        currentFrame = (int)(frames.size() - 1);

    emit drawFrame(currentFrame);
}

void Sprite::insertFrame(int index) {
    int* newImage = new int[width * width * 4]{};
    frames.insert(frames.begin() + index, newImage);
}
