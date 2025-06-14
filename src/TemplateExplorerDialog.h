// Written by malekpour-dev.ir
// TemplateExplorerDialog is a dialog for exploring and selecting spinner templates.
// It displays a grid of templates with previews and allows the user to select one.

#pragma once

#include <QDialog>
#include <QGridLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include "CanvasWidget.h"
#include "SpinnerTemplates.h"

class TemplateExplorerDialog : public QDialog {
    Q_OBJECT

public:
    explicit TemplateExplorerDialog(QWidget *parent = nullptr);
    ~TemplateExplorerDialog();
    int getSelectedTemplateIndex() const { return m_selectedTemplateIndex; }

private slots:
    void onTemplateSelected(int templateIndex);
    void updatePreviews();

private:
    void setupUI();
    void setupAnimationTimer();

    int m_selectedTemplateIndex = -1;
    QTimer *m_animationTimer = nullptr;
    std::vector<CanvasWidget*> m_previewCanvases;
    QGridLayout *m_gridLayout = nullptr;
    std::vector<SpinnerTemplate> m_templates;
}; 