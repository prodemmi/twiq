// Written by malekpour-dev.ir
// CanvasWidget is a custom widget that displays and manages spinner items.

#pragma once

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QVariant>
#include <blend2d.h>
#include <vector>
#include <memory>

enum class SpinnerType
{
    Circle,
    Ring,
    Square,
    Rectangle,
    Triangle,
    Star
};

enum class SpinnerAnimation
{
    None,
    Rotate,
    Scale,
    Fade,
    Bounce,
    Slide
};

struct SpinnerItem
{
    int id;
    SpinnerType type;
    SpinnerAnimation anim;
    QPointF position;
    int size;
    QString color;
    float speed;
    float animationTime;
    bool isSelected;
    QString name;
    float duration;
    float preDelay;  // Delay before animation starts in seconds
    float postDelay; // Delay after animation completes in seconds

    SpinnerItem(int itemId, SpinnerType spinnerType, SpinnerAnimation anim, QPointF pos, int itemSize, QString itemColor,
                float itemSpeed, float itemDuration, float itemPreDelay = 0.0f, float itemPostDelay = 0.0f)
        : id(itemId), type(spinnerType), anim(anim), position(pos), size(itemSize), color(itemColor),
          speed(itemSpeed), animationTime(0.0f), duration(itemDuration),
          preDelay(itemPreDelay), postDelay(itemPostDelay), isSelected(false),
          name(QString("Spinner %1").arg(itemId)) {}
};

class CanvasWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CanvasWidget(QWidget *parent = nullptr);

    // Item management
    int addSpinner(SpinnerType type, SpinnerAnimation anim, QPointF position, int size, const QString &color,
                   float speed, float duration, float preDelay = 0.0f, float postDelay = 0.0f);

    void removeSpinner(int id);
    void clearAll();

    // Selection
    void selectItem(int id);
    void clearSelection();
    int getSelectedItemId() const;
    SpinnerItem *getSelectedItem();

    // Item properties
    void setItemProperty(int id, const QString &property, const QVariant &value);
    QVariant getItemProperty(int id, const QString &property);

    // Animation
    void setAnimationTime(double t);
    void setAnimating(bool animating);
    void updateAnimation();
    void resetAnimation();
    double getAnimationDuration() const;

    const std::vector<std::unique_ptr<SpinnerItem>> &getItems() const;

    // Helper functions
    int findItemAt(const QPointF &position) const;
    QRectF getItemBounds(const SpinnerItem &item) const;

    // Drawing
    void drawSpinner(BLContext &ctx, const SpinnerItem &item);

signals:
    void itemSelected(int id);
    void itemDeselected();
    void itemsChanged();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    void drawCircleSpinner(BLContext &ctx, const SpinnerItem &item);
    void drawRingSpinner(BLContext &ctx, const SpinnerItem &item);
    void drawRectangleSpinner(BLContext &ctx, const SpinnerItem &item);
    void drawSquareSpinner(BLContext &ctx, const SpinnerItem &item);
    void drawStarSpinner(BLContext &ctx, const SpinnerItem &item);
    void drawTriangleSpinner(BLContext &ctx, const SpinnerItem &item);
    void drawSelectionBox(BLContext &ctx, const SpinnerItem &item);

    void rotateAnimation(BLContext &ctx, const SpinnerItem &item);
    void scaleAnimation(BLContext &ctx, const SpinnerItem &item);
    void fadeAnimation(BLContext &ctx, const SpinnerItem &item);
    void bounceAnimation(BLContext &ctx, const SpinnerItem &item);
    void slideAnimation(BLContext &ctx, const SpinnerItem &item);

    std::vector<std::unique_ptr<SpinnerItem>> m_items;
    int m_nextId = 0;
    int m_selectedItemId = -1;
    bool m_isAnimating = false;
    bool m_isDragging = false;
    QPointF m_dragStartPos;
    QPointF m_lastMousePos;

    QMenu *m_contextMenu = nullptr;
    QAction *m_deleteAction = nullptr;
    QAction *m_duplicateAction = nullptr;
};
