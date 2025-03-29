#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include "sprite.h"

class FileManager : public QObject {
    Q_OBJECT
public:
    FileManager(QObject *parent = nullptr);

    void saveFile(const Sprite& sprite);

    Sprite& loadFile();
  
private:
    //Width of the canvas, will always be X by X size.
    int width;
    //Initializes a vector for storing the r,g,b,a pixel data.
    QVector<int> frames;


public slots:
    void saveButtonClicked();

    void loadButtonClicked();
};

#endif // FILEMANAGER_H
