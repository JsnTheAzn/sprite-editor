#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include "sprite.h"

class FileManager : public QObject
{
    Q_OBJECT
public:
    FileManager();

    void saveFile(const Sprite& sprite);

    Sprite& loadFile();
};

#endif // FILEMANAGER_H
