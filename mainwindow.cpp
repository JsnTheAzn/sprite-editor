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

MainWindow::MainWindow(Model* model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setMouseTracking(true);
    canvasX = 0;
    canvasY = 0;
    newPosition = true;
    tool = model->getToolPtr();
    sprite = model->getSpritePtr();

    // DELETE:::::: \/
    tool->setCurrentColor({100, 100, 100, 255});
    // /\

    // IMAGE SETUP:
    image = QImage(sprite->getWidth(), sprite->getWidth(), QImage::Format_RGBA32FPx4);
    graphic = new QGraphicsScene(this);
    graphic->addPixmap(QPixmap::fromImage(image));    
    
    //COLOR PALETTE SETUP
    colorPalette = new ColorPalette();
    colorPaletteButtonGroup = new QButtonGroup(this);
    currentColorButton = nullptr;
      
    // CONNECTIONS:
    //view to model
    connect(ui->saveButton, &QPushButton::clicked, model, &Model::saveSprite);
    connect(ui->loadButton, &QPushButton::clicked, model, &Model::loadSprite);
    connect(ui->addColor, &QPushButton::clicked, colorPalette, &ColorPalette::addColor);
    connect(this, &MainWindow::colorDeleted, colorPalette, &ColorPalette::removeColor);

    //model to view
    connect(colorPalette, &ColorPalette::addNewColorButtonSignal, this, &MainWindow::addNewColorButton);
    connect(ui->savePaletteButton, &QPushButton::clicked, colorPalette, &ColorPalette::savePalette);
    connect(ui->loadPaletteButton, &QPushButton::clicked, colorPalette, &ColorPalette::loadPalette);
    connect(colorPalette, &ColorPalette::removeColorSignal, this, &MainWindow::removeColorButton);
    connect(colorPalette, &ColorPalette::clearColorPalette, this, &MainWindow::clearColorPalette);
    
      //view to... erm... view?
    connect(colorPaletteButtonGroup, &QButtonGroup::buttonClicked, this, &MainWindow::selectedColor);
    connect(ui->removeColor, &QPushButton::clicked, this, &MainWindow::removeColorButton);
    connect(sprite, &Sprite::drawFrame, this, &MainWindow::drawSpriteFrame);
}

MainWindow::~MainWindow()
{
    tool = nullptr;
    sprite = nullptr;
    delete ui;
    delete graphic;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    double mouseX = event->position().x();
    double mouseY = event->position().y();

    int oldCanvasX = canvasX;
    int oldCanvasY = canvasY;
    QLabel* canvas = ui->canvas;

    // If the mouse is moved into bounds -> Update canvas coordinates
    if (mouseX > canvas->pos().x() && mouseX < canvas->width() + canvas->pos().x() &&
        mouseY > canvas->pos().y() && mouseY < canvas->height() + canvas->pos().y()) {
        // Normalize
        mouseX = mouseX - canvas->pos().x();
        mouseY = mouseY - canvas->pos().y();

        // Project into Canvas Coords
        canvasX = (int)(mouseX / (canvas->width() / sprite->getWidth()));
        canvasY = (int)(mouseY / (canvas->width() / sprite->getWidth()));

        if (canvasX != oldCanvasX || canvasY != oldCanvasY)
            newPosition = true;
    }

    if (newPosition) {
        tool->useTool(sprite, canvasX, canvasY);
        newPosition = false;
        qDebug() << "mouse at cpos " << canvasX << ", " << canvasY;
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    double mouseX = event->position().x();
    double mouseY = event->position().y();
    QLabel* canvas = ui->canvas;

    if (mouseX > canvas->pos().x() && mouseX < canvas->width() + canvas->pos().x() &&
        mouseY > canvas->pos().y() && mouseY < canvas->height() + canvas->pos().y()) {

        mouseX = mouseX - canvas->pos().x();
        mouseY = mouseY - canvas->pos().y();

        canvasX = (int)(mouseX / (canvas->width() / sprite->getWidth()));
        canvasY = (int)(mouseY / (canvas->width() / sprite->getWidth()));

        tool->useTool(sprite, canvasX, canvasY);
        newPosition = true;
        qDebug() << "MOUSE PRESSED: mouse at cpos " << canvasX << ", " << canvasY;
    }
}

void MainWindow::drawSpriteFrame(int frame) {
    qDebug() << "DRAW SPRITE";

    int width = sprite->getWidth();
    int oldFrame = sprite->getFrame();
    sprite->setCurrentFrame(frame);

    int pixelColors[4];

    for (int row = 0; row < width; row++) {
        for (int col = 0; col < width; col++) {

            sprite->getPixel(row, col, pixelColors); //get pixel location & colors
            image.setPixel(row, col,
                           qRgba(pixelColors[0], pixelColors[1], pixelColors[2], pixelColors[3]));
        }
    }
    graphic->addPixmap(QPixmap::fromImage(image));
    ui->canvas->setPixmap(QPixmap::fromImage(image).scaled(
        ui->canvas->size(),
        Qt::KeepAspectRatio,
        Qt::FastTransformation));
    sprite->setCurrentFrame(oldFrame);
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
    if (!button) return;

    //TODO: I still have to make the REGEX to actually gnab the color (stored as the background image)
    QRegularExpression regexSolidWhite("solid white");
    QRegularExpression regexSolidBlack("solid black");
    if (currentColorButton != nullptr) {
        qDebug() << "if you see this before it crashed, this is it";
        QString previousStyle = currentColorButton->styleSheet();
        previousStyle.replace(regexSolidWhite, "solid black");
        currentColorButton->setStyleSheet(previousStyle);
    }
    currentColorButton = button;
    QString currentStyle = button->styleSheet();
    currentStyle.replace(regexSolidBlack, "solid white");
    currentColorButton->setStyleSheet(currentStyle);

    //set color:
    QColor newColor = currentColorButton->palette().color(QPalette::Button);
    tool->setCurrentColor({newColor.red(), newColor.green(), newColor.blue(), 255});
}

void MainWindow::removeColorButton() {
    //TODO: go back into the model and remove the specific color from the vector.
    if (currentColorButton) {
        colorPaletteButtonGroup->removeButton(currentColorButton);
        delete currentColorButton;
        currentColorButton = nullptr;
    }
    //TODO: THIS IS WRONGGGGGGGGGGGG!
    emit colorDeleted(0);
}

void MainWindow::clearColorPalette() {
    // Remove all buttons from the palette UI
    for (QAbstractButton* button : colorPaletteButtonGroup->buttons()) {
        colorPaletteButtonGroup->removeButton(button);
        delete button;
    }
}


