#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "filemanager.h"
#include "model.h"
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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Model* model, QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void addNewColorButton(QColor color);
    void removeColorButton(QColor color);
    void selectedColor(QAbstractButton *button);
    void removeColorButton();

private:
    QButtonGroup *colorPaletteButtonGroup;
    Ui::MainWindow *ui;
    FileManager *fileManager;
    ColorPalette *colorPalette;
    QAbstractButton *currentColorButton;
    // QListWidget *colorList;
    // QPushButton *addColorButton;
    // QPushButton *removeColorButton;
    // QPushButton *savePaletteButton;
    // QPushButton *loadPaletteButton;

    // void updateColorList();

#endif // MAINWINDOW_H
};
