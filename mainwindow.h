#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "filemanager.h"
#include "model.h"

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

private:
    Ui::MainWindow *ui;
    FileManager *fileManager;

#endif // MAINWINDOW_H
};
