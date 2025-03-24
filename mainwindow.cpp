#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

/*
 * Sources used:
 * https://doc.qt.io/qt-6/qfile.html
 * https://doc.qt.io/qt-6/qfiledialog.html
 * https://stackoverflow.com/questions/72134878/qt-creator-how-can-i-store-save-selected-file-to-project-directory
 * https://doc.qt.io/qt-6/json.html
 * https://doc.qt.io/qt-6/qstring.html
 * https://stackoverflow.com/questions/24936564/set-qfile-save-directory
 * https://forum.qt.io/topic/124003/how-to-write-data-into-json/2
 *
 * For deserializing:
 * https://stackoverflow.com/questions/15893040/how-to-create-read-write-json-files-in-qt5
*/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    pixelData = QVector<int>();

    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::saveButtonClicked);
    connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::loadButtonClicked);
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

    //This will represent the data of the saved canvas width.
    QJsonArray jsonData;
    jsonData.append(canvasWidth);

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
    QString loadedFile = QFileDialog::getOpenFileName(this, QObject::tr("Load .SSP File"), "C:/", "SSP (*.ssp)");

    //Read the json data.
    QFile file(loadedFile);
    if(file.open(QFile::ReadOnly)) {
        QTextStream stream(&file);
        QString jsonString = file.readAll();
        file.close();

        QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8());
        QJsonArray jsonArray = jsonDocument.array();

    //The sent json data to be deserialized.
    //QJsonArray jsonData;
    //jsonData >> jsonString;

    //Canvas width is the first data sent, so read it first.
    canvasWidth = jsonArray.first().toInt();

    //TODO: read the json data as an increment in order to place pixels on canvas.

        //Borrowed from assignment 3, utilize to set RGBA values & assist in determining pixel location.
        for(int x = 0; x < canvasWidth; x++) {
            for(int y = 0; x < canvasWidth; y++) {
                if (x < 0 || x >= canvasWidth || y < 0 || y >= canvasWidth) {
                    int offset = (y * canvasWidth + x) * 4;
                    pixelData[offset + 0] = r; // red
                    pixelData[offset + 1] = g; // green
                    pixelData[offset + 2] = b; // blue
                    pixelData[offset + 3] = a; // alpha (keep at 255 for no transparency)
                }
            }
        }
    }
}
