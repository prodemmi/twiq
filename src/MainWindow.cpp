// Written by malekpour-dev.ir
// MainWindow is a QWidget-based class responsible for setting up the main window, its components, and their connections.

#include "MainWindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_mainSplitter(nullptr), m_controlPanel(nullptr), m_fpsTimer(new QTimer(this)),
      m_animationTimer(new QTimer(this)), m_frameCount(0), m_currentColor(Qt::blue), m_isAnimating(true),
      m_selectedItemId(-1), m_updatingControls(false)
{
    setWindowTitle("twiq");
    setMinimumSize(800, 600);
    setupUI();
    setupMenuBar();
    setupToolBar();
    connectSignals();

    // Initialize FPS counter
    m_fpsTimer->setInterval(1000);
    connect(m_fpsTimer, &QTimer::timeout, this, &MainWindow::updateFrameRate);
    m_fpsTimer->start();

    // Initialize animation timer
    m_animationTimer->setInterval(16); // ~60 FPS
    connect(m_animationTimer, &QTimer::timeout, this, &MainWindow::updateAnimation);
    m_animationTimer->start();

    statusBar()->showMessage("Ready - Click 'Add Spinner' to create your first spinner");

    enableItemControls(false);
    updateItemList();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    QWidget *central = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(central);

    setupControlPanel();
    setupCanvas();

    layout->addWidget(m_controlPanel);
    layout->addWidget(m_canvas, 1, Qt::AlignCenter);

    central->setLayout(layout);
    setCentralWidget(central);
}

void MainWindow::setupMenuBar()
{
    QMenuBar *menuBar = this->menuBar();

    QMenu *fileMenu = menuBar->addMenu("&File");
    fileMenu->addAction("&Export Animation...", this, &MainWindow::onExportClicked, QKeySequence::SaveAs);
    fileMenu->addSeparator();
    fileMenu->addAction("E&xit", QKeySequence::Quit, this, &QWidget::close);

    QMenu *editMenu = menuBar->addMenu("&Edit");
    editMenu->addAction("&Add Spinner", QKeySequence("Ctrl+N"), this, &MainWindow::onAddSpinnerClicked);
    editMenu->addAction("&Remove Selected", QKeySequence::Delete, this, &MainWindow::onRemoveSpinnerClicked);
    editMenu->addAction("&Clear All", QKeySequence("Ctrl+Shift+N"), this, &MainWindow::onClearAllClicked);

    QMenu *animMenu = menuBar->addMenu("&Animation");
    animMenu->addAction("&Start/Stop", QKeySequence("Space"), this, &MainWindow::onStartStopClicked);

    QMenu *helpMenu = menuBar->addMenu("&Help");
    helpMenu->addAction("&About", [this]()
                        { QMessageBox::about(this, "About twiq",
                                             "<b>twiq v1.0.0</b><br><br>"
                                             "A modern multi-spinner animation creator using <b>Qt</b> and <b>Blend2D</b>.<br><br>"
                                             "<b>Website:</b> <a href=\"https://malekpour-dev.ir\">malekpour-dev.ir</a><br>"
                                             "<b>GitHub:</b> <a href=\"https://github.com/prodemmi\">github.com/prodemmi</a>"); });
}

void MainWindow::setupToolBar()
{
    QToolBar *toolBar = addToolBar("Main");
    toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolBar->setMovable(false);

    m_startStopButton = new QPushButton("⏸️ Pause");
    m_exportButton = new QPushButton("💾 Export");
    QPushButton *addSpinnerButton = new QPushButton("➕ Add Spinner");
    m_exploreTemplatesButton = new QPushButton("🎨 Explore Templates");

    toolBar->addWidget(addSpinnerButton);
    toolBar->addSeparator();
    toolBar->addWidget(m_startStopButton);
    toolBar->addSeparator();
    toolBar->addWidget(m_exportButton);
    toolBar->addSeparator();
    toolBar->addWidget(m_exploreTemplatesButton);

    connect(addSpinnerButton, &QPushButton::clicked, this, &MainWindow::onAddSpinnerClicked);
    connect(m_exploreTemplatesButton, &QPushButton::clicked, this, &MainWindow::onExploreTemplatesClicked);
}

