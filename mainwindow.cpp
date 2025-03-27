#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QWidget>
#include <QColor>
#include <QPushButton>
#include <QGridLayout>
#include <QButtonGroup>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    fileManager = new FileManager();
    colorPalette = new ColorPalette();
    colorPaletteButtonGroup = new QButtonGroup(this);

    //view to model
    connect(ui->saveButton, &QPushButton::clicked, fileManager, &FileManager::saveButtonClicked);
    connect(ui->loadButton, &QPushButton::clicked, fileManager, &FileManager::loadButtonClicked);


    connect(ui->addColor, &QPushButton::clicked, colorPalette, &ColorPalette::addColor);
    connect(ui->removeColor, &QPushButton::clicked, colorPalette, &ColorPalette::removeColor);

    //model to view
    connect(colorPalette, &ColorPalette::addNewColorButtonSignal, this, &MainWindow::addNewColorButton);
    connect(ui->savePaletteButton, &QPushButton::clicked, colorPalette, &ColorPalette::savePalette);
    connect(ui->loadPaletteButton, &QPushButton::clicked, colorPalette, &ColorPalette::loadPalette);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addNewColorButton(QColor color)
{
    QGroupBox *groupBox = findChild<QGroupBox*>("leftPanel");
    if (!groupBox) {
        return;
    }
    QWidget *innerWidget = groupBox->findChild<QWidget*>("paletteWidget");
    if (!innerWidget) {
        return;
    }
    QGridLayout *layout = innerWidget->findChild<QGridLayout*>("gridLayout");
    if (!layout) {
        return;
    }
    QPushButton *newButton = new QPushButton("", this);
    QString styleSheet = QString("background-color: rgb(%1, %2, %3);")
        .arg(color.red())
        .arg(color.green())
        .arg(color.blue());
    newButton->setStyleSheet(styleSheet);
    colorPaletteButtonGroup->addButton(newButton, -1);
    layout->addWidget(newButton);
    qDebug() << colorPaletteButtonGroup->buttons().count();
}

void MainWindow::removeColorButton(QColor color) {
    QGroupBox *groupBox = findChild<QGroupBox*>("leftPanel");
    if (!groupBox) {
        return;
    }
    QWidget *innerWidget = groupBox->findChild<QWidget*>("paletteWidget");
    if (!innerWidget) {
        return;
    }
    QGridLayout *layout = innerWidget->findChild<QGridLayout*>("gridLayout");
    if (!layout) {
        return;
    }
    QPushButton *newButton = new QPushButton("", this);
    QString styleSheet = QString("background-color: rgb(%1, %2, %3);")
     .arg(color.red())
     .arg(color.green())
     .arg(color.blue());
    newButton->setStyleSheet(styleSheet);
    colorPaletteButtonGroup->removeButton(newButton);
    layout->addWidget(newButton);
    qDebug() << colorPaletteButtonGroup->buttons().count();
}


