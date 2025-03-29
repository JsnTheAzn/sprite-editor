#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include "sprite.h"

class FileManager : public QObject {
    Q_OBJECT
public:
    /**
     * @brief FileManager - Default FileManager constructor.
     */
    FileManager(QObject *parent = nullptr);

    /**
     * @brief saveFile - Saves the sprite in .ssp format.
     * @param sprite - The sprite to be saved.
     */
    void saveFile(const Sprite& sprite);

    /**
     * @brief loadFile - Loads the given .ssp file into the canvas.
     */
    Sprite& loadFile();
  
private:
    /**
     * @brief width - The width of the canvas to be squared.
     */
    int width;

    /**
     * @brief frames - A vector of ints to keep track of the frames.
     */
    QVector<int> frames;

public slots:
    /**
     * @brief saveButtonClicked - Handles when the UI save button is clicked.
     */
    void saveButtonClicked();
    /**
     * @brief loadButtonClicked - Handles when the UI load button is clicked.
     */
    void loadButtonClicked();
};

#endif // FILEMANAGER_H
