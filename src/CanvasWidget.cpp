// Written by malekpour-dev.ir
// CanvasWidget is a custom widget that displays and manages spinner items.

#include "CanvasWidget.h"
#include <QContextMenuEvent>
#include <cmath>

CanvasWidget::CanvasWidget(QWidget *parent)
    : QWidget(parent), m_nextId(1), m_selectedItemId(-1), m_isAnimating(false),
      m_isDragging(false)
{
    setMouseTracking(true);
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::white);
    setPalette(pal);

    
    m_contextMenu = new QMenu(this);
    m_deleteAction = m_contextMenu->addAction("Delete Item");
    m_duplicateAction = m_contextMenu->addAction("Duplicate Item");

    connect(m_deleteAction, &QAction::triggered, [this]()
            {
        if (m_selectedItemId != -1) {
            removeSpinner(m_selectedItemId);
        } });

    connect(m_duplicateAction, &QAction::triggered, [this]()
            {
        if (m_selectedItemId != -1) {
            auto *item = getSelectedItem();
            if (item) {
                  addSpinner(item->type, item->anim, item->position, item->size, item->color, item->speed, item->duration, item->preDelay, item->postDelay);
            }
        } });
}

int CanvasWidget::addSpinner(SpinnerType type, SpinnerAnimation anim, QPointF percentPosition, int size, const QString &color,
                             float speed, float duration, float preDelay, float postDelay)
{
    QSize canvasSize = this->size();
    QPointF pixelPosition(
        canvasSize.width() * percentPosition.x() / 100.0,
        canvasSize.height() * percentPosition.y() / 100.0);

    int newSize = canvasSize.width() * size / 100.0;

    auto item = std::make_unique<SpinnerItem>(m_nextId++, type, anim, pixelPosition, newSize, color, speed, duration, preDelay, postDelay);
    int id = item->id;
    m_items.push_back(std::move(item));

    emit itemsChanged();
    update();
    return id;
}

void CanvasWidget::removeSpinner(int id)
{
    auto it = std::find_if(m_items.begin(), m_items.end(),
                           [id](const std::unique_ptr<SpinnerItem> &item)
                           {
                               return item->id == id;
                           });

    if (it != m_items.end())
    {
        if (m_selectedItemId == id)
        {
            m_selectedItemId = -1;
            emit itemDeselected();
        }
        m_items.erase(it);
        emit itemsChanged();
        update();
    }
}

void CanvasWidget::clearAll()
{
    m_items.clear();
    m_selectedItemId = -1;
    m_isAnimating = false; 
    emit itemsChanged();
    emit itemDeselected();
    update();
}

void CanvasWidget::selectItem(int id)
{
    
    for (auto &item : m_items)
    {
        item->isSelected = false;
    }

    
    auto it = std::find_if(m_items.begin(), m_items.end(),
                           [id](const std::unique_ptr<SpinnerItem> &item)
                           {
                               return item->id == id;
                           });

    if (it != m_items.end())
    {
        (*it)->isSelected = true;
        m_selectedItemId = id;
        emit itemSelected(id);
    }

    update();
}

void CanvasWidget::clearSelection()
{
    for (auto &item : m_items)
    {
        item->isSelected = false;
    }
    m_selectedItemId = -1;
    emit itemDeselected();
    update();
}

SpinnerItem *CanvasWidget::getSelectedItem()
{
    auto it = std::find_if(m_items.begin(), m_items.end(),
                           [this](const std::unique_ptr<SpinnerItem> &item)
                           {
                               return item->id == m_selectedItemId;
                           });
    return (it != m_items.end()) ? it->get() : nullptr;
}

void CanvasWidget::setItemProperty(int id, const QString &property, const QVariant &value)
{
    auto it = std::find_if(m_items.begin(), m_items.end(),
                           [id](const std::unique_ptr<SpinnerItem> &item)
                           {
                               return item->id == id;
                           });

    if (it != m_items.end())
    {
        SpinnerItem *item = it->get();
        bool shouldResetAnimation = false;

        if (property == "size")
        {
            item->size = value.toInt();
        }
        else if (property == "color")
        {
            item->color = value.value<QString>();
        }
        else if (property == "speed")
        {
            item->speed = value.toFloat();
            shouldResetAnimation = true;
        }
        else if (property == "position")
        {
            QPointF percentPos = value.toPointF();
            QSize canvasSize = this->size();
            QPointF pixelPos(
                canvasSize.width() * percentPos.x() / 100.0,
                canvasSize.height() * percentPos.y() / 100.0);
            item->position = pixelPos;
        }
        else if (property == "type")
        {
            item->type = static_cast<SpinnerType>(value.toInt());
            shouldResetAnimation = true;
        }
        else if (property == "anim")
        {
            item->anim = static_cast<SpinnerAnimation>(value.toInt());
            shouldResetAnimation = true;
        }
        else if (property == "name")
        {
            item->name = value.toString();
        }
        else if (property == "duration")
        {
            item->duration = value.toFloat();
            shouldResetAnimation = true;
        }
        else if (property == "preDelay")
        {
            item->preDelay = value.toFloat();
            shouldResetAnimation = true;
        }
        else if (property == "postDelay")
        {
            item->postDelay = value.toFloat();
            shouldResetAnimation = true;
        }

        if (shouldResetAnimation)
        {
            resetAnimation();
        }
        else
        {
            update();
        }
    }
}

