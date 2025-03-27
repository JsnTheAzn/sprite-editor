#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include "sprite.h"
#include "colorpalette.h"
#include "filemanager.h"
#include "versionhistory.h"
#include "tool.h"

class Model : public QObject
{
    Q_OBJECT

private:

    /**
     * @brief sprite - Stores the image and frame data for the current sprite.
     */
    Sprite sprite;

    /**
     * @brief colorPalette - Stores all colors in the loaded palette.
     */
    ColorPalette colorPalette;

    /**
     * @brief fileManager - Manages all saving and loading functionalities for both the Sprite
     * and the ColorPalette.
     */
    FileManager fileManager;

    /**
     * @brief versionHistory - Manages and stores the recent history for the current session.
     */
    VersionHistory versionHistory;

    /**
     * @brief tool - Manages the tools and their properties.
     */
    Tool tool;

    /**
     * @brief currFrame - Stores the currently showcased frame on the animation preview.
     */
    int currFrame;

    /**
     * @brief currColor - Stores the current Red, Green, Blue, and Alpha values of the color
     * to draw.
     */
    int currColor[4];

    /**
     * @brief frameRate - Stores the current speed of the sprite animation.
     */
    int frameRate;

    /**
     * @brief playAnimation - Loops through the frames to play the current animation of the
     * sprite somewhere.
     */
    void playAnimation();

    /**
     * @brief nextFrame - Goes to the next Frame (I forgot what this does)
     */
    void nextFrame();

public:
    explicit Model(QObject *parent = nullptr);
};

#endif // MODEL_H
