#ifndef GRIDWIDGET_H
#define GRIDWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTimer>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QGraphicsPolygonItem>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QInputDialog>
#include <QMessageBox>

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
    void contextMenuEvent(QContextMenuEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:

private:
    void drawGrid(QPainter &painter);
    void updateGrid();
    void setupGraphicsUI();
    void setupGraphicsConnections();
    void setupContextMenu();
    void showGraphicsStyleDialog();
    void addCircle(const QPointF &center, qreal radius, const QColor &color = Qt::blue);
    void addRectangle(const QRectF &rect, const QColor &color = Qt::red);
    void addLine(const QPointF &start, const QPointF &end, const QColor &color = Qt::black);
    void addText(const QPointF &pos, const QString &text, const QColor &color = Qt::black);
    void addPolygon(const QPolygonF &polygon, const QColor &color = Qt::green);

private:
    // 网格参数
    int m_gridSize;           // 网格大小
    int m_gridOffsetX;        // 网格X偏移
    int m_gridOffsetY;        // 网格Y偏移
    
    // 鼠标交互
    bool m_isDragging;        // 是否正在拖拽
    QPoint m_lastMousePos;    // 上次鼠标位置
    QPoint m_dragStartPos;    // 拖拽开始位置
    
    // 图形拖拽
    bool m_isDraggingGraphics; // 是否正在拖拽图形
    QPointF m_lastGraphicsPos; // 上次图形拖拽位置
    
    // 颜色
    QColor m_backgroundColor; // 背景色
    QColor m_gridColor;       // 网格线颜色
    
    // 图形显示组件
    QGraphicsView *m_graphicsView;
    QGraphicsScene *m_graphicsScene;
    
    // 右键菜单
    QMenu *m_contextMenu;
    QAction *m_addCircleAction;
    QAction *m_addRectAction;
    QAction *m_addLineAction;
    QAction *m_addTextAction;
    QAction *m_addPolygonAction;
    QAction *m_removeAction;
    QAction *m_clearAction;
    
    
    qreal m_zoomFactor;
    
    // 状态
    bool m_isAddingItem;
    QString m_currentAddMode;
    QPointF m_startPoint;
    QPointF m_endPoint;

public slots:
    // 设置方法
    void setGridSize(int size);
    void setGridOffset(int offsetX, int offsetY);
    void setGridColor(const QColor &color);
    void setBackgroundColor(const QColor &color);
    
    // 获取方法
    int getGridSize() const { return m_gridSize; }
    QPoint getGridOffset() const { return QPoint(m_gridOffsetX, m_gridOffsetY); }
    QColor getGridColor() const { return m_gridColor; }
    QColor getBackgroundColor() const { return m_backgroundColor; }
};

#endif // GRIDWIDGET_H 