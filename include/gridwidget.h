#ifndef GRIDWIDGET_H
#define GRIDWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QTimer>

class GridWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GridWidget(QWidget *parent = nullptr);
    ~GridWidget();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawGrid(QPainter &painter);
    void updateGrid();

private:
    // 网格参数
    int m_gridSize;           // 网格大小
    int m_gridOffsetX;        // 网格X偏移
    int m_gridOffsetY;        // 网格Y偏移
    

    
    // 鼠标交互
    bool m_isDragging;        // 是否正在拖拽
    QPoint m_lastMousePos;    // 上次鼠标位置
    QPoint m_dragStartPos;    // 拖拽开始位置
    
    // 颜色
    QColor m_backgroundColor; // 背景色
    QColor m_gridColor;       // 网格线颜色
};

#endif // GRIDWIDGET_H 