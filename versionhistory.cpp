#include "versionhistory.h"

VersionHistory::VersionHistory() {}


void VersionHistory::addHistory(const Sprite& newSprite) {
    // If we are in the middle of the history, remove future versions
    deleteFutureHistory(currentIndex + 1);

    // Add new version
    if (historyVector.size() == 30) {
        historyVector.erase(historyVector.begin()); // Remove the oldest version if at cap
    } else {
        currentIndex++;
    }

    historyVector.push_back(newSprite);

    emit historyChanged(getCurrentVersion()); // Notify UI
}

void VersionHistory::deleteFutureHistory(int position) {
    if (position < historyVector.size()) {
        historyVector.erase(historyVector.begin() + position, historyVector.end());
    }
}

void VersionHistory::undo() {
    if (currentIndex > 0) {
        currentIndex--;
        emit historyChanged(getCurrentVersion());
    }
}

void VersionHistory::redo() {
    if (currentIndex < historyVector.size() - 1) {
        currentIndex++;
        emit historyChanged(getCurrentVersion());
    }
}

Sprite VersionHistory::getCurrentVersion() const {
    return (currentIndex >= 0 && currentIndex < historyVector.size()) ? historyVector[currentIndex] : Sprite();
}
