#include "colorpalette.h"
#include <QColorDialog>
#include <QFile>
#include <QTextStream>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

ColorPalette::ColorPalette(QObject *parent) : QObject{parent} {
}

void ColorPalette::addColor() {
    if (colors.size() <= 8) {
        QColor selectedColor = QColorDialog::getColor(Qt::white, nullptr, "Select Color", QColorDialog::ShowAlphaChannel);
        if (selectedColor.isValid()) {
            colors.append(selectedColor);
            emit addNewColorButtonSignal(selectedColor);
        }
    }
}

void ColorPalette::removeColor(QColor color) {
    int index = colors.indexOf(color); // Find the index of the color
    if (index != -1) {
        colors.removeAt(index);
        emit removeColorSignal(color); // Notify UI to remove the button
    }
}

void ColorPalette::savePalette() {
    QString savedFile = QFileDialog::getSaveFileName(nullptr, QObject::tr("Save Color Palette"), "C:/", "JSON (*.json)");

    if (savedFile.isEmpty()) {
        return;
    }

    if (!savedFile.endsWith(".json", Qt::CaseInsensitive)) {
        savedFile += ".json";
    }

    QJsonObject jsonData;

    // Store colors
    QJsonArray colorsArray;
    for (const QColor &color : colors) {
        QJsonObject colorObject;
        colorObject["r"] = color.red();
        colorObject["g"] = color.green();
        colorObject["b"] = color.blue();
        colorObject["a"] = color.alpha();
        colorsArray.append(colorObject);
    }
    jsonData["colors"] = colorsArray;

    QFile file(savedFile);
    if (file.open(QFile::WriteOnly)) {
        file.write(QJsonDocument(jsonData).toJson(QJsonDocument::Indented));
        file.close();
    }
}

void ColorPalette::loadPalette() {
    QString fileName = QFileDialog::getOpenFileName(nullptr, QObject::tr("Load Color Palette"), "C:/", "JSON (*.json)");

    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull() || !doc.isObject()) {
        return;
    }

    QJsonObject rootObj = doc.object();
    QJsonArray colorsArray = rootObj["colors"].toArray();

    emit clearColorPalette();
    colors.clear();

    for (const QJsonValue &value : colorsArray) {
        QJsonObject colorObj = value.toObject();
        int r = colorObj["r"].toInt();
        int g = colorObj["g"].toInt();
        int b = colorObj["b"].toInt();
        int a = colorObj["a"].toInt(255);

        QColor color(r, g, b, a);
        colors.append(color);
        emit addNewColorButtonSignal(color);
    }

    qDebug() << "Loaded" << colors.size() << "colors from" << fileName;
}


QVector<QColor> ColorPalette::getColors() const {
    return colors;
}
