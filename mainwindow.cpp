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
#include <QLineEdit>
#include <QTimer>
#include <QCheckBox>

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
    tool = model->getToolPointer();
    sprite = model->getSpritePointer();
    fps = 0;
    animationFrame = 0;

    fileManager->setSprite(sprite);

    fpsValidator = new QIntValidator(0, 60, this); // Accepts integers from 0 to 100
    ui->fpsInput->setValidator(fpsValidator);

    // COLOR PALETTE SETUP:
    colorPalette = new ColorPalette();
    colorPaletteButtonGroup = new QButtonGroup(this);
    currentColorButton = nullptr;

    // ANIMATION SETUP
    animationTimer = new QTimer(this);
      
    // CONNECTIONS:
    // view to model:
    connect(ui->saveButton, &QPushButton::clicked,
            fileManager,&FileManager::saveButtonClicked); // save button setup
    connect(ui->loadButton, &QPushButton::clicked,
            fileManager, &FileManager::loadButtonClicked); // load button setup
    connect(ui->addColor, &QPushButton::clicked,
            colorPalette, &ColorPalette::queryColor); // add color button setup
    connect(ui->eraseButton, &QPushButton::clicked,
            model, [model]{model->setTool(Tool::Tools::Eraser);}); // erase tool button setup
    connect(ui->paintButton, &QPushButton::clicked,
            model, [model]{model->setTool(Tool::Tools::Pen);}); // paint tool button setup
    connect(ui->fillButton, &QPushButton::clicked,
            model, [model]{model->setTool(Tool::Tools::PaintBucket);}); // fill tool button setup
    connect(ui->savePaletteButton, &QPushButton::clicked,
            colorPalette, &ColorPalette::savePalette); // save palette button setup
    connect(ui->loadPaletteButton, &QPushButton::clicked,
            colorPalette, &ColorPalette::loadPalette); // load palette button setup
    connect(ui->undoButton, &QPushButton::clicked,
            model->getVersionHistoryPointer(), &VersionHistory::undo); // undo button setup
    connect(ui->redoButton, &QPushButton::clicked,
            model->getVersionHistoryPointer(), &VersionHistory::redo); // redo button setup
    connect(this, &MainWindow::ctrlZPressed,
            model->getVersionHistoryPointer(), &VersionHistory::undo); // undo keyboard shortcut setup
    connect(this, &MainWindow::ctrlYPressed,
            model->getVersionHistoryPointer(), &VersionHistory::redo); // redo keyboard shortcut setup

    // controller to model:
    connect(this, &MainWindow::colorDeleted,
            colorPalette, &ColorPalette::removeColor); // delete color signal
    connect(this, &MainWindow::saveNewVersion,
            model, &Model::appendVersion); // new version signal

    // model to controller:
    connect(colorPalette, &ColorPalette::addNewColorButtonSignal,
            this, &MainWindow::addNewColorButton); // add button color in palette
    connect(colorPalette, &ColorPalette::removeColorSignal,
            this, &MainWindow::removeColorButton); // remove color in palette
    connect(colorPalette, &ColorPalette::clearColorPalette,
            this, &MainWindow::clearColorPalette); // empty the color palette
    connect(model, &Model::updateSprite,
            this, &MainWindow::updateSprite); // update sprite signal
    connect(sprite, &Sprite::drawFrame,
            this, &MainWindow::drawSpriteFrame); // draw the sprite at a frame
    connect(model->getVersionHistoryPointer(), &VersionHistory::versionChanged,
            this, &MainWindow::updateSprite); // history version switched (sprite visuals)
    connect(model->getVersionHistoryPointer(), &VersionHistory::versionChanged,
            this, &MainWindow::updateVersionComboBox); // history version switched (history list)
    connect(model->getVersionHistoryPointer(), &VersionHistory::versionChanged,
            this, &MainWindow::revertSprite); // history version switched (sprite data)
    connect(model, &Model::toolChanged,
            this, &MainWindow::toggleToolButtons);
    
    // view to controller:
    connect(colorPaletteButtonGroup, &QButtonGroup::buttonClicked,
            this, &MainWindow::selectedColor); // color palette select
    connect(ui->removeColor, &QPushButton::clicked,
            this, &MainWindow::removeColorButton); // remove color button setup
    connect(ui->confirmResize, &QPushButton::clicked,
            this, &MainWindow::resizeButtonPressed); // resize button setup
    connect(ui->versionHistoryComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::selectVersion); // version history selection setup
    connect(ui->fpsSlider, &QSlider::valueChanged,
            this, &MainWindow::updateFPS);
    connect(ui->prevFrameButton, &QPushButton::clicked,
            this, &MainWindow::goToPrevFrame);
    connect(ui->nextFrameButton, &QPushButton::clicked,
            this, &MainWindow::goToNextFrame);
    connect(ui->deleteCurrentFrameButton, &QPushButton::clicked,
            this, &MainWindow::deleteCurrentFrame);
    connect(ui->addPrevFrameButton, &QPushButton::clicked,
            this, &MainWindow::addPrevFrame);
    connect(ui->addNextFrameButton, &QPushButton::clicked,
            this, &MainWindow::addNextFrame);
    connect(ui->frameSelectorCombobox, &QComboBox::currentIndexChanged,
            this, &MainWindow::goToFrameIndex);
    connect(animationTimer, &QTimer::timeout, 
            this, QOverload<>::of(&MainWindow::drawAnimationFrame));
    connect(ui->trueToSizeCheckbox, &QCheckBox::checkStateChanged,
            this, &MainWindow::trueToSizeToggled);

    // model to model:
    connect(fileManager, &FileManager::loadCanvas,
            model, &Model::loadCanvas); // communicates a loaded sprite
}

