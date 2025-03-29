#include "colorpalette.h"
#include <QColorDialog>
#include <QFile>
#include <QTextStream>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>

ColorPalette::ColorPalette(QObject *parent) : QObject{parent} {
}

void ColorPalette::addColor() {
    if (colors.size() <= 8) {
        QColor selectedColor = QColorDialog::getColor(Qt::white, nullptr, "Select Color");
        if (selectedColor.isValid()) {
            colors.append(selectedColor);
            emit addNewColorButtonSignal(selectedColor);
        }
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
    qDebug() << "test";
}

QVector<QColor> ColorPalette::getColors() const {
    return colors;
}