void MainWindow::setupControlPanel()
{
    m_controlPanel = new QWidget();
    m_controlPanel->setMinimumWidth(280);
    m_controlPanel->setStyleSheet("QGroupBox { font-weight: bold; margin-top: 8px; }");

    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QWidget *scrollContent = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(scrollContent);
    layout->setSpacing(12);
    layout->setContentsMargins(8, 8, 8, 8);

    setupItemList();
    setupItemProperties();

    layout->addWidget(m_itemListGroup);
    layout->addWidget(m_propertiesGroup);
    layout->addStretch();

    QLabel *infoLabel = new QLabel(
        "💡 Tips: Click to select • Drag to move • Right-click for menu");
    infoLabel->setStyleSheet("color: #888; font-size: 10px; padding: 8px;");
    infoLabel->setWordWrap(true);
    layout->addWidget(infoLabel);

    scrollArea->setWidget(scrollContent);

    QVBoxLayout *mainLayout = new QVBoxLayout(m_controlPanel);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(scrollArea);
}

void MainWindow::setupItemList()
{
    m_itemListGroup = new QGroupBox("Spinner Items");
    QVBoxLayout *listLayout = new QVBoxLayout(m_itemListGroup);

    m_itemList = new QListWidget();
    m_itemList->setMaximumHeight(150);
    listLayout->addWidget(m_itemList);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    m_addButton = new QPushButton("➕ Add");
    m_removeButton = new QPushButton("➖ Remove");
    m_clearAllButton = new QPushButton(" Clear All");

    buttonLayout->addWidget(m_addButton);
    buttonLayout->addWidget(m_removeButton);
    buttonLayout->addWidget(m_clearAllButton);
    listLayout->addLayout(buttonLayout);
}

void MainWindow::setupItemProperties()
{
    m_propertiesGroup = new QGroupBox("Item Properties");
    QGridLayout *propsLayout = new QGridLayout(m_propertiesGroup);

    int row = 0;

    propsLayout->addWidget(new QLabel("Name:"), row, 0);
    m_nameEdit = new QLineEdit();
    propsLayout->addWidget(m_nameEdit, row, 1, 1, 2);
    row++;

    propsLayout->addWidget(new QLabel("X:"), row, 0);
    m_xSpinBox = new QSpinBox();
    m_xSpinBox->setRange(0, 100);
    propsLayout->addWidget(m_xSpinBox, row, 1);

    propsLayout->addWidget(new QLabel("Y:"), row, 2);
    m_ySpinBox = new QSpinBox();
    m_ySpinBox->setRange(0, 100);
    propsLayout->addWidget(m_ySpinBox, row, 3);
    row++;

    propsLayout->addWidget(new QLabel("Type:"), row, 0);
    m_typeCombo = new QComboBox();
    m_typeCombo->addItems({"Circle", "Ring", "Rectangle", "Square", "Triangle", "Star"});
    propsLayout->addWidget(m_typeCombo, row, 1, 1, 3);
    row++;

    propsLayout->addWidget(new QLabel("Animation:"), row, 0);
    m_animationCombo = new QComboBox();
    m_animationCombo->addItems({"None", "Rotate", "Scale", "Fade", "Bounce", "Slide"});
    propsLayout->addWidget(m_animationCombo, row, 1, 1, 3);
    row++;

    propsLayout->addWidget(new QLabel("Speed:"), row, 0);
    m_speedSlider = new QSlider(Qt::Horizontal);
    m_speedSlider->setRange(1, 100);
    m_speedSlider->setValue(50);
    m_speedLabel = new QLabel("50%");
    propsLayout->addWidget(m_speedSlider, row, 1, 1, 2);
    propsLayout->addWidget(m_speedLabel, row, 3);
    row++;

    propsLayout->addWidget(new QLabel("Size:"), row, 0);
    m_sizeSlider = new QSlider(Qt::Horizontal);
    m_sizeSlider->setRange(0, 100);
    m_sizeSlider->setValue(32);
    m_sizeLabel = new QLabel("32px");
    propsLayout->addWidget(m_sizeSlider, row, 1, 1, 2);
    propsLayout->addWidget(m_sizeLabel, row, 3);
    row++;

    propsLayout->addWidget(new QLabel("Duration:"), row, 0);
    m_durationSlider = new QSlider(Qt::Horizontal);
    m_durationSlider->setRange(0, 40);
    m_durationSlider->setValue(1);
    propsLayout->addWidget(m_durationSlider, row, 1);
    m_durationLabel = new QLabel("1.0s");
    propsLayout->addWidget(m_durationLabel, row, 2);
    row++;

    propsLayout->addWidget(new QLabel("Pre-delay:"), row, 0);
    m_preDelaySlider = new QSlider(Qt::Horizontal);
    m_preDelaySlider->setRange(0, 40);
    m_preDelaySlider->setValue(0);
    propsLayout->addWidget(m_preDelaySlider, row, 1);
    m_preDelayLabel = new QLabel("0s");
    propsLayout->addWidget(m_preDelayLabel, row, 2);
    row++;
    
    propsLayout->addWidget(new QLabel("Post-delay:"), row, 0);
    m_postDelaySlider = new QSlider(Qt::Horizontal);
    m_postDelaySlider->setRange(0, 40);
    m_postDelaySlider->setValue(0);
    propsLayout->addWidget(m_postDelaySlider, row, 1);
    m_postDelayLabel = new QLabel("0s");
    propsLayout->addWidget(m_postDelayLabel, row, 2);
    row++;
    
    propsLayout->addWidget(new QLabel("Color:"), row, 0);
    m_colorButton = new QPushButton();
    m_colorButton->setStyleSheet(QString("background-color: %1; border: 2px solid white;").arg(m_currentColor.name()));
    m_colorButton->setFixedSize(60, 30);
    propsLayout->addWidget(m_colorButton, row, 1, 1, 3);
}

