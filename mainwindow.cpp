#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

MainWindow::MainWindow(Model* model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    fileManager = new FileManager();

    connect(ui->saveButton, &QPushButton::clicked, fileManager, &FileManager::saveButtonClicked);
    connect(ui->loadButton, &QPushButton::clicked, fileManager, &FileManager::loadButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete fileManager;
}
