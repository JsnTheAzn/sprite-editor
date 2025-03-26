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
     * @param oldImage[] - The pointer to the old image to incoporate into the cropping.
     * @param newImage[] - The pointer to the new image. Will be altered after the method returns.
     * @param oldWidth - The previous width of the image before resizing.
     * @param newWidth - The width of the resized image.
     */
    void resizeCropping(const int oldImage[], int newImage[], int oldWidth, int newWidth);

public:

    /**
     * @brief Sprite - Default constructor. Initializes a transparent image of one frame.
     */
    Sprite(QObject* parent);

    /**
     * @brief ~Sprite - Desconstructor that removes the image data from the heap.
     */
    ~Sprite();

    /**
     * @brief resize -
     * @param newWidth -
     */
    void resize(int newWidth);
    void setCurrentFrame(int frame);
    void updatePixel(int x, int y, int red, int green, int blue, int alpha);
    vector<int*>& getFrames();
};

#endif // SPRITE_H