void MainWindow::setupCanvas()
{
    m_canvas = new CanvasWidget(this);
    m_canvas->setFixedSize(300, 300);
}

void MainWindow::connectSignals()
{
    connect(m_addButton, &QPushButton::clicked, this, &MainWindow::onAddSpinnerClicked);
    connect(m_removeButton, &QPushButton::clicked, this, &MainWindow::onRemoveSpinnerClicked);
    connect(m_clearAllButton, &QPushButton::clicked, this, &MainWindow::onClearAllClicked);
    connect(m_itemList, &QListWidget::itemClicked, this, &MainWindow::onItemListItemClicked);

    connect(m_nameEdit, &QLineEdit::textChanged, this, &MainWindow::onNameChanged);
    connect(m_typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onSpinnerTypeChanged);
    connect(m_animationCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onSpinnerAnimationChanged);
    connect(m_speedSlider, &QSlider::valueChanged, this, &MainWindow::onSpeedChanged);
    connect(m_sizeSlider, &QSlider::valueChanged, this, &MainWindow::onSizeChanged);
    connect(m_colorButton, &QPushButton::clicked, this, &MainWindow::onColorChanged);

    connect(m_startStopButton, &QPushButton::clicked, this, &MainWindow::onStartStopClicked);
    connect(m_exportButton, &QPushButton::clicked, this, &MainWindow::onExportClicked);

    connect(m_canvas, &CanvasWidget::itemSelected, this, &MainWindow::onCanvasItemSelected);
    connect(m_canvas, &CanvasWidget::itemDeselected, this, &MainWindow::onCanvasItemDeselected);
    connect(m_canvas, &CanvasWidget::itemsChanged, this, &MainWindow::onCanvasItemsChanged);

    connect(m_speedSlider, &QSlider::valueChanged, this, [this](int value)
            { m_speedLabel->setText(QString("%1%").arg(value)); });

    connect(m_sizeSlider, &QSlider::valueChanged, this, [this](int value)
            { m_sizeLabel->setText(QString("%1px").arg(value)); });

    connect(m_xSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onPositionChanged);
    connect(m_ySpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onPositionChanged);
    connect(m_preDelaySlider, &QSlider::valueChanged, this, &MainWindow::onPreDelayChanged);
    connect(m_postDelaySlider, &QSlider::valueChanged, this, &MainWindow::onPostDelayChanged);

    connect(m_durationSlider, &QSlider::valueChanged, this, &MainWindow::onDurationChanged);
    connect(m_preDelaySlider, &QSlider::valueChanged, this, &MainWindow::onPreDelayChanged);
    connect(m_postDelaySlider, &QSlider::valueChanged, this, &MainWindow::onPostDelayChanged);
}

