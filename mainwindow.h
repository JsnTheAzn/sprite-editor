#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include "filemanager.h"

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

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    FileManager *fileManager;

    /**
     * @brief checkMouseBounds - Boolean statement to declare whether or not the current position
     * of the mouse is within the bounds of the canvas.
     * @return True if the mouse is inside of the canvas widget.
     */
    bool checkMouseBounds();

#endif // MAINWINDOW_H
};
