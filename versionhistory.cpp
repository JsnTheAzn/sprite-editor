#include "versionhistory.h"
#include <QDebug>

VersionHistory::VersionHistory() {
    spriteHistory.push_back(Sprite());
    qDebug() << "New version history created.";
}


void VersionHistory::addHistory(const Sprite& newSprite) {
    qDebug() << "Adding history. Current index: " << currentIndex;
    // If we are in the middle of the history, remove future versions
    deleteFutureHistory(currentIndex + 1);

    // Adding a new version:
    if (spriteHistory.size() == 30) {
        spriteHistory.erase(spriteHistory.begin()); // Remove the oldest version if at cap
    } else {
        currentIndex++;
    }

    spriteHistory.push_back(newSprite);

    emit versionChanged(getCurrentVersion()); // Notify UI
}

void VersionHistory::deleteFutureHistory(int position) {
    if (position < (int)spriteHistory.size()) {
        spriteHistory.erase(spriteHistory.begin() + position, spriteHistory.end());
    }
}

void VersionHistory::undo() {
    if (currentIndex > 0) {
        currentIndex--;
        emit versionChanged(getCurrentVersion());
    }
}

void VersionHistory::redo() {
    if (currentIndex < (int)spriteHistory.size() - 1) {
        currentIndex++;
        emit versionChanged(getCurrentVersion());
    }
}

Sprite VersionHistory::getCurrentVersion() const {
    if((currentIndex >= 0 && currentIndex < (int)spriteHistory.size()))
        return spriteHistory[currentIndex];
    return Sprite();
}