void MainWindow::onAddSpinnerClicked()
{
    bool ok;
    QStringList items;
    items << "Circle" << "Ring" << "Rectangle" << "Square" << "Triangle" << "Star";

    QString selectedType = QInputDialog::getItem(this, "Add Spinner",
                                                 "Select spinner type:", items, 0, false, &ok);

    if (ok && !selectedType.isEmpty())
    {
        SpinnerType type;
        if (selectedType == "Circle")
            type = SpinnerType::Circle;
        else if (selectedType == "Star")
            type = SpinnerType::Star;
        else if (selectedType == "Triangle")
            type = SpinnerType::Triangle;
        else if (selectedType == "Rectangle")
            type = SpinnerType::Rectangle;
        else if (selectedType == "Square")
            type = SpinnerType::Square;
        else
            type = SpinnerType::Ring;

        // Add spinner at center of canvas
        QPointF center(50, 50);
        QColor color = Qt::blue;
        QString colorString = color.name();
        int id = m_canvas->addSpinner(type, SpinnerAnimation::Rotate, center, 54, colorString, 20.0f, 1.0f, 0.0f, 0.0f);

        // Select the new spinner
        m_canvas->selectItem(id);

        statusBar()->showMessage(QString("Added new %1 spinner").arg(selectedType));
    }
}

void MainWindow::onRemoveSpinnerClicked()
{
    if (m_selectedItemId != -1)
    {
        m_canvas->removeSpinner(m_selectedItemId);
        statusBar()->showMessage("Spinner removed");
    }
    else
    {
        statusBar()->showMessage("No spinner selected");
    }
}

void MainWindow::onClearAllClicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Clear All",
                                                              "Are you sure you want to remove all spinners?",
                                                              QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        m_canvas->clearAll();
        statusBar()->showMessage("All spinners cleared");
    }
}

void MainWindow::onItemListItemClicked(QListWidgetItem *item)
{
    if (item)
    {
        int id = item->data(Qt::UserRole).toInt();
        m_canvas->selectItem(id);
    }
}

void MainWindow::onSpinnerTypeChanged(int index)
{
    if (m_updatingControls || m_selectedItemId == -1)
        return;

    m_canvas->setItemProperty(m_selectedItemId, "type", index);
}

void MainWindow::onSpinnerAnimationChanged(int index)
{
    if (m_updatingControls || m_selectedItemId == -1)
        return;

    m_canvas->setItemProperty(m_selectedItemId, "anim", index);
}

void MainWindow::onSpeedChanged(float value)
{
    if (m_updatingControls || m_selectedItemId == -1)
        return;

    m_canvas->setItemProperty(m_selectedItemId, "speed", static_cast<float>(value));
}

void MainWindow::onSizeChanged(int value)
{
    if (m_updatingControls || m_selectedItemId == -1)
        return;

    m_canvas->setItemProperty(m_selectedItemId, "size", value);
}

void MainWindow::onColorChanged()
{
    if (m_selectedItemId == -1)
        return;

    QString colorValue = m_canvas->getItemProperty(m_selectedItemId, "color").toString();

    QColor currentColor = QColor::fromString(colorValue);
    QColor color = QColorDialog::getColor(currentColor, this, "Select Spinner Color");

    if (color.isValid())
    {
        m_canvas->setItemProperty(m_selectedItemId, "color", color.name());
        m_colorButton->setStyleSheet(QString("background-color: %1; border: 2px solid white;").arg(color.name()));
    }

    updateItemList();
}

void MainWindow::onNameChanged()
{
    if (m_updatingControls || m_selectedItemId == -1)
        return;

    m_canvas->setItemProperty(m_selectedItemId, "name", m_nameEdit->text());
    updateItemList();
}

