#include "widgets/gridwidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug> // Added for debugging

GridWidget::GridWidget(QWidget *parent)
    : QWidget(parent)
    , m_gridSize(20)
    , m_gridOffsetX(0)
    , m_gridOffsetY(0)
    , m_isDragging(false)
    , m_backgroundColor(20, 20, 20)    // 深灰色背景
    , m_gridColor(48, 48, 48)          // 浅灰色网格线
    , m_graphicsView(nullptr)
    , m_graphicsScene(nullptr)
    , m_contextMenu(nullptr)
    , m_addCircleAction(nullptr)
    , m_addRectAction(nullptr)
    , m_addLineAction(nullptr)
    , m_addTextAction(nullptr)
    , m_addPolygonAction(nullptr)
    , m_removeAction(nullptr)
    , m_clearAction(nullptr)
    , m_zoomFactor(1.0)
    , m_isAddingItem(false)
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    
    // 设置样式
    setStyleSheet("QWidget { border: none; }");
    
    // 设置图形显示界面
    setupGraphicsUI();
    setupGraphicsConnections();
    setupContextMenu();
}

GridWidget::~GridWidget()
{
}

void GridWidget::setupGraphicsUI()
{
    // 创建图形场景和视图
    m_graphicsScene = new QGraphicsScene(this);
    m_graphicsView = new QGraphicsView(m_graphicsScene);
    m_graphicsView->setRenderHint(QPainter::Antialiasing, true);
    m_graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    m_graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_graphicsView->setDragMode(QGraphicsView::NoDrag); // 设置为无拖拽模式，单击选择
    m_graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    m_graphicsView->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    
    // 设置图形场景大小
    m_graphicsScene->setSceneRect(0, 0, 2000, 2000);
    
    // 设置图形视图背景为透明
    m_graphicsView->setBackgroundBrush(Qt::transparent);
    m_graphicsView->setStyleSheet("QGraphicsView { border: none; background: transparent; }");
    
    // 设置图形场景背景为透明
    m_graphicsScene->setBackgroundBrush(Qt::transparent);
    
    // 设置图形视图不拦截鼠标事件
    m_graphicsView->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    m_graphicsView->viewport()->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    
    // 创建主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_graphicsView);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    setLayout(mainLayout);
}

void GridWidget::setupGraphicsConnections()
{
    // 设置图形视图不处理右键事件，让它们传递给父窗口
    m_graphicsView->setContextMenuPolicy(Qt::NoContextMenu);
    
    // 确保图形视图不会拦截鼠标事件
    m_graphicsView->setMouseTracking(true);
    m_graphicsView->setFocusPolicy(Qt::NoFocus);
    
    // 保持图形视图透明于鼠标事件，让事件传递给GridWidget
    m_graphicsView->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    m_graphicsView->viewport()->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    
    // 设置图形场景不处理右键事件
    m_graphicsScene->setItemIndexMethod(QGraphicsScene::NoIndex);
}