MainWindow::~MainWindow()
{
    tool = nullptr;
    sprite = nullptr;
    model = nullptr;
    delete fileManager;
    delete ui;
    delete fpsValidator;
}

// MOUSE EVENTS
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
        newPosition = false;
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

    // If the mouse is not in the canvas, return.
    if(!isInCanvas(mouseX, mouseY))
        return;

    emit saveNewVersion(sprite);
}

// DRAWING
void MainWindow::drawSpriteFrame(int frame) {
    qDebug() << "drawSpriteFrame";
    int width = sprite->getWidth();
    int oldFrame = sprite->getFrame();
    sprite->setCurrentFrame(frame);

    // Resize loaded .ssp file to correct width and update QLineEdit.
    image = QImage(width, width, QImage::Format_ARGB32);
    ui->widthInput->setText(QString::number(width));

    int pixelColors[4];

    for (int row = 0; row < width; row++) {
        for (int col = 0; col < width; col++) {
            sprite->getPixel(row, col, pixelColors); //get pixel location & colors
            QColor setColor = QColor(
                pixelColors[0], pixelColors[1], pixelColors[2], pixelColors[3]);
            image.setPixelColor(row, col, setColor);
        }
    }

    ui->canvas->setPixmap(QPixmap::fromImage(image).scaled(
        ui->canvas->size(),
        Qt::KeepAspectRatio,
        Qt::FastTransformation));
    sprite->setCurrentFrame(oldFrame);
}

void MainWindow::updateSprite() {
    qDebug() << "started update sprite. Should be called after addHistory is called.";
    drawSpriteFrame(sprite->getFrame());
    qDebug() << "Updated sprite. Should be called after addHistory is called.";
}


void MainWindow::addNewColorButton(QColor color, int id) {
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
    QString styleSheet = QString("background-color: rgba(%1, %2, %3, %4);"
                                 "border: 1px solid black;")
        .arg(color.red())
        .arg(color.green())
        .arg(color.blue())
        .arg(color.alpha());
    newButton->setStyleSheet(styleSheet);
    colorPaletteButtonGroup->addButton(newButton, id);
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
    // QColor newColor = currentColorButton->palette().color(QPalette::Button);
    qDebug() << "buttongroup id: " << colorPaletteButtonGroup->id(currentColorButton);
    QColor newColor = colorPalette->getColor(colorPaletteButtonGroup->id(currentColorButton));
    tool->setCurrentColor({newColor.red(), newColor.green(), newColor.blue(), newColor.alpha()});
    QString styleSheet = QString("QLabel  {"
        "background-color: rgba(%1, %2, %3, %4); "
        "border-color: black; "
        "border-style: solid; "
        "border-width: 2px; "
        "border-radius: 5px;}")
        .arg(newColor.red()).arg(newColor.green()).arg(newColor.blue()).arg(newColor.alpha());
    ui->currentColorLabel->setStyleSheet(styleSheet);
}

void MainWindow::removeColorButton() {
    if (currentColorButton) {
        int id = colorPaletteButtonGroup->id(currentColorButton);
        colorPalette->removeColor(id);
        colorPaletteButtonGroup->removeButton(currentColorButton);
        delete currentColorButton;
        currentColorButton = nullptr;
        emit colorDeleted(id);
    }
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
    model->setVersionIndex(index);
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
    drawAnimationFrame(animationFrame);

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

    updateSprite();
    updateVersionComboBox();
    updateUndoRedoButtons();
}

void MainWindow::resizeButtonPressed() {
    bool viable;
    int newWidth = ui->widthInput->text().toInt(&viable);
    if (viable) {
        image = QImage(newWidth, newWidth, QImage::Format_RGBA64);
        sprite->resize(newWidth);
    }
}

