#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setMouseTracking(true);
    fileManager = new FileManager();
    canvasX = 0;
    canvasY = 0;
    newPosition = true;
    tool = new Tool();
    sprite = new Sprite();

    // DELETE:::::: \/
    tool->setCurrentColor({100, 100, 100, 255});
    // /\

    // IMAGE SETUP:
    image = QImage(sprite->getWidth(), sprite->getWidth(), QImage::Format_RGBA32FPx4);
    graphic = new QGraphicsScene(this);
    graphic->addPixmap(QPixmap::fromImage(image));

    // CONNECTIONS:
    connect(ui->saveButton, &QPushButton::clicked, fileManager, &FileManager::saveButtonClicked);
    connect(ui->loadButton, &QPushButton::clicked, fileManager, &FileManager::loadButtonClicked);
    connect(sprite, &Sprite::drawFrame, this, &MainWindow::drawSpriteFrame);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete fileManager;
    delete tool;
    delete graphic;
    delete sprite;
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

        // qDebug() << "mouse at " << mouseX << ", " << mouseY;

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
            sprite->getPixel(row, col, pixelColors);
            image.setPixel(row, col,
                           qRgba(pixelColors[0], pixelColors[1], pixelColors[2], pixelColors[3]));
        }
    }
    graphic->addPixmap(QPixmap::fromImage(image));
    ui->canvas->setPixmap(QPixmap::fromImage(image));
    sprite->setCurrentFrame(oldFrame);
}
