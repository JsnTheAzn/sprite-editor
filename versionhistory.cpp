#include "versionhistory.h"
#include <QDebug>

VersionHistory::VersionHistory() : currentIndex(0) {
    spriteHistory.push_back(Sprite());
}


void VersionHistory::addHistory(const Sprite& newSprite) {
    // If we are in the middle of the history, remove future versions
    deleteFutureHistory(currentIndex + 1);

    // Adding a new version:
    if (spriteHistory.size() == 30) {
        spriteHistory.erase(spriteHistory.begin());
    } else {
        currentIndex++;
    }
    spriteHistory.push_back(newSprite);
    emit versionChanged(getSpriteAtCurrentVersion());
}

void VersionHistory::deleteFutureHistory(int position) {
    if (position < (int)spriteHistory.size()) {
        spriteHistory.erase(spriteHistory.begin() + position, spriteHistory.end());
    }
}

void VersionHistory::undo() {
    if (currentIndex > 0) {
        currentIndex--;
        emit versionChanged(getSpriteAtCurrentVersion());
    }

}

void VersionHistory::redo() {
    if (currentIndex < (int)spriteHistory.size() - 1) {
        currentIndex++;
        emit versionChanged(getSpriteAtCurrentVersion());
    }

}

Sprite VersionHistory::getSpriteAtCurrentVersion() const {
    if((currentIndex >= 0 && currentIndex < (int)spriteHistory.size()))
        return spriteHistory[currentIndex];
    return Sprite();
}

void VersionHistory::setVersionIndex(int index) {
    if (index >= 0 && index < (int)spriteHistory.size()) {
        currentIndex = index;
        emit versionChanged(getSpriteAtCurrentVersion());
    }
}
