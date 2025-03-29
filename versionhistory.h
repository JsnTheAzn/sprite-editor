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

    void deleteFutureHistory(size_t position);

public:
    VersionHistory();

    void versionClicked(size_t index);

    void addHistory(const Sprite& sprite);

    void deleteHistory(size_t position);

signals:
    void versionChanged(const Sprite& sprite);

};

#endif // VERSIONHISTORY_H