// Animation Slots
void MainWindow::onStartStopClicked()
{
    m_isAnimating = !m_isAnimating;
    m_startStopButton->setText(m_isAnimating ? "⏸️ Pause" : "▶️ Play");
    m_canvas->setAnimating(m_isAnimating);

    statusBar()->showMessage(m_isAnimating ? "Animation playing" : "Animation paused");
}

QImage MainWindow::captureFrame()
{
    // Create an image with transparent background for export
    QImage image(m_canvas->size(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    BLImage blImage;
    blImage.createFromData(
        image.width(),
        image.height(),
        BL_FORMAT_PRGB32,
        image.bits(),
        image.bytesPerLine(),
        BL_DATA_ACCESS_RW,
        nullptr,
        nullptr);

    BLContext ctx(blImage);
    ctx.setCompOp(BL_COMP_OP_SRC_OVER);
    ctx.clearAll();

    // Draw all spinners
    for (const auto &item : m_canvas->getItems())
    {
        if (!item)
            continue;
        m_canvas->drawSpinner(ctx, *item);
    }

    ctx.end();

    image = image.copy();
    return image;
}

bool MainWindow::exportGif(const QString &fileName, QProgressDialog &progress)
{
    // Use the canvas's animation FPS and duration, not the current frameCount
    double duration = m_canvas->getAnimationDuration();
    if (duration <= 0.0)
        duration = 1.0; // fallback to 1 second if not set

    int fps = m_frameCount;
    if (fps <= 0)
        fps = 60; // fallback to 60 FPS

    // Calculate total frames based on duration and FPS
    int totalFrames = static_cast<int>(duration * fps);
    if (totalFrames <= 0)
        totalFrames = 60; // fallback to 60 frames

    const int width = m_canvas->width();
    const int height = m_canvas->height();
    const char *filename = fileName.toUtf8().constData();

    int error;
    GifFileType *gif = EGifOpenFileName(filename, false, &error);
    if (!gif)
    {
        qDebug() << "Error opening GIF: " << GifErrorString(error);
        return false;
    }

    QVector<QImage> frames;
    frames.reserve(totalFrames);
    double dt = duration / totalFrames;

    // Save original animation state and time
    bool wasAnimating = m_isAnimating;
    double originalTime = duration / totalFrames;
    m_canvas->setAnimating(false);

    // Render each frame at correct time
    QVector<QVector<QRgb>> framePalettes;
    for (int i = 0; i < totalFrames; ++i)
    {
        double t = i * dt;
        m_canvas->setAnimationTime(t);
        QImage frame = captureFrame().convertToFormat(QImage::Format_ARGB32);

        // Convert to 8-bit with optimal palette and dithering, preserving transparency
        QImage quantized = frame.convertToFormat(
            QImage::Format_Indexed8,
            Qt::DiffuseDither | Qt::ThresholdAlphaDither | Qt::PreferDither);

        QVector<QRgb> palette = quantized.colorTable();
        int transparentIndex = -1;
        for (int j = 0; j < palette.size(); ++j)
        {
            if (qAlpha(palette[j]) == 0)
            {
                transparentIndex = j;
                break;
            }
        }
        if (transparentIndex == -1)
        {
            // Add transparent color if not present
            palette.insert(0, qRgba(0, 0, 0, 0));
            transparentIndex = 0;
            quantized.setColorTable(palette);
        }
        else if (transparentIndex != 0)
        {
            // Move transparent color to index 0
            QRgb transparentColor = palette[transparentIndex];
            palette.removeAt(transparentIndex);
            palette.insert(0, transparentColor);

            for (int y = 0; y < quantized.height(); ++y)
            {
                uchar *scan = quantized.scanLine(y);
                for (int x = 0; x < quantized.width(); ++x)
                {
                    if (scan[x] == transparentIndex)
                        scan[x] = 0;
                    else if (scan[x] < transparentIndex)
                        scan[x] += 1;
                }
            }
            quantized.setColorTable(palette);
            transparentIndex = 0;
        }

        for (int y = 0; y < quantized.height(); ++y)
        {
            uchar *scan = quantized.scanLine(y);
            for (int x = 0; x < quantized.width(); ++x)
            {
                if (qAlpha(palette[scan[x]]) < 128)
                {
                    scan[x] = transparentIndex;
                }
            }
        }

        frames.append(quantized);
        framePalettes.append(palette);

        // Set progress value for rendering frames
        progress.setValue(static_cast<int>((i + 1) * 50.0 / totalFrames));
        QApplication::processEvents();
        if (progress.wasCanceled())
        {
            EGifCloseFile(gif, &error);
            return false;
        }
    }

    m_canvas->setAnimationTime(originalTime);
    m_canvas->setAnimating(wasAnimating);

    QVector<QRgb> globalColorTable = frames[0].colorTable();
    int colorCount = globalColorTable.size();
    if (colorCount > 256)
        colorCount = 256;

    ColorMapObject *colorMap = GifMakeMapObject(colorCount, nullptr);
    for (int i = 0; i < colorCount; ++i)
    {
        colorMap->Colors[i].Red = qRed(globalColorTable[i]);
        colorMap->Colors[i].Green = qGreen(globalColorTable[i]);
        colorMap->Colors[i].Blue = qBlue(globalColorTable[i]);
    }

    if (EGifPutScreenDesc(gif, width, height, colorCount, 0, colorMap) == GIF_ERROR)
    {
        qDebug() << "Error writing screen desc";
        EGifCloseFile(gif, &error);
        GifFreeMapObject(colorMap);
        return false;
    }

    // Write each frame, and loop forever
    // Add Netscape Application Extension for infinite looping
    unsigned char netscapeExt[] = {1, 0, 0};
    if (EGifPutExtensionLeader(gif, APPLICATION_EXT_FUNC_CODE) == GIF_ERROR ||
        EGifPutExtensionBlock(gif, 11, "NETSCAPE2.0") == GIF_ERROR ||
        EGifPutExtensionBlock(gif, sizeof(netscapeExt), netscapeExt) == GIF_ERROR ||
        EGifPutExtensionTrailer(gif) == GIF_ERROR)
    {
        EGifCloseFile(gif, &error);
        GifFreeMapObject(colorMap);
        return false;
    }

    int frameDelay = static_cast<int>(100.0 / fps); 
    int transparentIndex = 0;                      

    for (int i = 0; i < frames.size(); ++i)
    {
        const QImage &img = frames[i];

        unsigned char gce[4] = {0x09, static_cast<unsigned char>(frameDelay), static_cast<unsigned char>(transparentIndex), 0};
        if (EGifPutExtension(gif, GRAPHICS_EXT_FUNC_CODE, 4, gce) == GIF_ERROR)
        {
            EGifCloseFile(gif, &error);
            GifFreeMapObject(colorMap);
            return false;
        }

        if (EGifPutImageDesc(gif, 0, 0, width, height, false, nullptr) == GIF_ERROR)
        {
            qDebug() << "Error writing image desc";
            EGifCloseFile(gif, &error);
            GifFreeMapObject(colorMap);
            return false;
        }
        
        for (int y = 0; y < height; ++y)
        {
            const uchar *scan = img.scanLine(y);
            if (EGifPutLine(gif, const_cast<GifByteType *>(scan), width) == GIF_ERROR)
            {
                qDebug() << "Error writing GIF line";
                EGifCloseFile(gif, &error);
                GifFreeMapObject(colorMap);
                return false;
            }
        }

        // Set progress value for writing frames
        progress.setValue(50 + static_cast<int>((i + 1) * 50.0 / frames.size()));
        QApplication::processEvents();
        if (progress.wasCanceled())
        {
            EGifCloseFile(gif, &error);
            GifFreeMapObject(colorMap);
            return false;
        }
    }

    if (EGifCloseFile(gif, &error) == GIF_ERROR)
    {
        qDebug() << "Error closing GIF file: " << GifErrorString(error);
        GifFreeMapObject(colorMap);
        return false;
    }
    GifFreeMapObject(colorMap);

    progress.setValue(100);

    qDebug() << "GIF created: " << filename;
    return true;
}

void MainWindow::onExportClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Export Animation", "spinner_animation.gif",
                                                    "GIF Files (*.gif);;PNG Sequence (*.png)");

    if (!fileName.isEmpty())
    {
        if (fileName.endsWith(".gif", Qt::CaseInsensitive))
        {
            // Show progress dialog
            QProgressDialog progress("Exporting animation...", "Cancel", 0, 100, this);
            progress.setWindowModality(Qt::WindowModal);
            progress.setMinimumDuration(0);
            progress.setValue(0);
            progress.show();

            // Export GIF
            if (exportGif(fileName, progress))
            {
                progress.setValue(100);
                QMessageBox::information(this, "Export", "Animation exported successfully!");
            }
            else
            {
                QMessageBox::critical(this, "Export Error", "Failed to export animation.");
            }
        }
    }
}

