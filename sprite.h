/**
 * @brief sprite - The sprite class deals with frames and maintaining them.
 * @version 3/31/2025
 * @authors Evan Tucker, Max Donaldson, Ariadne Petroulakis, Jason Khong, Ian Weller
 * * checked by Ariadne Petroulakis
 */

#ifndef SPRITE_H
#define SPRITE_H

#include <QObject>

using std::vector;

class Sprite : public QObject {
    Q_OBJECT
    friend class Model;

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
     * @brief currentFrame - Index of the current selected frame to draw upon.
     */
    int currentFrame;

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
    Sprite(QObject* parent = nullptr);

    /**
     * @brief ~Sprite - Desconstructor that removes the image data from the heap.
     */
    ~Sprite();

    /**
     * @brief Sprite - Copy Constructor for the Sprite Object
     * @param other - other Sprite Object to copy
     */
    Sprite(const Sprite& other);

    /**
     * @brief operator = - Copy assignment operator using swap
     * @param other - other Sprite object to copy
     * @return values swapped between the two
     */
    Sprite& operator=(Sprite other);

    /**
     * @brief resize - Updates the canvas size and crops the image accordingly. Anchors the crop
     * to the top left of the image.
     * @param newWidth - The new side length to set the canvas to (in pixels).
     */
    void resize(int newWidth);

    /**
     * @brief setCurrentFrame
     * @param frame
     */
    void setCurrentFrame(int frame);

    /**
     * @brief updatePixel - Sets the image pixel data at canvas grid position (x, y) to the array
     * given.
     * @param x - The canvas X coordinate (ascending left to right).
     * @param y - The canvas Y coordinate (ascending top to bottom).
     * @param rgba - The color array to set the selected pixel to. Must have a size of 4.
     */
    void updatePixel(int x, int y, const int (&rgba)[4]);

    /**
     * @brief clearPixel - Clears the image pixel data at canvas grid position (x, y).
     * @param x - The canvas X coordinate (ascending left to right).
     * @param y - The canvas Y coordinate (ascending top to bottom).
     */
    void clearPixel(int x, int y);

    /**
     * @brief getFrames - Returns a reference to the image data stored within the sprite.
     * WARNING: NOTE THAT THIS IS A REFERENCE, NOT A COPY.
     * @return The frames vector holding the image data.
     */
    vector<int*>& getFrames();

    /**
     * @brief getFrame - returns index of frame.
     * @return Index of frame
     */
    int getFrame();

    /**
     * @brief getWidth - Returns the side length of the canvas in pixels.
     * @return The number of pixels stored on each side of the canvas.
     */
    int getWidth();

    /**
     * @brief getNumberOfFrames - Returns the current number of frames.
     * @return The number of frames stored in the vector.
     */
    int getNumberOfFrames();

    /**
     * @brief deleteCurrentFrame - Deletes the current frame from the vector of frames.
     */
    void deleteCurrentFrame();

    /**
     * @brief addPrevFrame - Adds a new frame into the vertex of frames.
     * @param Index of the vector you wish to add the frame into
     */
    void insertFrame(int index);

    /**
     * @brief getPixel - Sets the inputted array to the color of the pixel at the given x and y
     * canvas coordinates.
     * @param x - The canvas X coordinate (ascending left to right).
     * @param y - The canvas Y coordinate (ascending top to bottom).
     * @param returnArray - The array to store the color data into. Must have a size of 4.
     */
    void getPixel(int x, int y, int (&returnArray)[4]);

    /**
     * @brief getPixel - Sets the inputted array to the color of the pixel at the given x and y
     * canvas coordinates and any given frame.
     * @param frame - the frame to take from.
     * @param x - The canvas X coordinate (ascending left to right).
     * @param y - The canvas Y coordinate (ascending top to bottom).
     * @param returnArray - The array to store the color data into. Must have a size of 4.
     */
    void getPixelAtFrame(int frame, int x, int y, int (&returnArray)[4]);

signals:
    void drawFrame(int frame);
};

#endif // SPRITE_H
