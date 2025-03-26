#ifndef COLORPALETTE_H
#define COLORPALETTE_H

#include <QObject>
#include <QColorDialog>

using std::vector;

class ColorPalette : public QObject
{
    Q_OBJECT

private:
    vector<int> colors;

public:
    ColorPalette(QWidget *parent = nullptr);

    void addColor();

    void removeColor(int colorIndex);

    void savePalette();

    void loadPalette();

    void selectColor();
};

#endif // COLORPALETTE_H
