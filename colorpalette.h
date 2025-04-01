#ifndef COLORPALETTE_H
#define COLORPALETTE_H

#include <QObject>
#include <QColor>
#include <QVector>
#include <QMap>

class ColorPalette : public QObject {
    Q_OBJECT
    friend class Model;

private:
    /**
     * @brief colors - The vector that associates every stored color with a given ID.
     */
    QMap<int, QColor> colors;

    /**
     * @brief nextID - The ID of the next added Color.
     */
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
    /**
     * @brief ColorPalette - Default constructor for the QObject class.
     * @param parent
     */
    ColorPalette(QObject *parent = nullptr);

    /**
     * @brief queryColor - Opens a ColorDialog to let the user select a color, then sets the
     * currentColor to the selected color.
     */
    void queryColor();

    /**
     * @brief addColor - Appends a color into the palette.
     * @param newColor - The QColor to add to the palette vector.
     */
    void addColor(const QColor& newColor);

    /**
     * @brief savePalette - Saves the ColorPalette from a file.
     */
    void savePalette();

    /**
     * @brief loadPalette - Loads a ColorPalette from a file.
     */
    void loadPalette();

    /**
     * @brief getColor - Returns the color at a specified index.
     * @param index - The ID of the color to check.
     * @return The QColor at the given index.
     */
    QColor& getColor(int index);
};

#endif // COLORPALETTE_H
