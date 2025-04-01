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
     * @brief spriteHistory - Holds, in order, a history of the last
     * 30 tool strokes done by the user.
     */
    vector<Sprite> spriteHistory;

    /**
     * @brief currentIndex - Holds the current version index of the sprite,
     * stored as a number between 1 and 30.
     */
    int currentIndex;

    /**
     * @brief deleteFutureHistory - Deletes the entire history beyond an index.
     * @param versionNumber - The version to delete from, stored as an integer from 1-30
     */
    void deleteFutureHistory(int versionNumber);

public:
    /**
     * @brief VersionHistory - Creates a VersionHistory object.
     */
    VersionHistory();

    /**
     * @brief updateVersion - Updates the version of the sprite.
     */
    void updateVersion();

    /**
     * @brief addHistory - Adds an event to the version history.
     * @param newSprite - The new version of the sprite, to be appended to the history.
     */
    void addHistory(const Sprite& newSprite);

    /**
     * @brief deleteHistory - Removes the history from the given point. All history after the
     * point is deleted.
     * @param position - The index of the history to remove.
     */
    void deleteHistory(int position);

    /**
     * @brief undo - Returns to the previous version of the Sprite.
     */
    void undo();

    /**
     * @brief redo - Returns to the next version of the Sprite.
     */
    void redo();

    /**
     * @brief getSpriteAtCurrentVersion - Fetches the sprite at the current version.
     * @return The current version of th sprite.
     */
    Sprite getSpriteAtCurrentVersion() const;

    /**
     * @brief setVersionIndex - Sets the current version.
     * @param index - The version to revert to.
     */
    void setVersionIndex(int index);

    /**
     * @brief getVersionIndex - Gets the current version number.
     * @return The current version number
     */
    int getVersionIndex() const { return currentIndex; };

    /**
     * @brief size - Returns the total size of the version history.
     * @return The size as an int.
     */
    int size() const { return (int)spriteHistory.size(); }

signals:
    /**
     * @brief versionChanged - Emits a signal that the current version
     * has been changed.
     * @param currentSprite - Sends a copy of the current Sprite.
     */
    void versionChanged(const Sprite& currentSprite);
};

#endif // VERSIONHISTORY_H
