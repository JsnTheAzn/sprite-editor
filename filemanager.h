/**
 * @brief filemanager - The FileManager class. This handles the saving and
 * loading of the application.
 * @version 3/31/2025
 * @authors Evan Tucker, Max Donaldson, Ariadne Petroulakis, Jason Khong, Ian Weller
 * * checked by Evan Tucker
 */

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include "sprite.h"

class FileManager : public QObject {
    Q_OBJECT
    friend class Model;

public:
    /**
     * @brief FileManager - Default FileManager constructor.
     */
    FileManager(QObject *parent = nullptr);

    /**
     * @brief setSprite - Sets the given sprite pointer to the stored file.
     * @param sprite - The pointer of the Sprite to alter.
     */
    void setSprite(Sprite *sprite);

    /**
     * @brief saveFile - Saves the sprite in .ssp format.
     * @param sprite - The sprite to be saved.
     */
    void saveFile(const Sprite& sprite);
  
private:
    /**
     * @brief sprite - An instance of sprite.
     */
    Sprite *sprite;

public slots:
    /**
     * @brief saveButtonClicked - Handles when the UI save button is clicked.
     */
    void saveButtonClicked();
    /**
     * @brief loadButtonClicked - Handles when the UI load button is clicked.
     */
    void loadButtonClicked();

signals:
    /**
     * @brief loadCanvas - Notifies for the canvas to be updated upon file load.
     * @param width - The image data width.
     * @param frames - The image data frames.
     */
    void loadCanvas(int width, std::vector<int> &frames);
};

#endif // FILEMANAGER_H
