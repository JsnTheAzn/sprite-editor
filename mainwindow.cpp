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
    connect(ui->savePaletteButton, &QPushButton::clicked, colorPalette, &ColorPalette::savePalette);
    connect(ui->addColor, &QPushButton::clicked, colorPalette, &ColorPalette::addColor);
    connect(colorPaletteButtonGroup, &QButtonGroup::buttonClicked, colorPalette, &ColorPalette::selectColor);

    //model to view
    connect(colorPalette, &ColorPalette::addNewColorButtonSignal, this, &MainWindow::addNewColorButton);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete colorPalette;
    delete colorPaletteButtonGroup;
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


