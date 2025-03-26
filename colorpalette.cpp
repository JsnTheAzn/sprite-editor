#include "colorpalette.h"
#include <QColorDialog>
#include <QFile>
#include <QTextStream>

ColorPalette::ColorPalette(QObject *parent) : QObject{parent} {}

void ColorPalette::addColor() {
    QColor selectedColor = QColorDialog::getColor(Qt::white, nullptr, "Select Color");
    if (selectedColor.isValid()) {
        colors.append(selectedColor);
    }
}

void ColorPalette::removeColor(int colorIndex) {
    if (colorIndex >= 0 && colorIndex < colors.size()) {
        colors.removeAt(colorIndex);
    }
}

void ColorPalette::savePalette() {

}

void ColorPalette::loadPalette() {

}

void ColorPalette::selectColor() {
}

QVector<QColor> ColorPalette::getColors() const {
    return colors;
}
