#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "colorpalette.h"
#include "filemanager.h"
#include "model.h"
#include "sprite.h"
#include "tool.h"

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
    ColorPalette *colorPalette;
    FileManager *fileManager;
    Model *model;
    Sprite *sprite;
    Tool *tool;

#endif // MAINWINDOW_H
};
