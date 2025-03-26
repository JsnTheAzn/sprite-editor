#include "colorpalette.h"
#include <QColorDialog>

ColorPalette::ColorPalette(QWidget *parent) : QObject{parent} {
    colors = vector<int>();

}

void ColorPalette::addColor() {
    QColorDialog colorDialog = QColorDialog(parent);
    colorDialog.getColor();
}

void ColorPalette::removeColor(int colorIndex)
{

}

void ColorPalette::savePalette() {}

void ColorPalette::loadPalette() {}

void ColorPalette::selectColor() {}
