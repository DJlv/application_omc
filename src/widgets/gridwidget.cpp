#include "widgets/gridwidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>

GridWidget::GridWidget(QWidget *parent)
    : QWidget(parent)
    , m_gridSize(20)
    , m_gridOffsetX(0)
    , m_gridOffsetY(0)
    , m_isDragging(false)
    , m_backgroundColor(20, 20, 20)    // 深灰色背景
    , m_gridColor(48, 48, 48)          // 浅灰色网格线
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    
    // 设置样式
    setStyleSheet("QWidget { border: none; }");
}

GridWidget::~GridWidget()
{
}

void GridWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);
    
    // 绘制背景
    painter.fillRect(rect(), m_backgroundColor);
    
    // 绘制网格
    drawGrid(painter);
}

void GridWidget::drawGrid(QPainter &painter)
{
    painter.setPen(QPen(m_gridColor, 1, Qt::SolidLine));
    
    int width = this->width();
    int height = this->height();
    
    // 计算网格起始位置
    int startX = m_gridOffsetX % m_gridSize;
    int startY = m_gridOffsetY % m_gridSize;
    
    // 绘制垂直线
    for (int x = startX; x < width; x += m_gridSize) {
        painter.drawLine(x, 0, x, height);
    }
    
    // 绘制水平线
    for (int y = startY; y < height; y += m_gridSize) {
        painter.drawLine(0, y, width, y);
    }
}

void GridWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        m_isDragging = true;
        m_dragStartPos = event->pos();
        m_lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
    }
}

void GridWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isDragging) {
        QPoint delta = event->pos() - m_lastMousePos;
        m_gridOffsetX += delta.x();
        m_gridOffsetY += delta.y();
        m_lastMousePos = event->pos();
        update();
        event->accept();
    }
}

void GridWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton && m_isDragging) {
        m_isDragging = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
    }
}

void GridWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    update();
}

void GridWidget::updateGrid()
{
    update();
}

// 设置方法实现
void GridWidget::setGridSize(int size)
{
    if (size > 0 && size != m_gridSize) {
        m_gridSize = size;
        update();
    }
}

void GridWidget::setGridOffset(int offsetX, int offsetY)
{
    if (m_gridOffsetX != offsetX || m_gridOffsetY != offsetY) {
        m_gridOffsetX = offsetX;
        m_gridOffsetY = offsetY;
        update();
    }
}

void GridWidget::setGridColor(const QColor &color)
{
    if (m_gridColor != color) {
        m_gridColor = color;
        update();
    }
}

void GridWidget::setBackgroundColor(const QColor &color)
{
    if (m_backgroundColor != color) {
        m_backgroundColor = color;
        update();
    }
} 