void GridWidget::setupContextMenu()
{
    m_contextMenu = new QMenu(this);
    
    m_addCircleAction = new QAction("添加圆形", this);
    m_addRectAction = new QAction("添加矩形", this);
    m_addLineAction = new QAction("添加直线", this);
    m_addTextAction = new QAction("添加文本", this);
    m_addPolygonAction = new QAction("添加多边形", this);
    m_removeAction = new QAction("删除选中", this);
    m_clearAction = new QAction("清空所有", this);
    
    m_contextMenu->addAction(m_addCircleAction);
    m_contextMenu->addAction(m_addRectAction);
    m_contextMenu->addAction(m_addLineAction);
    m_contextMenu->addAction(m_addTextAction);
    m_contextMenu->addAction(m_addPolygonAction);
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(m_removeAction);
    m_contextMenu->addAction(m_clearAction);
    
    // 连接右键菜单动作
    connect(m_addCircleAction, &QAction::triggered, [this]() {
        m_isAddingItem = true;
        m_currentAddMode = "circle";
        setCursor(Qt::CrossCursor);
    });
    
    connect(m_addRectAction, &QAction::triggered, [this]() {
        m_isAddingItem = true;
        m_currentAddMode = "rectangle";
        setCursor(Qt::CrossCursor);
    });
    
    connect(m_addLineAction, &QAction::triggered, [this]() {
        m_isAddingItem = true;
        m_currentAddMode = "line";
        m_startPoint = QPointF();
        setCursor(Qt::CrossCursor);
    });
    
    connect(m_addTextAction, &QAction::triggered, [this]() {
        m_isAddingItem = true;
        m_currentAddMode = "text";
        setCursor(Qt::CrossCursor);
    });
    
    connect(m_addPolygonAction, &QAction::triggered, [this]() {
        // 添加一个简单的三角形作为示例
        QPolygonF triangle;
        triangle << QPointF(0, -30) << QPointF(-25, 25) << QPointF(25, 25);
        addPolygon(triangle, Qt::green);
    });
    
    connect(m_removeAction, &QAction::triggered, [this]() {
        QList<QGraphicsItem*> selectedItems = m_graphicsScene->selectedItems();
        for (QGraphicsItem *item : selectedItems) {
            m_graphicsScene->removeItem(item);
            delete item;
        }
    });
    
    connect(m_clearAction, &QAction::triggered, [this]() {
        int result = QMessageBox::question(this, "确认清空", 
                                         "确定要清空所有图形吗？", 
                                         QMessageBox::Yes | QMessageBox::No);
        if (result == QMessageBox::Yes) {
            m_graphicsScene->clear();
        }
    });
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
    // 网格绘制功能保留，但主要用于背景
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
    qDebug() << "GridWidget::mousePressEvent - button:" << event->button() 
             << "modifiers:" << event->modifiers() 
             << "pos:" << event->pos();
    
    // 处理所有鼠标事件，包括在图形视图上的事件
    if (event->button() == Qt::RightButton) {
        if (event->modifiers() & Qt::ControlModifier) {
            // Ctrl+右键：记录位置用于拖拽
            qDebug() << "Ctrl+右键按下，记录拖拽起始位置";
            m_dragStartPos = event->pos();
            m_lastMousePos = event->pos();
            m_isDragging = false; // 初始化拖拽状态
        }
        // 移除普通右键显示菜单的功能
        event->accept();
    } else if (event->button() == Qt::LeftButton && m_isAddingItem) {
        // 处理左键添加图形
        // 由于QGraphicsView是透明的，我们需要直接计算场景坐标
        QPoint widgetPos = event->pos();
        QPointF scenePos = m_graphicsView->mapToScene(widgetPos);
        
        qDebug() << "添加图形 - 鼠标位置:" << widgetPos << "场景位置:" << scenePos;
        
        if (m_currentAddMode == "circle") {
            addCircle(scenePos, 30, Qt::blue);
        } else if (m_currentAddMode == "rectangle") {
            addRectangle(QRectF(scenePos.x() - 25, scenePos.y() - 20, 50, 40), Qt::red);
        } else if (m_currentAddMode == "line") {
            if (m_startPoint.isNull()) {
                m_startPoint = scenePos;
            } else {
                addLine(m_startPoint, scenePos, Qt::black);
                m_startPoint = QPointF();
            }
        } else if (m_currentAddMode == "text") {
            bool ok;
            QString text = QInputDialog::getText(this, "添加文本", "请输入文本:", QLineEdit::Normal, "", &ok);
            if (ok && !text.isEmpty()) {
                addText(scenePos, text, Qt::black);
            }
        }
        
        m_isAddingItem = false;
        m_currentAddMode.clear();
        setCursor(Qt::ArrowCursor);
        event->accept();
    }
}

void GridWidget::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "GridWidget::mouseMoveEvent - buttons:" << event->buttons() 
             << "modifiers:" << event->modifiers() 
             << "isDragging:" << m_isDragging;
    
    if ((event->buttons() & Qt::RightButton) && (event->modifiers() & Qt::ControlModifier)) {
        // 检查是否已经移动了足够的距离来开始拖拽
        QPoint dragDistance = event->pos() - m_dragStartPos;
        int dragThreshold = 5; // 5像素的拖拽阈值
        
        qDebug() << "Ctrl+右键移动，拖拽距离:" << dragDistance.manhattanLength() 
                 << "阈值:" << dragThreshold;
        
        if (!m_isDragging && dragDistance.manhattanLength() > dragThreshold) {
            // 开始拖拽
            qDebug() << "开始拖拽网格";
            m_isDragging = true;
            setCursor(Qt::ClosedHandCursor);
        }
        
        if (m_isDragging) {
            QPoint delta = event->pos() - m_lastMousePos;
            qDebug() << "拖拽中，delta:" << delta;
            m_gridOffsetX += delta.x();
            m_gridOffsetY += delta.y();
            m_lastMousePos = event->pos();
            
            // 移动图形场景中的所有图形对象
            QList<QGraphicsItem*> items = m_graphicsScene->items();
            for (QGraphicsItem *item : items) {
                item->moveBy(delta.x(), delta.y());
            }
            
            update();
            event->accept();
        }
    }
}

void GridWidget::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << "GridWidget::mouseReleaseEvent - button:" << event->button() 
             << "isDragging:" << m_isDragging;
    
    if (event->button() == Qt::RightButton) {
        if (m_isDragging) {
            // 结束拖拽
            qDebug() << "结束拖拽";
            m_isDragging = false;
            setCursor(Qt::ArrowCursor);
        }
        event->accept();
    }
}

void GridWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    
    // 更新图形场景大小以匹配窗口大小
    if (m_graphicsScene) {
        m_graphicsScene->setSceneRect(0, 0, width(), height());
    }
    
    update();
}

void GridWidget::contextMenuEvent(QContextMenuEvent *event)
{
    // 右键菜单现在在mousePressEvent中处理
    Q_UNUSED(event)
}

