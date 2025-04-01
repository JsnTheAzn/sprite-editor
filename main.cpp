/**
 * @version 3/31/2025
 * @authors Evan Tucker, Max Donaldson, Ariadne Petroulakis, Jason Khong, Ian Weller
 * * checked by Jason Khong
 */

#include "mainwindow.h"
#include "model.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Model m;
    MainWindow w(&m);
    w.show();
    return a.exec();
}