void MainWindow::updateFrameRate()
{
    statusBar()->showMessage(QString("FPS: %1 - %2 spinners active")
                                 .arg(m_frameCount)
                                 .arg(m_canvas->getItems().size()));
    m_frameCount = 0;
}

void MainWindow::updateAnimation()
{
    if (m_isAnimating)
    {
        m_canvas->updateAnimation();
        m_frameCount++;
    }
}

void MainWindow::onCanvasItemSelected(int id)
{
    m_selectedItemId = id;
    updateItemProperties();
    enableItemControls(true);

    for (int i = 0; i < m_itemList->count(); ++i)
    {
        QListWidgetItem *item = m_itemList->item(i);
        if (item->data(Qt::UserRole).toInt() == id)
        {
            m_itemList->setCurrentItem(item);
            break;
        }
    }

    auto *spinnerItem = m_canvas->getSelectedItem();
    if (spinnerItem)
    {
        statusBar()->showMessage(QString("Selected: %1").arg(spinnerItem->name));
    }
}

void MainWindow::onCanvasItemDeselected()
{
    m_selectedItemId = -1;
    enableItemControls(false);
    m_itemList->clearSelection();
    statusBar()->showMessage("No item selected");
}

void MainWindow::onCanvasItemsChanged()
{
    updateItemList();
}

