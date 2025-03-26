#ifndef COLORPALETTE_H
#define COLORPALETTE_H

#include <QObject>

using std::vector;

class ColorPalette : public QObject
{
    Q_OBJECT

private:
    vector<int> colors;

public:
    ColorPalette();

    void addColor();

    void removeColor(int colorIndex);

    void savePalette();

    void loadPalette();
};

#endif // COLORPALETTE_H
