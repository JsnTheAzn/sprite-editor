/**
 * @brief mainwindow - This is the Mainwindow class. This class aids out with the view.
 * @version 3/31/2025
 * @authors Evan Tucker, Max Donaldson, Ariadne Petroulakis, Jason Khong, Ian Weller
 * * checked by Ian Weller
 */

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

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

protected:
    /**
     * @brief mouseMoveEvent - Updates the canvas coordinates whenever
     * the mouse has moved while hovering over the canvas.
     * @param event - Mouse movement.
     */
    void mouseMoveEvent(QMouseEvent *event);

    /**
     * @brief mousePressEvent - Draws on the canvas at the given coordinates
     * if the mouse is pressed while on the canvas.
     * @param event - Mouse clicks.
     */
    void mousePressEvent(QMouseEvent *event);

    /**
     * @brief mouseReleaseEvent - Triggers version history update
     * when the mouse is released.
     * @param event - Mouse released.
     */
    void mouseReleaseEvent(QMouseEvent *event);

public:
    /**
     * @brief MainWindow - Creates a new main window, inheriting
     * from QWidget.
     * @param model - The Model class this works off of.
     * @param parent - The parent QWidget.
     */
    MainWindow(Model* model, QWidget *parent = nullptr);

    /**
     * @brief ~MainWindow - Destructor for the MainWindow class.
     */
    ~MainWindow();

signals:
    /**
     * @brief colorDeleted - Signals when a color has been deleted from
     * the color palette UI.
     * @param colorIndex - The index, from the ButtonGroup, of the color that
     * has been deleted.
     */
    void colorDeleted(int colorIndex);

    /**
     * @brief saveNewVersion - Signals to save the sprite passed in
     * as a new version.
     * @param current - A pointer for the most up-to-date
     * version of the sprite.
     */
    void saveNewVersion(Sprite *current);

    /**
     * @brief addNewColorButton - Adds a new button to the color palette.
     * @param color - The color to add, as a QColor.
     */
    void ctrlZPressed();

    /**
     * @brief ctrlYPressed - Signal when control + Y is pressed.
     */
    void ctrlYPressed();

public slots:
    /**
     * @brief addNewColorButton - Adds a new button to the color palette.
     * @param color - The color to add, as a QColor.
     */
    void addNewColorButton(QColor color, int id);

    /**
     * @brief selectedColor - Handles selection of a color button.
     * @param button - The selected button
     */
    void selectedColor(QAbstractButton *button);

    /**
     * @brief removeColorButton - Handles removal of a color button.
     */
    void removeColorButton();

    /**
     * @brief drawSpriteFrame - Draws a frame of a Sprite on the canvas.
     * @param frame - The specific frame to draw.
     */
    void drawSpriteFrame(int frame);

    /**
     * @brief updateSprite - Draws, on the camnvas in the view,
     *  a sprite update from the model.
     */
    void drawAnimationFrame(int frame);

     /**
      * @brief updateSprite - Draws, on the camnvas in the view,
     *  a sprite update from the model.
      */
    void updateSprite();

    /**
     * @brief clearColorPalette - Clears all buttons from the color palette.
     */
    void clearColorPalette();

    /**
     * @brief resizeButtonPressed - Updates the sprite to visually reflect the
     * resize.
     */
    void resizeButtonPressed();

    /**
     * @brief revertSprite - Reverts sprite to a different version from
     * its version history.
     * @param currentSprite - The reference to the sprite being altered.
     */
    void revertSprite(const Sprite& currentSprite);

    /**
     * @brief toggleToolButtons - Toggles the tool button.
     */
    void toggleToolButtons();

    /**
     * @brief updateFPS - Updates the FPS from the slider.
     * @param fps - The new FPS
     */
    void updateFPS(int newFps);

    /**
     * @brief goToNextFrame - Switches current frame to the next one
     */
    void goToNextFrame();

    /**
     * @brief goToPrevFrame - Switches current frame to the previous one
     */
    void goToPrevFrame();

    /**
     * @brief goToFrameIndex - Sets the canvas to the frame index specified.
     * @param index - The frame index to go to.
     */
    void goToFrameIndex(int index);

    /**
     * @brief deleteCurrentFrame - Removes the current frame.
     */
    void deleteCurrentFrame();

    /**
     * @brief addPrevFrame - Adds frame before current frame.
     */
    void addPrevFrame();

    /**
     * @brief addNextFrame - Adds frame after current frame.
     */
    void addNextFrame();

    /**
     * @brief trueToSizeToggled - checks if the true to size button has been toggled.
     * @param status - if it's checked or unchecked.
     */
    void trueToSizeToggled(int status);

