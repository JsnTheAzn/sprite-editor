#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

    //Width of the canvas, will always be X by X size.
    int width;
    //Initializes a vector for storing the r,g,b,a pixel data.
    QVector<int> frames;

public slots:
    void saveButtonClicked();

    void loadButtonClicked();

};
#endif // MAINWINDOW_H
