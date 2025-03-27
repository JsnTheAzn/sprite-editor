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
    newPosition = false;
    tool = new Tool();

    connect(ui->saveButton, &QPushButton::clicked, fileManager, &FileManager::saveButtonClicked);
    connect(ui->loadButton, &QPushButton::clicked, fileManager, &FileManager::loadButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete fileManager;
    delete tool;
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
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (newPosition) {
        tool->useTool(sprite, canvasX, canvasY);
        newPosition = false;
    }

}