QVariant CanvasWidget::getItemProperty(int id, const QString &property)
{
    auto it = std::find_if(m_items.begin(), m_items.end(),
                           [id](const std::unique_ptr<SpinnerItem> &item)
                           {
                               return item->id == id;
                           });

    if (it != m_items.end())
    {
        SpinnerItem *item = it->get();

        if (property == "size")
            return item->size;
        if (property == "color")
            return item->color;
        if (property == "speed")
            return item->speed;
        if (property == "position")
        {
            QSize canvasSize = this->size();
            QPointF percentPos(
                item->position.x() * 100.0 / canvasSize.width(),
                item->position.y() * 100.0 / canvasSize.height());
            return percentPos;
        }
        if (property == "type")
            return static_cast<int>(item->type);
        if (property == "anim")
            return static_cast<int>(item->anim);
        if (property == "name")
            return item->name;
        if (property == "duration")
            return item->duration;
        if (property == "preDelay")
            return item->preDelay;
        if (property == "postDelay")
            return item->postDelay;
    }

    return QVariant();
}

void CanvasWidget::setAnimating(bool animating)
{
    if (m_isAnimating == animating)
        return;

    m_isAnimating = animating;
    if (!animating)
    {
        update();
    }
}

void CanvasWidget::updateAnimation()
{
    if (!m_isAnimating || !isVisible())
        return;

    bool needsUpdate = false;
    for (auto &item : m_items)
    {
        if (!item)
            continue;

        float totalCycleTime = item->preDelay + item->duration + item->postDelay;
        if (totalCycleTime <= 0)
            continue;

        item->animationTime += (item->speed / 100) * 0.04f; 

        
        if (item->animationTime >= totalCycleTime)
        {
            item->animationTime = 0.0f;
        }
        needsUpdate = true;
    }

    if (needsUpdate)
    {
        update();
    }
}

void CanvasWidget::resetAnimation()
{
    for (auto &item : m_items)
    {
        item->animationTime = 0.0f;
    }
    update();
}

void CanvasWidget::setAnimationTime(double t)
{
    for (auto &item : m_items)
    {
        item->animationTime = t;
    }
    update();
}

double CanvasWidget::getAnimationDuration() const
{
    
    double maxEnd = 0.0;
    for (const auto &item : m_items)
    {
        double end = item->preDelay + item->duration + item->postDelay;
        if (end > maxEnd)
            maxEnd = end;
    }
    return maxEnd;
}

void CanvasWidget::paintEvent(QPaintEvent *)
{
    if (!isVisible())
        return;

    QImage image(size(), QImage::Format_ARGB32);
    image.fill(Qt::white); 

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

    
    for (const auto &item : m_items)
    {
        if (!item)
            continue;
        drawSpinner(ctx, *item);

        
        if (item->isSelected)
        {
            drawSelectionBox(ctx, *item);
        }
    }

    ctx.end();

    QPainter painter(this);
    painter.drawImage(0, 0, image);
}

void CanvasWidget::drawSpinner(BLContext &ctx, const SpinnerItem &item)
{
    ctx.save();
    ctx.translate(item.position.x(), item.position.y());

    
    QColor c = QColor::fromString(item.color);
    BLRgba32 color(c.red(), c.green(), c.blue(), c.alpha());
    ctx.setFillStyle(color);
    ctx.setStrokeStyle(color);

    
    switch (item.anim)
    {
    case SpinnerAnimation::None:
        break;
    case SpinnerAnimation::Rotate:
        rotateAnimation(ctx, item);
        break;
    case SpinnerAnimation::Scale:
        scaleAnimation(ctx, item);
        break;
    case SpinnerAnimation::Fade:
        fadeAnimation(ctx, item);
        break;
    case SpinnerAnimation::Bounce:
        bounceAnimation(ctx, item);
        break;
    case SpinnerAnimation::Slide:
        slideAnimation(ctx, item);
        break;
    }

    
    switch (item.type)
    {
    case SpinnerType::Circle:
        drawCircleSpinner(ctx, item);
        break;
    case SpinnerType::Ring:
        drawRingSpinner(ctx, item);
        break;
    case SpinnerType::Rectangle:
        drawRectangleSpinner(ctx, item);
        break;
    case SpinnerType::Square:
        drawSquareSpinner(ctx, item);
        break;
    case SpinnerType::Star:
        drawStarSpinner(ctx, item);
        break;
    case SpinnerType::Triangle:
        drawTriangleSpinner(ctx, item);
        break;
    }

    ctx.restore();
}

