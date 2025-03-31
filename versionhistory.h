#ifndef VERSIONHISTORY_H
#define VERSIONHISTORY_H

#include <QObject>
#include "sprite.h"

using std::vector;

class VersionHistory : public QObject {
    Q_OBJECT
    friend class Model;

private:
    /**
     * @brief spriteHistory - holds, in order, a history of the last
     * 30 tool strokes done by the user.
     */
    vector<Sprite> spriteHistory;

    /**
     * @brief currentIndex - holds the current version index of the sprite,
     * stored as a number between 1 and 30.
     */
    int currentIndex;

    /**
     * @brief deleteFutureHistory - deletes the entire history beyond an index.
     * @param versionNumber - the version to delete from, stored as an integer from 1-30
     */
    void deleteFutureHistory(int versionNumber);

public:
    /**
     * @brief VersionHistory - creates a VersionHistory object.
     */
    VersionHistory();

    /**
     * @brief updateVersion - updates the version of the sprite.
     */
    void updateVersion();

    /**
     * @brief addHistory - adds an event to the version history.
     * @param newSprite - the new version of the sprite, to be appended to the history.
     */
    void addHistory(const Sprite& newSprite);

    /**
     * @brief deleteHistory
     * @param position
     */
    void deleteHistory(int position);

    /**
     * @brief undo - returns to the previous version of the Sprite.
     */
    void undo();

    /**
     * @brief redo - returns to the next version of the Sprite.
     */
    void redo();

    /**
     * @brief getSpriteAtCurrentVersion - fetches the sprite at the current version.
     * @return the current version of th sprite.
     */
    Sprite getSpriteAtCurrentVersion() const;

signals:
    /**
     * @brief versionChanged - emits a signal that the current version
     * has been changed.
     * @param currentSprite - sends a copy of the current Sprite.
     */
    void versionChanged(const Sprite& currentSprite);
};

#endif // VERSIONHISTORY_H
