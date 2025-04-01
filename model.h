#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include "sprite.h"
#include "colorpalette.h"
#include "filemanager.h"
#include "versionhistory.h"
#include "tool.h"

class Model : public QObject {
    Q_OBJECT

private:

    /**
     * @brief sprite - Stores a refrence to the image and frame data for the current sprite.
     */
    Sprite *sprite;

    /**
     * @brief tool - Manages the tools and their properties.
     */
    Tool *tool;

    /**
     * @brief colorPalette - Stores all colors in the loaded palette.
     */
    ColorPalette *colorPalette;

    /**
     * @brief fileManager - Manages all saving and loading functionalities for both the Sprite
     * and the ColorPalette.
     */
    FileManager *fileManager;

    /**
     * @brief versionHistory - Manages and stores the recent history for the current session.
     */
    VersionHistory *versionHistory;

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

    /**
     * @brief Model - constructs a model object.
     * @param parent
     */
    explicit Model(QObject *parent = nullptr);

    /**
     * @brief destructor for the Model class.
     */
    ~Model();

    /**
     * @brief getTool - Gets the current used by the user.
     * @return The current tool used.
     */
    Tool::Tools getTool();

    /**
     * @brief setTool - Sets to the tool clicked by the user.
     * @param clickedTool - The tool clicked by the user.
     */
    void setTool(Tool::Tools clickedTool);

    /**
     * @brief setToolColor - Sets the current color of the tool.
     * @param rgba - Sets the color of the current tool, where rgba[0]
     * is red, rgba[1] is green, rgba[2] is blue, and rgba[3] is alpha.
     */
    void setToolColor(const int (&rgba)[4]);

    /**
     * @brief getSpritePtr - Returns a pointer to the sprite object stored
     * in the Model.
     * @return The pointer to the Model's sprite.
     */
    void useTool(Sprite *sprite, int x, int y);

    /**
     * @brief setVersionIndex - Sets the current version to a different one.
     */
    void setVersionIndex(int index);

    /**
     * @brief getVersionIndex - Returns the version number.
     * @return The current version number.
     */
    int getVersionIndex();
    
    /**
     * @brief getSpritePointer - Returns a pointer to the Sprite object stored
     * in the Model.
     * @return The pointer to the Model's tool.
     */
    Sprite* getSpritePointer();

    /**
     * @brief getToolPointer - Returns a pointer to the Tool object stored
     * in the Model.
     * @return The pointer to the Model's tool.
     */
    Tool* getToolPointer();

    /**
     * @brief getVersionHistoryPointer - Returns a pointer to the
     * version history object stored in the Model.
     * @return The pointer to the Model's versionHistory.
     */
    VersionHistory* getVersionHistoryPointer();

    /**
     * @brief getHistorySize - Gets the size of the version history vector.
     * @return The size of the version history vector as an int.
     */
    int getHistorySize();

public slots:
    /**
     * @brief loadCanvas - Notifies for the canvas to be updated upon file load.
     * @param width - The image data width.
     * @param frames - The image data frames.
     */
    void loadCanvas(int width, std::vector<int> &frames);
    
    /**
     * @brief saveSprite - Executes the necessary code to save the sprite as
     * a .ssp file.
     */
    void saveSprite();

    /**
     * @brief loadSprite - Executes the necessary code to load the sprite from
     * a .ssp file.
     */
    void loadSprite();

    /**
     * @brief addColorToPalette - Adds a color to colorPalette
     */
    void addColorToPalette();

    /**
     * @brief appendVersion - Adds a version of this sprite to the version history.
     * @param updatedSprite - The updated sprite to add.
     */
    void appendVersion(Sprite* updatedSprite);

signals:
    /**
     * @brief updateSprite - Signals that the internal sprite state
     * has been updated.
     */
    void updateSprite();

    /**
     * @brief toolChanged - Emitted when a new tool is clicked.
     */
    void toolChanged();
};

#endif // MODEL_H
