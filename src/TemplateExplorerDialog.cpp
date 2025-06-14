// Written by malekpour-dev.ir
// TemplateExplorerDialog is a dialog for exploring and selecting spinner templates.
// It displays a grid of templates with previews and allows the user to select one.

#include "TemplateExplorerDialog.h"

TemplateExplorerDialog::TemplateExplorerDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Template Explorer");
    setFixedSize(600, 600);
    setupUI();
    QTimer::singleShot(0, this, &TemplateExplorerDialog::setupAnimationTimer);
}

TemplateExplorerDialog::~TemplateExplorerDialog()
{
    if (m_animationTimer)
    {
        m_animationTimer->stop();
        delete m_animationTimer;
    }

    
    for (auto *canvas : m_previewCanvases)
    {
        delete canvas;
    }
    m_previewCanvases.clear();
}

void TemplateExplorerDialog::onTemplateSelected(int templateIndex)
{
    m_selectedTemplateIndex = templateIndex;
    accept();
}

void TemplateExplorerDialog::updatePreviews()
{
    if (!isVisible())
        return;

    for (auto *canvas : m_previewCanvases)
    {
        if (canvas && canvas->isVisible())
        {
            canvas->updateAnimation();
        }
    }
}

void TemplateExplorerDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(8);
    mainLayout->setContentsMargins(8, 8, 8, 8);

    
    QLabel *headerLabel = new QLabel("Choose a Template");
    headerLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #333;");
    mainLayout->addWidget(headerLabel);

    
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setStyleSheet("QScrollArea { border: none; }");

    
    QWidget *contentWidget = new QWidget(scrollArea);
    m_gridLayout = new QGridLayout(contentWidget);
    m_gridLayout->setSpacing(12);
    m_gridLayout->setContentsMargins(2, 2, 2, 2);

    
    const auto &templates = SpinnerTemplates::getTemplates();
    m_templates = templates;

    int numCols = 3;
    
    int row = 0, col = 0;
    for (size_t i = 0; i < m_templates.size(); ++i)
    {
        const auto &template_ = m_templates[i];
        
        QWidget *previewContainer = new QWidget;
        previewContainer->setStyleSheet("background: white; border-radius: 6px;");
        QVBoxLayout *previewLayout = new QVBoxLayout(previewContainer);
        previewLayout->setSpacing(2);
        previewLayout->setContentsMargins(8, 8, 8, 8);

        QLabel *nameLabel = new QLabel(template_.name);
        nameLabel->setStyleSheet("font-weight: bold; font-size: 13px; color: #333;");
        previewLayout->addWidget(nameLabel);

        QLabel *descLabel = new QLabel(template_.description);
        descLabel->setWordWrap(true);
        descLabel->setStyleSheet("color: #666; font-size: 10px;");
        previewLayout->addWidget(descLabel);
        
        CanvasWidget *previewCanvas = new CanvasWidget(previewContainer);
        previewCanvas->setFixedSize(100, 100);
        previewCanvas->setStyleSheet("border: 1px solid #ddd; border-radius: 4px; background: #f8f8f8;");
        previewCanvas->setEnabled(false);
        m_previewCanvases.push_back(previewCanvas);

        
        for (const auto &item : template_.items)
        {
            previewCanvas->addSpinner(
                item.type,
                item.anim,
                item.position,
                item.size,
                item.color,
                item.speed,
                item.duration,
                item.preDelay,
                item.postDelay);
        }

        
        previewCanvas->setAnimating(true);
        previewLayout->addWidget(previewCanvas, 0, Qt::AlignCenter);

        
        QPushButton *applyButton = new QPushButton("Apply Template");
        applyButton->setStyleSheet(
            "QPushButton {"
            "    background: #4a90e2;"
            "    color: white;"
            "    border: none;"
            "    padding: 6px 12px;"
            "    border-radius: 4px;"
            "    font-weight: bold;"
            "    font-size: 11px;"
            "}"
            "QPushButton:hover {"
            "    background: #357abd;"
            "}"
            "QPushButton:pressed {"
            "    background: #2a5f9e;"
            "}");
        connect(applyButton, &QPushButton::clicked, [this, i]()
                { onTemplateSelected(i); });
        previewLayout->addWidget(applyButton);

        
        m_gridLayout->setColumnStretch(col, 1);
        m_gridLayout->addWidget(previewContainer, row, col);

        
        col++;
        if (col >= numCols)
        {
            col = 0;
            row++;
        }
    }

    
    scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(scrollArea);
}

void TemplateExplorerDialog::setupAnimationTimer()
{
    if (m_animationTimer)
    {
        m_animationTimer->stop();
        delete m_animationTimer;
    }

    m_animationTimer = new QTimer(this);
    connect(m_animationTimer, &QTimer::timeout, this, &TemplateExplorerDialog::updatePreviews);
    m_animationTimer->start(16); 
}