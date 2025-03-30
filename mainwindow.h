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

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

public:
    MainWindow(Model* model, QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void colorDeleted(int colorIndex);

public slots:
    void addNewColorButton(QColor color);
    void selectedColor(QAbstractButton *button);
    void removeColorButton();
    void drawSpriteFrame(int frame);
    void updateSprite();
    void clearColorPalette();

private:
    QButtonGroup *colorPaletteButtonGroup;
    Ui::MainWindow *ui;
    ColorPalette *colorPalette;
    Sprite *sprite;
    FileManager *fileManager;
    Model *model;
    Tool *tool;
    QImage image;
    QGraphicsScene *graphic;
    QPushButton* undoButton;
    QPushButton* redoButton;
    VersionHistory versionHistory;


    int canvasX;
    int canvasY;
    bool newPosition;

    /**
     * @brief checkMouseBounds - Boolean statement to declare whether or not the current position
     * of the mouse is within the bounds of the canvas.
     * @return True if the mouse is inside of the canvas widget.
     */
    bool checkMouseBounds();

    QAbstractButton *currentColorButton;
#endif // MAINWINDOW_H
};