void MainWindow::toggleToolButtons() {
    Tool::Tools updatedTool = tool->getCurrentTool();

    // Set all the buttons to unchecked
    ui->paintButton->setDisabled(false);
    ui->eraseButton->setDisabled(false);
    ui->fillButton->setDisabled(false);

    // Then, toggle the ones that SHOULD be checked.
    switch(updatedTool) {
    case Tool::Tools::Pen:
        ui->paintButton->setDisabled(true);
        break;
    case Tool::Tools::Eraser:
        ui->eraseButton->setDisabled(true);
        break;
    case Tool::Tools::PaintBucket:
        ui->fillButton->setDisabled(true);
        break;
    }
}

// ANIMATION
void MainWindow::trueToSizeToggled(int status) {
    drawAnimationFrame();
}

void MainWindow::drawAnimationFrame() {
    drawAnimationFrame((animationFrame + 1) % sprite->getNumberOfFrames());
}

void MainWindow::drawAnimationFrame(int frame) {
    int width = sprite->getWidth();
    animationFrame = frame;

    // //Resize loaded .ssp file to correct width and update QLineEdit.
    animationImage = QImage(width, width, QImage::Format_ARGB32);

    int pixelColors[4];

    //
    for (int row = 0; row < width; row++) {
        for (int col = 0; col < width; col++) {

            sprite->getPixelAtFrame(animationFrame, row, col, pixelColors); //get pixel location & colors
            QColor setColor = QColor(
                pixelColors[0], pixelColors[1], pixelColors[2], pixelColors[3]);
            animationImage.setPixelColor(row, col, setColor);
        }
    }
    if(ui->trueToSizeCheckbox->isChecked()){
        ui->animationPreview->setPixmap(QPixmap::fromImage(animationImage));
    } else {
        ui->animationPreview->setPixmap(QPixmap::fromImage(animationImage).scaled(
            ui->animationPreview->size(),
            Qt::KeepAspectRatio,
            Qt::FastTransformation));
    }

}

// FRAMES
void MainWindow::updateFPS(int newFps) {
    fps = newFps;
    ui->fpsInput->setText(QString::number(fps));

    // Trigger animation changes.
    if(fps == 0)
        animationTimer->stop();
    else
        animationTimer->start(1000/fps);

}

void MainWindow::goToNextFrame() {
    int nextFrameIndex = sprite->getFrame() + 1;

    // Ensure there is a next frame before continuing.
    if(nextFrameIndex < sprite->getNumberOfFrames()) {
        sprite->setCurrentFrame(nextFrameIndex);
        drawSpriteFrame(sprite->getFrame());
        ui->frameSelectorCombobox->setCurrentIndex(sprite->getFrame());
    }
    qDebug() << "index: " << sprite->getFrame();
}

void MainWindow::goToPrevFrame() {
    int prevFrameIndex = sprite->getFrame() - 1;

    // Ensure there is a previous frame before continuing.
    if (prevFrameIndex >= 0) {
        sprite->setCurrentFrame(prevFrameIndex);
        drawSpriteFrame(sprite->getFrame());
        ui->frameSelectorCombobox->setCurrentIndex(sprite->getFrame());
    }
    qDebug() << "index: " << sprite->getFrame();
}

void MainWindow::goToFrameIndex(int index) {
    sprite->setCurrentFrame(index);
    drawSpriteFrame(sprite->getFrame());
    ui->frameSelectorCombobox->setCurrentIndex(sprite->getFrame());
}

void MainWindow::deleteCurrentFrame() {
    ui->frameSelectorCombobox->removeItem(sprite->getFrame());
    sprite->deleteCurrentFrame();
}

void MainWindow::addPrevFrame() {
    int currentFrameIndex = sprite->getFrame();
    sprite->insertFrame(currentFrameIndex);
    ui->frameSelectorCombobox->insertItem(currentFrameIndex,
                                          "Frame " + QString::number(currentFrameIndex + 1));
    ui->frameSelectorCombobox->setItemText(currentFrameIndex + 1,
                                           "Frame " + QString::number(currentFrameIndex + 2));
}

void MainWindow::addNextFrame() {
    int currentFrameIndex = sprite->getFrame() + 1;
    sprite->insertFrame(currentFrameIndex);
    ui->frameSelectorCombobox->insertItem(currentFrameIndex,
                                "Frame " + QString::number(currentFrameIndex + 1));
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->modifiers() & Qt::ControlModifier && event->key() == Qt::Key_Z) {
        emit ctrlZPressed();
    }
    if (event->modifiers() & Qt::ControlModifier && event->key() == Qt::Key_Y) {
        emit ctrlYPressed();
    }
    QMainWindow::keyPressEvent(event);
}

