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

ColorPalette::ColorPalette(QObject *parent) : QObject{parent}, nextID(0) {
}

void ColorPalette::queryColor() {
    QColor selectedColor = QColorDialog::getColor(Qt::white, nullptr, "Select Color", QColorDialog::ShowAlphaChannel);
    if (selectedColor.isValid()) {
        addColor(selectedColor);
    }
}

void ColorPalette::addColor(const QColor& newColor) {
    if (colors.size() <= 8) {
        colors.insert(nextID, newColor);
        emit addNewColorButtonSignal(newColor, nextID);
        nextID++;
    }
}

void ColorPalette::removeColor(int id) {
    colors.remove(id);
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
    for (const QColor &color : std::as_const(colors)) {
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

    for (const QJsonValue &value : std::as_const(colorsArray)) {
        QJsonObject colorObj = value.toObject();
        int r = colorObj["r"].toInt();
        int g = colorObj["g"].toInt();
        int b = colorObj["b"].toInt();
        int a = colorObj["a"].toInt(255);

        QColor color(r, g, b, a);
        addColor(color);
    }

    qDebug() << "Loaded" << colors.size() << "colors from" << fileName;
}

QColor& ColorPalette::getColor(int index) {
    return colors[index];
}

