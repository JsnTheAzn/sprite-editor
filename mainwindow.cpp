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
    fileManager = new FileManager();
    this->model = model;
    canvasX = 0;
    canvasY = 0;
    newPosition = true;
    tool = model->getToolPtr();
    sprite = model->getSpritePtr();

    fileManager->setSprite(sprite);
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
    connect(ui->saveButton, &QPushButton::clicked, fileManager, &FileManager::saveButtonClicked);
    connect(ui->loadButton, &QPushButton::clicked, fileManager, &FileManager::loadButtonClicked);
    connect(ui->addColor, &QPushButton::clicked, colorPalette, &ColorPalette::addColor);
    connect(ui->eraseButton, &QPushButton::clicked, model, [model]{model->setTool(Tool::Tools::Eraser);});
    connect(ui->paintButton, &QPushButton::clicked, model, [model]{model->setTool(Tool::Tools::Pen);});
    connect(ui->fillButton, &QPushButton::clicked, model, [model]{model->setTool(Tool::Tools::PaintBucket);});
    connect(this, &MainWindow::colorDeleted, colorPalette, &ColorPalette::removeColor);

    //model to view
    connect(colorPalette, &ColorPalette::addNewColorButtonSignal, this, &MainWindow::addNewColorButton);
    connect(ui->savePaletteButton, &QPushButton::clicked, colorPalette, &ColorPalette::savePalette);
    connect(ui->loadPaletteButton, &QPushButton::clicked, colorPalette, &ColorPalette::loadPalette);
    connect(model, &Model::updateSprite, this, &MainWindow::updateSprite);
    connect(colorPalette, &ColorPalette::removeColorSignal, this, &MainWindow::removeColorButton);
    connect(colorPalette, &ColorPalette::clearColorPalette, this, &MainWindow::clearColorPalette);
    
    //view to... erm... view?
    connect(colorPaletteButtonGroup, &QButtonGroup::buttonClicked, this, &MainWindow::selectedColor);
    connect(ui->removeColor, &QPushButton::clicked, this, &MainWindow::removeColorButton);

    // model to model
    connect(sprite, &Sprite::drawFrame, this, &MainWindow::drawSpriteFrame);
    connect(fileManager, &FileManager::loadCanvas, model, &Model::loadCanvas);

    // Version History
    connect(ui->undoButton, &QPushButton::clicked, model->getVersionHistoryPointer(), &VersionHistory::undo);
    connect(ui->redoButton, &QPushButton::clicked, model->getVersionHistoryPointer(), &VersionHistory::redo);
    connect(model->getVersionHistoryPointer(), &VersionHistory::versionChanged, this, &MainWindow::updateSprite);
    connect(this, &MainWindow::saveNewVersion, model, &Model::appendVersion);
    connect(ui->versionHistoryComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::selectVersion);
    connect(model->getVersionHistoryPointer(), &VersionHistory::versionChanged, this, &MainWindow::updateVersionComboBox);
    connect(model->getVersionHistoryPointer(), &VersionHistory::versionChanged, this, &MainWindow::revertSprite);
}

MainWindow::~MainWindow()
{
    tool = nullptr;
    sprite = nullptr;
    model = nullptr;
    delete fileManager;
    delete ui;
    delete graphic;
    qDebug() << "all pointers destructed in MW.";
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
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    double mouseX = event->position().x();
    double mouseY = event->position().y();
    QLabel* canvas = ui->canvas;

    if (isInCanvas(mouseX, mouseY)) {
        mouseX = mouseX - canvas->pos().x();
        mouseY = mouseY - canvas->pos().y();

        canvasX = (int)(mouseX / (canvas->width() / sprite->getWidth()));
        canvasY = (int)(mouseY / (canvas->width() / sprite->getWidth()));

        tool->useTool(sprite, canvasX, canvasY);
        newPosition = true;
    }
}

bool MainWindow::isInCanvas(int x, int y){
    bool isValidX = x > ui->canvas->pos().x() && x < ui->canvas->width() + ui->canvas->pos().x();
    bool isValidY = y > ui->canvas->pos().y() && y < ui->canvas->height() + ui->canvas->pos().y();
    return isValidX && isValidY;
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    double mouseX = event->position().x();
    double mouseY = event->position().y();

    //if the mouse is not in the canvas, we don't care.
    if(!isInCanvas(mouseX, mouseY))
        return;

    qDebug() << "Mouse released; sending signal from MAIN to MODEL to save new version.";
    emit saveNewVersion(sprite);
}

void MainWindow::drawSpriteFrame(int frame) {

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

void MainWindow::updateSprite() {
    qDebug() << "Updated sprite. Should be called after addHistory is called.";
    drawSpriteFrame(sprite->getFrame());
}

// COLORPALETTE
void MainWindow::addNewColorButton(QColor color) {
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
}

void MainWindow::selectedColor(QAbstractButton *button) {
    if (!button) return;

    QRegularExpression regexSolidWhite("solid white");
    QRegularExpression regexSolidBlack("solid black");
    if (currentColorButton != nullptr) {
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

// VERSION HISTORY
void MainWindow::selectVersion(int index) {
    if(ui->versionHistoryComboBox->hasFocus())
        model->setVersionIndex(index);
    qDebug() << "version index changed to " << index;
    //revertSprite(versionHistory->getSpriteAtCurrentVersion());
}

void MainWindow::updateUndoRedoButtons() {
    // If there is no previous version, disable undo button.
    ui->undoButton->setEnabled(model->getVersionIndex() > 0);

    // If there is no next version, disable redo button.
    ui->redoButton->setEnabled(model->getVersionIndex() < model->getHistorySize() - 1);
}

void MainWindow::updateVersionComboBox() {
    // Block signals to prevent triggering versionSelected.
    ui->versionHistoryComboBox->blockSignals(true);

    qDebug() << "version updated, combo box will update.";

    //store the string name of the tool used...
    QString currentToolString;
    switch (tool->getCurrentTool()){
    case Tool::Tools::Pen:
        currentToolString = "Pen";
        break;
    case Tool::Tools::Eraser:
        currentToolString = "Eraser";
        break;
    case Tool::Tools::PaintBucket:
        currentToolString = "Paint Bucket";
        break;
    }

    // ...then, update the VH combobox items.
    ui->versionHistoryComboBox->clear();
    for(int i = 0; i < model->getHistorySize(); i++){
        QString label(QString("Version %1: ").arg(i+1).append(currentToolString));
        ui->versionHistoryComboBox->addItem(label);
    }

    // Select the current version
    ui->versionHistoryComboBox->setCurrentIndex(model->getVersionIndex());

    ui->versionHistoryComboBox->blockSignals(false);
}

void MainWindow::revertSprite(const Sprite& currentSprite) {
    *sprite = currentSprite;
    qDebug() << "reverting sprite";
    updateSprite();
    updateVersionComboBox();
    updateUndoRedoButtons();
}