void MainWindow::updateItemList()
{
    m_itemList->clear();

    const auto &items = m_canvas->getItems();
    for (const auto &item : items)
    {
        QString typeStr;
        switch (item->type)
        {
        case SpinnerType::Circle:
            typeStr = "Circle";
            break;
        case SpinnerType::Ring:
            typeStr = "Ring";
            break;
        case SpinnerType::Rectangle:
            typeStr = "Rectangle";
        case SpinnerType::Square:
            typeStr = "Square";
            break;
        case SpinnerType::Triangle:
            typeStr = "Triangle";
            break;
        case SpinnerType::Star:
            typeStr = "Star";
            break;
        }

        QString itemText = QString("%1 (%2)").arg(item->name, typeStr);
        QListWidgetItem *listItem = new QListWidgetItem(itemText, m_itemList);
        listItem->setData(Qt::UserRole, item->id);

        QPixmap colorPixmap(16, 16);
        colorPixmap.fill(QColor::fromString(item->color));
        listItem->setIcon(QIcon(colorPixmap));
    }
}

void MainWindow::updateItemProperties()
{
    if (m_selectedItemId == -1)
        return;

    m_updatingControls = true;

    QString name = m_canvas->getItemProperty(m_selectedItemId, "name").toString();
    QPointF pos = m_canvas->getItemProperty(m_selectedItemId, "position").toPointF();
    int type = m_canvas->getItemProperty(m_selectedItemId, "type").toInt();
    int anim = m_canvas->getItemProperty(m_selectedItemId, "anim").toInt();
    float speed = m_canvas->getItemProperty(m_selectedItemId, "speed").toFloat();
    int size = m_canvas->getItemProperty(m_selectedItemId, "size").toInt();
    float preDelay = m_canvas->getItemProperty(m_selectedItemId, "preDelay").toFloat();
    float duration = m_canvas->getItemProperty(m_selectedItemId, "duration").toFloat();
    float postDelay = m_canvas->getItemProperty(m_selectedItemId, "postDelay").toFloat();
    QColor color = QColor::fromString(m_canvas->getItemProperty(m_selectedItemId, "color").toString());

    m_nameEdit->setText(name);
    m_xSpinBox->setValue(qRound(pos.x())); 
    m_ySpinBox->setValue(qRound(pos.y())); 
    m_typeCombo->setCurrentIndex(type);
    m_animationCombo->setCurrentIndex(anim);
    m_speedSlider->setValue(static_cast<int>(speed));
    m_sizeSlider->setValue(size);
    m_preDelaySlider->setValue(static_cast<float>(preDelay));
    m_durationSlider->setValue(static_cast<float>(duration));
    m_postDelaySlider->setValue(static_cast<float>(postDelay));
    m_colorButton->setStyleSheet(QString("background-color: %1; border: 2px solid white;").arg(color.name()));

    m_speedLabel->setText(QString("%1%").arg(static_cast<int>(speed)));
    m_sizeLabel->setText(QString("%1px").arg(size));
    m_preDelayLabel->setText(QString("%1s").arg(preDelay, 0, 'f', preDelay < 1.0 ? 1 : 0));
    m_durationLabel->setText(QString("%1s").arg(duration, 0, 'f', duration < 1.0 ? 1 : 0));
    m_postDelayLabel->setText(QString("%1s").arg(postDelay, 0, 'f', postDelay < 1.0 ? 1 : 0));

    m_updatingControls = false;
}

