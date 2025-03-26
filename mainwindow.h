#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "filemanager.h"
#include <QPushButton>
#include <QListWidget>
#include "colorpalette.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    FileManager *fileManager;

    // ColorPalette *colorPalette;
    // QListWidget *colorList;
    // QPushButton *addColorButton;
    // QPushButton *removeColorButton;
    // QPushButton *savePaletteButton;
    // QPushButton *loadPaletteButton;

    // void updateColorList();

#endif // MAINWINDOW_H
};
