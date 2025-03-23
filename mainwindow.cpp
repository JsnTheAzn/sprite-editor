#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>

/*
 * Sources used:
 * https://doc.qt.io/qt-6/qfile.html
 * https://doc.qt.io/qt-6/qfiledialog.html
 * https://stackoverflow.com/questions/72134878/qt-creator-how-can-i-store-save-selected-file-to-project-directory
 * https://doc.qt.io/qt-6/json.html
 * https://doc.qt.io/qt-6/qstring.html
 * https://stackoverflow.com/questions/24936564/set-qfile-save-directory
 * https://forum.qt.io/topic/124003/how-to-write-data-into-json/2
*/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    pixelData = QVector<int>();

    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::saveButtonClicked);
    connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::saveButtonClicked);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::saveButtonClicked() {
    QString savedFile = QFileDialog::getSaveFileName(this, QObject::tr("Save .SSP File"), "C:/", "SSP (*.ssp)");

    //Add .ssp extension if file isn't saved with it manually.
    if(!savedFile.endsWith((".ssp"), Qt::CaseSensitive)) {
        savedFile += ".ssp";
    }

    //This will represent the data of the saved canvas width & height.
    QJsonArray jsonData;
    jsonData.append(canvasWidth);
    jsonData.append(canvasHeight);

    //This will represent the pixel data of the r,g,b,a values.
    QJsonArray jsonArray;
    for(int i = 0; i < pixelData.length(); i++) {
        jsonArray.append(pixelData[i]);
    }
    jsonData.append(jsonArray);

    //Write the data in json to the file.
    QFile file(savedFile);
    if(file.open(QFile::WriteOnly)) {
        QTextStream stream(&file);
        stream << QJsonDocument(jsonData).toJson(QJsonDocument::Indented);
        file.close();
    }
}

void MainWindow::loadButtonClicked() {

}