void MainWindow::enableItemControls(bool enabled)
{
    m_propertiesGroup->setEnabled(enabled);
    m_removeButton->setEnabled(enabled);
}

void MainWindow::onPositionChanged()
{
    if (m_updatingControls || m_selectedItemId == -1)
        return;

    QPointF currentPos = m_canvas->getItemProperty(m_selectedItemId, "position").toPointF();

    QPointF newPos(
        m_xSpinBox->hasFocus() ? m_xSpinBox->value() : currentPos.x(),
        m_ySpinBox->hasFocus() ? m_ySpinBox->value() : currentPos.y());

    // Ensure values stay within 0-100 range
    newPos.setX(qBound(0.0, newPos.x(), 100.0));
    newPos.setY(qBound(0.0, newPos.y(), 100.0));

    m_canvas->setItemProperty(m_selectedItemId, "position", newPos);
}

void MainWindow::onPreDelayChanged(float value)
{
    if (m_updatingControls || m_selectedItemId == -1)
        return;

    m_canvas->setItemProperty(m_selectedItemId, "preDelay", value);
    m_preDelayLabel->setText(QString("%1s").arg(value, 0, 'f', value < 1.0 ? 1 : 0));
}

void MainWindow::onPostDelayChanged(float value)
{
    if (m_updatingControls || m_selectedItemId == -1)
        return;

    m_canvas->setItemProperty(m_selectedItemId, "postDelay", value);
    m_postDelayLabel->setText(QString("%1s").arg(value, 0, 'f', value < 1.0 ? 1 : 0));
}

void MainWindow::onDurationChanged(float value)
{
    if (m_updatingControls || m_selectedItemId == -1)
        return;

    m_canvas->setItemProperty(m_selectedItemId, "duration", value);
    m_durationLabel->setText(QString("%1s").arg(value, 0, 'f', value < 1.0 ? 1 : 0));
}

void MainWindow::onExploreTemplatesClicked()
{
    TemplateExplorerDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        int templateIndex = dialog.getSelectedTemplateIndex();
        if (templateIndex >= 0)
        {
            applyTemplate(templateIndex);
        }
    }
}

void MainWindow::applyTemplate(int templateIndex)
{
    if (m_updatingControls)
        return;

    const auto &templates = SpinnerTemplates::getTemplates();
    if (templateIndex >= 0 && templateIndex < static_cast<int>(templates.size()))
    {
        const auto &template_ = templates[templateIndex];

        m_canvas->clearAll();

        for (const auto &item : template_.items)
        {
            int id = m_canvas->addSpinner(
                item.type,
                item.anim,
                item.position,
                item.size,
                item.color,
                item.speed,
                item.duration,
                item.preDelay,
                item.postDelay);
            m_canvas->setItemProperty(id, "name", QString("%1 %2").arg(template_.name).arg(id));
        }

        // Ensure animation is running
        m_isAnimating = true;
        m_startStopButton->setText("⏸️ Pause");
        m_canvas->setAnimating(true);

        updateItemList();

        statusBar()->showMessage(QString("Applied template: %1 - %2").arg(template_.name, template_.description));
    }
}