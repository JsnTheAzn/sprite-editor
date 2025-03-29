#include "versionhistory.h"

VersionHistory::VersionHistory() {}


void VersionHistory::addHistory(const Sprite& sprite) {
    deleteFutureHistory(historyVector.size());

    historyVector.push_back(sprite);
}

void VersionHistory::deleteFutureHistory(size_t position) {
    if (position < historyVector.size()) {
        historyVector.erase(historyVector.begin() + position, historyVector.end());
    }
}


void VersionHistory::deleteHistory(size_t position) {
    if (position >= 0 && position < historyVector.size()) {
        historyVector.erase(historyVector.begin() + position);
    }
}

void VersionHistory::versionClicked(size_t index) {
    if (index >= 0 && index < historyVector.size()) {
        emit versionChanged(historyVector[index]);
    }
}
