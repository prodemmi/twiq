// Written by malekpour-dev.ir
// MainWindow is a QWidget-based class responsible for setting up the main window, its components, and their connections.

#pragma once

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QColorDialog>
#include <QTimer>
#include <QSplitter>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QListWidget>
#include <QLineEdit>
#include <QProgressDialog>
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QTextBlock>
#include <QListWidgetItem>
#include <QInputDialog>
#include <QScrollArea>
#include <gif_lib.h>
#include "CanvasWidget.h"
#include "SpinnerTemplates.h"
#include "TemplateExplorerDialog.h"

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Item management
    void onAddSpinnerClicked();
    void onRemoveSpinnerClicked();
    void onClearAllClicked();
    void onItemListItemClicked(QListWidgetItem* item);

    // Item properties
    void onSpinnerTypeChanged(int index);
    void onSpinnerAnimationChanged(int index);
    void onSpeedChanged(float value);
    void onSizeChanged(int value);
    void onColorChanged();
    void onNameChanged();
    void onPreDelayChanged(float value);
    void onPostDelayChanged(float value);
    void onDurationChanged(float value);
    void onExploreTemplatesClicked();

    // Animation
    void onStartStopClicked();
    void onExportClicked();
    void updateFrameRate();
    void updateAnimation();

    // Canvas events
    void onCanvasItemSelected(int id);
    void onCanvasItemDeselected();
    void onCanvasItemsChanged();

    void onPositionChanged();

private:
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupControlPanel();
    void setupItemList();
    void setupItemProperties();
    void setupCanvas();
    void connectSignals();
    void updateItemList();
    void updateItemProperties();
    void enableItemControls(bool enabled);
    void applyTemplate(int templateIndex);
    QImage captureFrame();
    bool exportGif(const QString &fileName, QProgressDialog &progress);

    // UI Components
    QSplitter *m_mainSplitter;
    QWidget *m_controlPanel;
    CanvasWidget *m_canvas;

    // Item List
    QGroupBox *m_itemListGroup;
    QListWidget *m_itemList;
    QPushButton *m_addButton;
    QPushButton *m_removeButton;
    QPushButton *m_clearAllButton;

    // Item Properties
    QGroupBox *m_propertiesGroup;
    QLineEdit *m_nameEdit;
    QComboBox *m_typeCombo;
    QComboBox *m_animationCombo;
    QSlider *m_speedSlider;
    QSlider *m_sizeSlider;
    QPushButton *m_colorButton;
    QSlider *m_preDelaySlider;
    QSlider *m_postDelaySlider;
    QLabel *m_preDelayLabel;
    QLabel *m_postDelayLabel;

    // Controls
    QPushButton *m_startStopButton;
    QPushButton *m_exportButton;
    QPushButton *m_exploreTemplatesButton;

    // Labels
    QLabel *m_speedLabel;
    QLabel *m_sizeLabel;

    // Timing
    QTimer *m_fpsTimer;
    QTimer *m_animationTimer;
    int m_frameCount;

    // Settings
    QColor m_currentColor;
    bool m_isAnimating;
    int m_selectedItemId;
    bool m_updatingControls;

    QSpinBox *m_xSpinBox;
    QSpinBox *m_ySpinBox;
    QSlider *m_durationSlider;
    QLabel *m_durationLabel;
};