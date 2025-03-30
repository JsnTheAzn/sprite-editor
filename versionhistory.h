#ifndef VERSIONHISTORY_H
#define VERSIONHISTORY_H

#include <QObject>
#include "sprite.h"

using std::vector;

class VersionHistory : public QObject
{
    Q_OBJECT

private:

    vector<Sprite> historyVector;

    int currentIndex = -1;

    void deleteFutureHistory(int position);

public:
    VersionHistory();

    void versionClicked();

    void addHistory(const Sprite& newSprite);

    void undo();
    void redo();

    void deleteHistory(int position);

    Sprite getCurrentVersion() const;

signals:
    void historyChanged(const Sprite& currentSprite);
};

#endif // VERSIONHISTORY_H