private slots:
    /**
     * @brief selectVersion - Updates the index of the version combo box.
     */
    void selectVersion(int index);

    /**
     * @brief updateVersionComboBox - Updates the contents of the version combo box
     * to reflect version history.
     */
    void updateVersionComboBox();

    /**
     * @brief drawAnimationFrame
     */
    void drawAnimationFrame();

private:
    //Instance Variables: Pointers
    /**
     * @brief ui - Stores visual elements.
     */
    Ui::MainWindow *ui;

    /**
     * @brief colorPaletteButtonGroup - Stores ButtonGroup data for the ColorPalette.
     */
    QButtonGroup *colorPaletteButtonGroup;

    /**
     * @brief colorPalette - Stores ColorPalette data.
     */
    ColorPalette *colorPalette;

    /**
     * @brief sprite - Stores Sprite data.
     */
    Sprite *sprite;

    /**
     * @brief fileManager - Stores FileManager data.
     */
    FileManager *fileManager;

    /**
     * @brief model - Stores Model data.
     */
    Model *model;

    /**
     * @brief tool - Stores Tool data.
     */
    Tool *tool;

    /**
     * @brief image - Stores the image to draw on the Canvas.
     */
    QImage image;

    /**
     * @brief animationImage - the image on which to draw the animation preview. 
     */
    QImage animationImage;;
    
    /**
     * @brief graphic - the scene for graphics. 
     */
    QGraphicsScene *graphic;

    /**
     * @brief currentColorButton - The color button selected from the ColorPalette.
     */
    QAbstractButton *currentColorButton;

    /**
     * @brief fpsValidator -
     */
    QIntValidator* fpsValidator;
    
    /**
     * @brief animationTimer - a timer to trigger frame drawing based on fps.
     */
    QTimer* animationTimer;

    // Canvas Positions
    /**
     * @brief canvasX - The pixel coords relative to the Canvas.
     */
    int canvasX;

    /**
     * @brief canvasY - The pixel coords relative to the Canvas.
     */
    int canvasY;

    /**
     * @brief newPosition - Checks to see if the pixel to draw to is already drawn on by this
     * current tool usage.
     */
    bool newPosition;

    // Animation Vars
    /**
     * @brief fps - The frames per second the animation preview will display at.
     */
    int fps;
    
    /**
     * @brief fps - The frames the animation preview is currently displaying.
     */
    int animationFrame;

    /**
     * @brief isInCanvas - Boolean statement to declare whether or not an x-y position
     *  is within the bounds of the canvas.
     * @param x - The canvas x position to check.
     * @param y - The canvas y position to check.
     * @return True if the mouse is inside of the canvas widget.
     */
    bool isInCanvas(int x, int y);

    /**
     * @brief updateUndoRedoButtons - Handles enabling and disabling of the undo and redo buttons.
     */
    void updateUndoRedoButtons();

    /**
     * @brief keyPressEvent - Takes in a key press and determines if it's a
     * Control + Z or a Control + Y. Otherwise, calls the non-overrided keyPressEvent with event
     * @param Key which was pressed
     */
    void keyPressEvent(QKeyEvent *event);

#endif // MAINWINDOW_H
};
