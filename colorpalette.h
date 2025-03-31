#ifndef COLORPALETTE_H
#define COLORPALETTE_H

#include <QObject>
#include <QColor>
#include <QVector>
#include <QMap>

class ColorPalette : public QObject
{
    Q_OBJECT

private:
    QMap<int, QColor> colors;
    int nextID;

signals:
    void addNewColorButtonSignal(QColor color, int id);
    void removeColorSignal(QColor color);
    void clearColorPalette();

public slots:
    /**
     * @brief removeColor - Removes color from the color palette
     * @param colorIndex - Based on the color index is what we want to remove
     */
    void removeColor(int id);

public:
    ColorPalette(QObject *parent = nullptr);

    void queryColor();

    void addColor(const QColor& newColor);

    /**
     * @brief savePalette - Saves the Color Palette
     */
    void savePalette();

    /**
     * @brief loadPalette - Loads a Color Palette
     */
    void loadPalette();

    QColor& getColor(int index);

};

#endif // COLORPALETTE_H
