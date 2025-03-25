#ifndef SPRITE_H
#define SPRITE_H

#include <QObject>

using std::vector;

class Sprite : public QObject
{
    Q_OBJECT

private:

    /**
     * @brief width - Stores the side length of the canvas in terms of pixels.
     */
    int width;

    /**
     * @brief frames - Stores the pixel data (integer array) for each frame (vector entry)
     */
    vector<int*> frames;

    /**
     * @brief currFrame - Index of the current selected frame to draw upon.
     */
    int currFrame;

    /**
     * @brief resizeCropping - Helper method to transform the image upon canvas size change.
     */
    void resizeCropping();

public:
    Sprite();
    void resize(int newWidth);
    void setCurrentFrame(int frame);
    void updatePixel(int x, int y, int red, int green, int blue, int alpha);
    vector<int[]> getFrames();
};

#endif // SPRITE_H
