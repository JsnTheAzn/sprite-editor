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

    void deleteFutureHistory(int position);

public:
    VersionHistory();

    void versionClicked();

    void addHistory();

    void deleteHistory(int position);
};

#endif // VERSIONHISTORY_H