void CanvasWidget::drawCircleSpinner(BLContext &ctx, const SpinnerItem &item)
{
    ctx.fillCircle(0, 0, item.size / 2.0f);
}

void CanvasWidget::drawRingSpinner(BLContext &ctx, const SpinnerItem &item)
{
    double outerRadius = item.size / 2.0f;
    double innerRadius = outerRadius * 0.6;

    
    ctx.fillCircle(0, 0, outerRadius);

    
    ctx.setCompOp(BL_COMP_OP_DST_OUT);
    ctx.fillCircle(0, 0, innerRadius);
    ctx.setCompOp(BL_COMP_OP_SRC_OVER);
}

void CanvasWidget::drawRectangleSpinner(BLContext &ctx, const SpinnerItem &item)
{
    double w = item.size;
    double h = item.size;

    QColor c = item.color;
    BLRgba32 dotColor(c.red(), c.green(), c.blue(), c.alpha());
    ctx.setFillStyle(dotColor);

    ctx.fillRect(-w / 2.0, -h / 2.0, w, h);
}

void CanvasWidget::drawSquareSpinner(BLContext &ctx, const SpinnerItem &item)
{
    double w = item.size;
    double h = item.size;

    QColor c = item.color;
    BLRgba32 dotColor(c.red(), c.green(), c.blue(), c.alpha());
    ctx.setFillStyle(dotColor);

    ctx.fillRect(-w / 2.0, -h / 2.0, w, h);
}

void CanvasWidget::drawStarSpinner(BLContext &ctx, const SpinnerItem &item)
{
    const int numPoints = 5;
    const double outerRadius = item.size;
    const double innerRadius = outerRadius * 0.4;

    BLPoint pts[numPoints * 2];

    for (int i = 0; i < numPoints * 2; ++i)
    {
        double angle = i * M_PI / numPoints;
        double radius = (i % 2 == 0) ? outerRadius : innerRadius;
        pts[i] = BLPoint(radius * std::cos(angle - M_PI / 2),
                         radius * std::sin(angle - M_PI / 2));
    }

    ctx.fillPolygon(pts, numPoints * 2);
}

void CanvasWidget::drawTriangleSpinner(BLContext &ctx, const SpinnerItem &item)
{
    double size = item.size;

    BLPoint pts[3] = {
        BLPoint(0, -size / 2.0),
        BLPoint(size / 2.0, size / 2.0),
        BLPoint(-size / 2.0, size / 2.0)};

    QColor c = QColor::fromString(item.color);
    BLRgba32 color(c.red(), c.green(), c.blue(), c.alpha());
    ctx.setFillStyle(color);
    ctx.fillPolygon(pts, 3);
}

void CanvasWidget::drawSelectionBox(BLContext &ctx, const SpinnerItem &item)
{
    
    double baseSize = item.size;
    double padding = 10.0; 

    
    double actualSize = baseSize;
    if (item.anim == SpinnerAnimation::Scale)
    {
        float totalCycleTime = item.preDelay + item.duration + item.postDelay;
        float cyclePosition = fmod(item.animationTime, totalCycleTime);
        if (cyclePosition >= item.preDelay && cyclePosition < (item.preDelay + item.duration))
        {
            float normalizedTime = (cyclePosition - item.preDelay) / item.duration;
            float scale = 0.5f + 0.5f * (1.0f + std::sin(normalizedTime * 2.0f * M_PI));
            actualSize = baseSize * scale;
        }
    }
    
    switch (item.type)
    {
    case SpinnerType::Star:
        actualSize = std::max(actualSize, baseSize * 1.2);
        break;
    case SpinnerType::Triangle:
        actualSize = std::max(actualSize, baseSize * 1.1);
        break;        
        break;
    }

    
    double finalSize = actualSize + padding;

    
    BLRgba32 selectionColor(100, 150, 255, 128);
    ctx.setStrokeStyle(selectionColor);
    ctx.setStrokeWidth(2.0);

    ctx.strokeRect(item.position.x() - finalSize / 2,
                   item.position.y() - finalSize / 2,
                   finalSize,
                   finalSize);
}

void CanvasWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_dragStartPos = event->position();
        m_lastMousePos = event->position();

        int itemId = findItemAt(event->position());
        if (itemId != -1)
        {
            selectItem(itemId);
            m_isDragging = true;
        }
        else
        {
            clearSelection();
        }
    }
}

void CanvasWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isDragging && m_selectedItemId != -1)
    {
        QPointF delta = event->position() - m_lastMousePos;
        auto *item = getSelectedItem();
        if (item)
        {
            item->position += delta;
            update();
        }
    }

    m_lastMousePos = event->position();
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_isDragging = false;
    }
}

void CanvasWidget::contextMenuEvent(QContextMenuEvent *event)
{
    if (m_selectedItemId != -1)
    {
        m_contextMenu->exec(event->globalPos());
    }
}

int CanvasWidget::findItemAt(const QPointF &position) const
{
    
    for (auto it = m_items.rbegin(); it != m_items.rend(); ++it)
    {
        QRectF bounds = getItemBounds(**it);
        if (bounds.contains(position))
        {
            return (*it)->id;
        }
    }
    return -1;
}

QRectF CanvasWidget::getItemBounds(const SpinnerItem &item) const
{
    float size = item.size;
    float halfSize = size / 2.0f;
    return QRectF(item.position.x() - halfSize,
                  item.position.y() - halfSize,
                  size, size);
}

void CanvasWidget::rotateAnimation(BLContext &ctx, const SpinnerItem &item)
{
    float totalCycleTime = item.preDelay + item.duration + item.postDelay;
    float cyclePosition = fmod(item.animationTime, totalCycleTime);

    if (cyclePosition >= item.preDelay && cyclePosition < (item.preDelay + item.duration))
    {
        float normalizedTime = (cyclePosition - item.preDelay) / item.duration;
        ctx.rotate(normalizedTime * 2.0f * M_PI);
    }
}

void CanvasWidget::scaleAnimation(BLContext &ctx, const SpinnerItem &item)
{
    float totalCycleTime = item.preDelay + item.duration + item.postDelay;
    float cyclePosition = fmod(item.animationTime, totalCycleTime);

    if (cyclePosition >= item.preDelay && cyclePosition < (item.preDelay + item.duration))
    {
        float normalizedTime = (cyclePosition - item.preDelay) / item.duration;
        float scale = 0.5f + 0.5f * (1.0f + std::sin(normalizedTime * 2.0f * M_PI));
        ctx.scale(scale);
    }
}

void CanvasWidget::fadeAnimation(BLContext &ctx, const SpinnerItem &item)
{
    float totalCycleTime = item.preDelay + item.duration + item.postDelay;
    float cyclePosition = fmod(item.animationTime, totalCycleTime);

    if (cyclePosition >= item.preDelay && cyclePosition < (item.preDelay + item.duration))
    {
        float normalizedTime = (cyclePosition - item.preDelay) / item.duration;
        
        float alpha = 0.1f + 0.9f * (0.5f * (1.0f + std::sin(normalizedTime * 2.0f * M_PI)));

        
        QColor baseColor = QColor::fromString(item.color);

        
        BLRgba32 fadeColor(
            baseColor.red(),
            baseColor.green(),
            baseColor.blue(),
            static_cast<uint8_t>(alpha * 255.0f));

        
        ctx.setFillStyle(fadeColor);
        ctx.setStrokeStyle(fadeColor);
    }
}

void CanvasWidget::bounceAnimation(BLContext &ctx, const SpinnerItem &item)
{
    float totalCycleTime = item.preDelay + item.duration + item.postDelay;
    float cyclePosition = fmod(item.animationTime, totalCycleTime);

    if (cyclePosition >= item.preDelay && cyclePosition < (item.preDelay + item.duration))
    {
        float normalizedTime = (cyclePosition - item.preDelay) / item.duration;

        
        float bounceHeight = 20.0f; 
        float verticalPos = std::abs(std::sin(normalizedTime * M_PI)) * bounceHeight;

        
        float verticalVelocity = std::cos(normalizedTime * M_PI);
        float scaleX = 1.0f + 0.2f * std::abs(verticalVelocity); 
        float scaleY = 1.0f - 0.2f * std::abs(verticalVelocity); 

        
        ctx.translate(0, -verticalPos); 
        ctx.scale(scaleX, scaleY);
    }
}

void CanvasWidget::slideAnimation(BLContext &ctx, const SpinnerItem &item)
{
    float totalCycleTime = item.preDelay + item.duration + item.postDelay;
    float cyclePosition = fmod(item.animationTime, totalCycleTime);

    if (cyclePosition >= item.preDelay && cyclePosition < (item.preDelay + item.duration))
    {
        float normalizedTime = (cyclePosition - item.preDelay) / item.duration;
        float offset = 20.0f * std::sin(normalizedTime * 2.0f * M_PI);
        ctx.translate(offset, 0);
    }
}

const std::vector<std::unique_ptr<SpinnerItem>> &CanvasWidget::getItems() const
{
    return m_items;
}
