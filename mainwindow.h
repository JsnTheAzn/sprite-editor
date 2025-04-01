#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QGraphicsScene>
#include "colorpalette.h"
#include "filemanager.h"
#include "model.h"
#include "sprite.h"
#include "tool.h"
#include <QPushButton>
#include <QListWidget>
#include "colorpalette.h"
#include <QColor>
#include <QGridLayout>
#include <QButtonGroup>
#include "versionhistory.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

protected:
    /**
     * @brief mouseMoveEvent - updates the canvas coordinates whenever
     * the mouse has moved while hovering over the canvas.
     * @param event - mouse movement.
     */
    void mouseMoveEvent(QMouseEvent *event);

    /**
     * @brief mousePressEvent - draws on the canvas at the given coordinates
     * if the mouse is pressed while on the canvas.
     * @param event - mouse clicks.
     */
    void mousePressEvent(QMouseEvent *event);

    /**
     * @brief mouseReleaseEvent - triggers version history update
     * when the mouse is released.
     * @param event - mouse released.
     */
    void mouseReleaseEvent(QMouseEvent *event);

public:
    /**
     * @brief MainWindow - creates a new main window, inheriting
     * from QWidget.
     * @param model - the Model class this works off of.
     * @param parent - the parent QWidget.
     */
    MainWindow(Model* model, QWidget *parent = nullptr);

    /**
     * @brief destructor for the MainWindow class.
     */
    ~MainWindow();


signals:
    /**
     * @brief colorDeleted - signals when a color has been deleted from
     * the color palette UI.
     * @param colorIndex - the index, from the ButtonGroup, of the color that
     * has been deleted.
     */
    void colorDeleted(int colorIndex);

    /**
     * @brief saveNewVersion - signals to save the sprite passed in
     * as a new version.
     * @param current - a pointer for the most up-to-date
     * version of the sprite.
     */
    void saveNewVersion(Sprite *current);

public slots:
    /**
     * @brief addNewColorButton - adds a new button to the color palette.
     * @param color - the color to add, as a QColor.
     */

    void addNewColorButton(QColor color, int id);

    /**
     * @brief selectedColor - handles selection of a color button.
     * @param button - the selected button
     */
    void selectedColor(QAbstractButton *button);

    /**
     * @brief removeColorButton - handles removal of a color button.
     */
    void removeColorButton();

    /**
     * @brief drawSpriteFrame - draws a frame of a Sprite on the canvas.
     * @param frame - the specific frame to draw.
     */
    void drawSpriteFrame(int frame);

    /**
     * @brief updateSprite - draws, on the camnvas in the view,
     *  a sprite update from the model
     */
    void updateSprite();

    /**
     * @brief clearColorPalette - clears all buttons from the color palette.
     */
    void clearColorPalette();

    /**
     * @brief resizeButtonPressed - updates the sprite to visually reflect the
     * resize.
     */
    void resizeButtonPressed();

    /**
     * @brief revertSprite - reverts sprite to a different version from
     * its version history.
     */
    void revertSprite(const Sprite& currentSprite);

    /**
     * @brief toggleToolButtons - toggles the tool button
     */
    void toggleToolButtons();

    /**
     * @brief updateFPS - updates the FPS from the slider.
     * @param fps - the new FPS
     */
    void updateFPS(int newFps);

    /**
     * @brief goToNextFrame - switches current frame to the next one
     */
    void goToNextFrame();

    /**
     * @brief goToPrevFrame - switches current frame to the previous one
     */
    void goToPrevFrame();

    /**
     * @brief deleteCurrentFrame - removes the current frame
     */
    void deleteCurrentFrame();

    /**
     * @brief addPrevFrame - adds frame before current frame
     */
    void addPrevFrame();

    /**
     * @brief addNextFrame - adds frame after current frame
     */
    void addNextFrame();

private slots:
    /**
     * @brief selectVersion - updates the index of the version combo box.
     */
    void selectVersion(int index);

    /**
     * @brief updateVersionComboBox - updates the contents of the version combo box
     * to reflect version history.
     */
    void updateVersionComboBox();

private:
    //Instance Variables: Pointers
    QButtonGroup *colorPaletteButtonGroup;
    Ui::MainWindow *ui;
    ColorPalette *colorPalette;
    Sprite *sprite;
    FileManager *fileManager;
    Model *model;
    Tool *tool;
    QImage image;
    QGraphicsScene *graphic;
    QAbstractButton *currentColorButton;
    QIntValidator* fpsValidator;

    // Canvas Positions
    int canvasX;
    int canvasY;
    bool newPosition;

    //Other instace vars
    int fps;

    /**
     * @brief isInCanvas - Boolean statement to declare whether or not an x-y position
     *  is within the bounds of the canvas.
     * @param x - the x position to check.
     * @param y - the y position to check.
     * @return True if the mouse is inside of the canvas widget.
     */
    bool isInCanvas(int x, int y);

    /**
     * @brief updateUndoRedoButtons - handles enabling and disabling of the undo and redo buttons.
     */
    void updateUndoRedoButtons();

#endif // MAINWINDOW_H
};
