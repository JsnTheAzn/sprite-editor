#ifndef COLORPALETTE_H
#define COLORPALETTE_H

#include <QObject>
#include <QColor>
#include <QVector>

class ColorPalette : public QObject {
    Q_OBJECT
    friend class Model;

private:
    QVector<QColor> colors; // Store colors as QColor objects

signals:
    void addNewColorButtonSignal(QColor color);
    void removeColorSignal(QColor color);
    void clearColorPalette();

public slots:
    /**
     * @brief removeColor - Removes color from the color palette
     * @param colorIndex - Based on the color index is what we want to remove
     */
    void removeColor(QColor color);

public:
    ColorPalette(QObject *parent = nullptr);

    /**
     * @brief addColor - Adds a color to the color palette
     */
    void addColor();

    /**
     * @brief savePalette - Saves the Color Palette
     */
    void savePalette();

    /**
     * @brief loadPalette - Loads a Color Palette
     */
    void loadPalette();


    /**
     * @brief getColors - Gets the colors of the palette
     * @return The vector of colors stored
     */
    QVector<QColor> getColors() const; // Function to return the stored colors
};

#endif // COLORPALETTE_H
