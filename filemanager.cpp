#include "filemanager.h"
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QVector>

/**
 * @version 3/31/2025
 * @authors Evan Tucker, Max Donaldson, Ariadne Petroulakis, Jason Khong, Ian Weller
 * * checked by Max Donaldson
 */

/*
 * Sources used:
 * https://doc.qt.io/qt-6/qfile.html
 * https://doc.qt.io/qt-6/qfiledialog.html
 * https://stackoverflow.com/questions/72134878/qt-creator-how-can-i-store-save-selected-file-to-project-directory
 * https://doc.qt.io/qt-6/json.html
 * https://doc.qt.io/qt-6/qstring.html
 * https://stackoverflow.com/questions/24936564/set-qfile-save-directory
 * https://forum.qt.io/topic/124003/how-to-write-data-into-json/2
 *
 * For deserializing:
 * https://stackoverflow.com/questions/15893040/how-to-create-read-write-json-files-in-qt5
*/

FileManager::FileManager(QObject *parent) : QObject{parent}, sprite(nullptr) {}

void FileManager::setSprite(Sprite *sprite) {
    this->sprite = sprite;
}

void FileManager::saveButtonClicked() {
    QString savedFile = QFileDialog::getSaveFileName(nullptr, QObject::tr("Save .SSP File"), "C:/", "SSP (*.ssp)");

    // Add .ssp extension if file isn't saved with it manually.
    if(!savedFile.endsWith((".ssp"), Qt::CaseSensitive)) {
        savedFile += ".ssp";
    }

    // This will represent the data of the saved canvas width.
    QJsonObject jsonData;
    int width = sprite->getWidth();
    jsonData["Width"] = width;

    int frameSize = (width * width) * 4;
    vector<int*>& spriteFrames = sprite->getFrames();

    // valuesArray will represent the pixel data of the r,g,b,a values.
    // framesArray will represent all the total valuesArray.
    QJsonArray framesArray;
    for (int i = 0; i < spriteFrames.size(); i++) {
        QJsonArray valuesArray;
        int* pixelData = spriteFrames[i];
        for(int j = 0; j < frameSize; j++) {
            valuesArray.append(pixelData[j]);
        }
        framesArray.append(valuesArray);
    }

    jsonData["Frames"] = framesArray;

    // Write the data in json to the file.
    QFile file(savedFile);
    if(file.open(QFile::WriteOnly)) {
        QTextStream stream(&file);
        stream << QJsonDocument(jsonData).toJson(QJsonDocument::Indented);
        file.close();
    }
}

void FileManager::loadButtonClicked() {
    QString loadedFile = QFileDialog::getOpenFileName(nullptr, QObject::tr("Load .SSP File"), "C:/", "SSP (*.ssp)");

    if (loadedFile.isEmpty())
        return;

    // Read the json data.
    QFile file(loadedFile);
    if (!file.open(QFile::ReadOnly)) {
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        return;
    }

    QJsonObject jsonObj = jsonDoc.object();
    int width = jsonObj["Width"].toInt();
    QJsonArray jsonFramesArray = jsonObj["Frames"].toArray();

    vector<int> frames;

    for (const QJsonValue &frameValue : jsonFramesArray) {
        QJsonArray frameArray = frameValue.toArray();

        for (const QJsonValue &frameValue : frameArray)
            frames.push_back(frameValue.toInt());
    }

    // Load the canvas with the given width and frames.
    emit loadCanvas(width, frames);
}