bool GridWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_graphicsView->viewport()) {
        if (event->type() == QEvent::MouseButtonPress || 
            event->type() == QEvent::MouseButtonRelease || 
            event->type() == QEvent::MouseMove) {
            
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            
            // 将所有鼠标事件转发给GridWidget处理
            QMouseEvent *newEvent = new QMouseEvent(
                event->type(),
                mouseEvent->pos(),
                mouseEvent->globalPos(),
                mouseEvent->button(),
                mouseEvent->buttons(),
                mouseEvent->modifiers()
            );
            QApplication::postEvent(this, newEvent);
            return true; // 阻止事件继续传播
        }
    }
    
    return QWidget::eventFilter(obj, event);
}

void GridWidget::addCircle(const QPointF &center, qreal radius, const QColor &color)
{
    QGraphicsEllipseItem *circle = new QGraphicsEllipseItem(center.x() - radius, center.y() - radius, radius * 2, radius * 2);
    circle->setBrush(QBrush(color));
    circle->setPen(QPen(Qt::black, 2));
    circle->setFlag(QGraphicsItem::ItemIsSelectable, true);
    circle->setFlag(QGraphicsItem::ItemIsMovable, true);
    circle->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    circle->setFlag(QGraphicsItem::ItemIsFocusable, false); // 不需要焦点
    // 设置图形对象不处理右键事件
    circle->setAcceptHoverEvents(false);
    
    m_graphicsScene->addItem(circle);
}

void GridWidget::addRectangle(const QRectF &rect, const QColor &color)
{
    QGraphicsRectItem *rectangle = new QGraphicsRectItem(rect);
    rectangle->setBrush(QBrush(color));
    rectangle->setPen(QPen(Qt::black, 2));
    rectangle->setFlag(QGraphicsItem::ItemIsSelectable, true);
    rectangle->setFlag(QGraphicsItem::ItemIsMovable, true);
    rectangle->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    rectangle->setFlag(QGraphicsItem::ItemIsFocusable, false); // 不需要焦点
    // 设置图形对象不处理右键事件
    rectangle->setAcceptHoverEvents(false);
    
    m_graphicsScene->addItem(rectangle);
}

void GridWidget::addLine(const QPointF &start, const QPointF &end, const QColor &color)
{
    QGraphicsLineItem *line = new QGraphicsLineItem(start.x(), start.y(), end.x(), end.y());
    line->setPen(QPen(color, 3));
    line->setFlag(QGraphicsItem::ItemIsSelectable, true);
    line->setFlag(QGraphicsItem::ItemIsMovable, true);
    line->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    line->setFlag(QGraphicsItem::ItemIsFocusable, false); // 不需要焦点
    // 设置图形对象不处理右键事件
    line->setAcceptHoverEvents(false);
    
    m_graphicsScene->addItem(line);
}

void GridWidget::addText(const QPointF &pos, const QString &text, const QColor &color)
{
    QGraphicsTextItem *textItem = new QGraphicsTextItem(text);
    textItem->setPos(pos);
    textItem->setDefaultTextColor(color);
    textItem->setFlag(QGraphicsItem::ItemIsSelectable, true);
    textItem->setFlag(QGraphicsItem::ItemIsMovable, true);
    textItem->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    textItem->setFlag(QGraphicsItem::ItemIsFocusable, false); // 不需要焦点
    // 设置图形对象不处理右键事件
    textItem->setAcceptHoverEvents(false);
    
    m_graphicsScene->addItem(textItem);
}

void GridWidget::addPolygon(const QPolygonF &polygon, const QColor &color)
{
    QGraphicsPolygonItem *polygonItem = new QGraphicsPolygonItem(polygon);
    polygonItem->setBrush(QBrush(color));
    polygonItem->setPen(QPen(Qt::black, 2));
    polygonItem->setFlag(QGraphicsItem::ItemIsSelectable, true);
    polygonItem->setFlag(QGraphicsItem::ItemIsMovable, true);
    polygonItem->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    polygonItem->setFlag(QGraphicsItem::ItemIsFocusable, false); // 不需要焦点
    // 设置图形对象不处理右键事件
    polygonItem->setAcceptHoverEvents(false);
    
    m_graphicsScene->addItem(polygonItem);
}



void GridWidget::updateGrid()
{
    update();
}

// 设置方法
void GridWidget::setGridSize(int size)
{
    if (size > 0 && size != m_gridSize) {
        m_gridSize = size;
        update();
    }
}

void GridWidget::setGridOffset(int offsetX, int offsetY)
{
    if (offsetX != m_gridOffsetX || offsetY != m_gridOffsetY) {
        m_gridOffsetX = offsetX;
        m_gridOffsetY = offsetY;
        update();
    }
}

void GridWidget::setGridColor(const QColor &color)
{
    if (color != m_gridColor) {
        m_gridColor = color;
        update();
    }
}

void GridWidget::setBackgroundColor(const QColor &color)
{
    if (color != m_backgroundColor) {
        m_backgroundColor = color;
        update();
    }
} 

void GridWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_S && event->modifiers() & Qt::ControlModifier) {
        // Ctrl+S：显示右键菜单
        QPoint cursorPos = QCursor::pos();
        m_contextMenu->exec(cursorPos);
        event->accept();
    } else {
        QWidget::keyPressEvent(event);
    }
} 