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
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    fileManager = new FileManager();
    colorPalette = new ColorPalette();
    colorPaletteButtonGroup = new QButtonGroup(this);
    currentColorButton = nullptr;

    //view to model
    connect(ui->saveButton, &QPushButton::clicked, fileManager, &FileManager::saveButtonClicked);
    connect(ui->loadButton, &QPushButton::clicked, fileManager, &FileManager::loadButtonClicked);
    connect(ui->savePaletteButton, &QPushButton::clicked, colorPalette, &ColorPalette::savePalette);
    connect(ui->addColor, &QPushButton::clicked, colorPalette, &ColorPalette::addColor);

    //model to view
    connect(colorPalette, &ColorPalette::addNewColorButtonSignal, this, &MainWindow::addNewColorButton);

    //view to... erm... view?
    connect(colorPaletteButtonGroup, &QButtonGroup::buttonClicked, this, &MainWindow::selectedColor);
    connect(ui->removeColor, &QPushButton::clicked, this, &MainWindow::removeColorButton);
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
    QPushButton* newButton = new QPushButton("", this);
    QString styleSheet = QString("background-color: rgb(%1, %2, %3);"
                                 "border: 1px solid black;")
        .arg(color.red())
        .arg(color.green())
        .arg(color.blue());
    newButton->setStyleSheet(styleSheet);
    colorPaletteButtonGroup->addButton(newButton, -1);
    layout->addWidget(newButton);
    qDebug() << colorPaletteButtonGroup->buttons().count();
}

void MainWindow::selectedColor(QAbstractButton *button) {
    //TODO: I still have to make the REGEX to actually gnab the color (stored as the background image)
    QRegularExpression regexSolidWhite("solid white");
    QRegularExpression regexSolidBlack("solid black");
    if (currentColorButton != nullptr) {
        QString previousStyle = currentColorButton->styleSheet();
        previousStyle.replace(regexSolidWhite, "solid black");
        currentColorButton->setStyleSheet(previousStyle);
        qDebug() << "test: " << previousStyle;
    }
    currentColorButton = button;
    QString currentStyle = button->styleSheet();
    currentStyle.replace(regexSolidBlack, "solid white");
    currentColorButton->setStyleSheet(currentStyle);
}

void MainWindow::removeColorButton() {
    //TODO: go back into the model and remove the specific color from the vector.
    colorPaletteButtonGroup->removeButton(currentColorButton);
    delete currentColorButton;
}

