#include "widgets/gridwidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug> // Added for debugging
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QColorDialog> // Added for color dialog
#include <QSpinBox> // Added for spin box
#include <QGridLayout> // Added for grid layout
#include <QHBoxLayout> // Added for horizontal box layout
#include <QVBoxLayout> // Added for vertical box layout
#include <QFrame> // Added for frame
#include <QComboBox> // Added for combo box
#include <QCheckBox> // Added for check box
#include <QFont> // Added for font
#include <QScrollArea> // Added for scroll area

GridWidget::GridWidget(QWidget *parent)
    : QWidget(parent)
    , m_gridSize(20)
    , m_gridOffsetX(0)
    , m_gridOffsetY(0)
    , m_isDragging(false)
    , m_isDraggingGraphics(false)
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
    m_graphicsView->setDragMode(QGraphicsView::NoDrag); // 设置为无拖拽模式，避免与网格拖拽冲突
    m_graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    m_graphicsView->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    
    // 设置图形场景大小
    m_graphicsScene->setSceneRect(0, 0, 2000, 2000);
    
    // 设置图形视图背景为透明
    m_graphicsView->setBackgroundBrush(Qt::transparent);
    m_graphicsView->setStyleSheet("QGraphicsView { border: none; background: transparent; }");
    
    // 设置图形场景背景为透明
    m_graphicsScene->setBackgroundBrush(Qt::transparent);
    
    // 设置图形视图不拦截鼠标事件，让GridWidget处理
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
    
    // 处理Ctrl+右键拖拽
    if (event->button() == Qt::RightButton && (event->modifiers() & Qt::ControlModifier)) {
        // Ctrl+右键：记录位置用于拖拽
        qDebug() << "Ctrl+右键按下，记录拖拽起始位置";
        m_dragStartPos = event->pos();
        m_lastMousePos = event->pos();
        m_isDragging = false; // 初始化拖拽状态
        event->accept();
        return;
    }
    
    // 处理左键添加图形
    if (event->button() == Qt::LeftButton && m_isAddingItem) {
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
        return;
    }
    
    // 处理左键图形选择
    if (event->button() == Qt::LeftButton && !m_isAddingItem) {
        // 检查是否点击了图形对象
        QPointF scenePos = m_graphicsView->mapToScene(event->pos());
        QGraphicsItem *item = m_graphicsScene->itemAt(scenePos, m_graphicsView->transform());
        
        if (item) {
            // 点击了图形对象，进行选择
            if (event->modifiers() & Qt::ControlModifier) {
                // Ctrl+左键：多选
                item->setSelected(!item->isSelected());
            } else {
                // 普通左键：单选
                m_graphicsScene->clearSelection();
                item->setSelected(true);
            }
            
            // 记录拖拽起始位置
            m_isDraggingGraphics = true;
            m_lastGraphicsPos = scenePos;
            
            qDebug() << "选择图形对象，选中数量:" << m_graphicsScene->selectedItems().size();
        } else {
            // 点击空白区域，取消选择
            m_graphicsScene->clearSelection();
            m_isDraggingGraphics = false;
            qDebug() << "取消选择";
        }
        event->accept();
        return;
    }
    
    // 其他鼠标事件传递给父类处理
    QWidget::mousePressEvent(event);
}

void GridWidget::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "GridWidget::mouseMoveEvent - buttons:" << event->buttons() 
             << "modifiers:" << event->modifiers() 
             << "isDragging:" << m_isDragging;
    
    // 处理Ctrl+右键网格拖拽
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
            return;
        }
    }
    

    
    // 处理左键图形拖拽
    if ((event->buttons() & Qt::LeftButton) && !m_isAddingItem && m_isDraggingGraphics) {
        QList<QGraphicsItem*> selectedItems = m_graphicsScene->selectedItems();
        if (!selectedItems.isEmpty()) {
            // 有选中的图形，进行拖拽
            QPointF scenePos = m_graphicsView->mapToScene(event->pos());
            
            if (m_lastGraphicsPos != scenePos) {
                QPointF delta = scenePos - m_lastGraphicsPos;
                for (QGraphicsItem *item : selectedItems) {
                    item->moveBy(delta.x(), delta.y());
                }
                m_lastGraphicsPos = scenePos;
                qDebug() << "拖拽图形，delta:" << delta;
            }
            event->accept();
            return;
        }
    }
    
    // 其他鼠标移动事件传递给父类处理
    QWidget::mouseMoveEvent(event);
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
    } else if (event->button() == Qt::LeftButton) {
        // 结束图形拖拽
        if (m_isDraggingGraphics) {
            qDebug() << "结束图形拖拽";
            m_isDraggingGraphics = false;
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
    circle->setFlag(QGraphicsItem::ItemIsMovable, false); // 禁用Qt的自动移动，我们手动处理
    circle->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    circle->setFlag(QGraphicsItem::ItemIsFocusable, false); // 不需要焦点
    circle->setAcceptHoverEvents(false); // 不需要悬停事件
    
    m_graphicsScene->addItem(circle);
}

void GridWidget::addRectangle(const QRectF &rect, const QColor &color)
{
    QGraphicsRectItem *rectangle = new QGraphicsRectItem(rect);
    rectangle->setBrush(QBrush(color));
    rectangle->setPen(QPen(Qt::black, 2));
    rectangle->setFlag(QGraphicsItem::ItemIsSelectable, true);
    rectangle->setFlag(QGraphicsItem::ItemIsMovable, false); // 禁用Qt的自动移动，我们手动处理
    rectangle->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    rectangle->setFlag(QGraphicsItem::ItemIsFocusable, false); // 不需要焦点
    rectangle->setAcceptHoverEvents(false); // 不需要悬停事件
    
    m_graphicsScene->addItem(rectangle);
}

void GridWidget::addLine(const QPointF &start, const QPointF &end, const QColor &color)
{
    QGraphicsLineItem *line = new QGraphicsLineItem(start.x(), start.y(), end.x(), end.y());
    line->setPen(QPen(color, 3));
    line->setFlag(QGraphicsItem::ItemIsSelectable, true);
    line->setFlag(QGraphicsItem::ItemIsMovable, false); // 禁用Qt的自动移动，我们手动处理
    line->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    line->setFlag(QGraphicsItem::ItemIsFocusable, false); // 不需要焦点
    line->setAcceptHoverEvents(false); // 不需要悬停事件
    
    m_graphicsScene->addItem(line);
}

void GridWidget::addText(const QPointF &pos, const QString &text, const QColor &color)
{
    QGraphicsTextItem *textItem = new QGraphicsTextItem(text);
    textItem->setPos(pos);
    textItem->setDefaultTextColor(color);
    textItem->setFlag(QGraphicsItem::ItemIsSelectable, true);
    textItem->setFlag(QGraphicsItem::ItemIsMovable, false); // 禁用Qt的自动移动，我们手动处理
    textItem->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    textItem->setFlag(QGraphicsItem::ItemIsFocusable, false); // 不需要焦点
    textItem->setAcceptHoverEvents(false); // 不需要悬停事件
    
    m_graphicsScene->addItem(textItem);
}

void GridWidget::addPolygon(const QPolygonF &polygon, const QColor &color)
{
    QGraphicsPolygonItem *polygonItem = new QGraphicsPolygonItem(polygon);
    polygonItem->setBrush(QBrush(color));
    polygonItem->setPen(QPen(Qt::black, 2));
    polygonItem->setFlag(QGraphicsItem::ItemIsSelectable, true);
    polygonItem->setFlag(QGraphicsItem::ItemIsMovable, false); // 禁用Qt的自动移动，我们手动处理
    polygonItem->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    polygonItem->setFlag(QGraphicsItem::ItemIsFocusable, false); // 不需要焦点
    polygonItem->setAcceptHoverEvents(false); // 不需要悬停事件
    
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
    } else if (event->key() == Qt::Key_D && event->modifiers() & Qt::ControlModifier) {
        // Ctrl+D：显示图形样式弹窗
        showGraphicsStyleDialog();
        event->accept();
    } else {
        QWidget::keyPressEvent(event);
    }
}

void GridWidget::showGraphicsStyleDialog()
{
    // 获取选中的图形
    QList<QGraphicsItem*> selectedItems = m_graphicsScene->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::information(this, "提示", "请先选择一个图形对象");
        return;
    }
    
    // 创建图形样式设置对话框
    QDialog dialog(this);
    dialog.setWindowTitle("图形样式设置");
    dialog.setFixedSize(1000, 700);
    dialog.setModal(true);
    
    // 设置对话框样式
    dialog.setStyleSheet("QDialog { "
                        "    background-color: #2b2b2b; "
                        "    border: 1px solid #404040; "
                        "    border-radius: 8px; "
                        "}");
    
    // 创建主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(25, 25, 25, 25);
    
    // 创建标题
    QLabel *titleLabel = new QLabel("图形样式设置", &dialog);
    titleLabel->setStyleSheet("QLabel { "
                             "    font-size: 14px; "
                             "    font-weight: 600; "
                             "    color: #e0e0e0; "
                             "    padding: 10px 0; "
                             "    border-bottom: 1px solid #404040; "
                             "}");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    // 创建滚动区域
    QScrollArea *scrollArea = new QScrollArea(&dialog);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { "
                             "    border: none; "
                             "    background-color: transparent; "
                             "} "
                             "QScrollBar:vertical { "
                             "    background-color: #404040; "
                             "    width: 12px; "
                             "    border-radius: 6px; "
                             "} "
                             "QScrollBar::handle:vertical { "
                             "    background-color: #666666; "
                             "    border-radius: 6px; "
                             "    min-height: 20px; "
                             "} "
                             "QScrollBar::handle:vertical:hover { "
                             "    background-color: #777777; "
                             "}");
    
    QWidget *scrollContent = new QWidget();
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContent);
    scrollLayout->setSpacing(25);
    scrollLayout->setContentsMargins(0, 0, 0, 0);
    
    // 颜色设置区域
    QFrame *colorFrame = new QFrame(&dialog);
    colorFrame->setStyleSheet("QFrame { "
                             "    background-color: #333333; "
                             "    border: 1px solid #404040; "
                             "    border-radius: 6px; "
                             "    padding: 5px; "
                             "}");
    
    QVBoxLayout *colorLayout = new QVBoxLayout(colorFrame);
    
    QLabel *colorTitle = new QLabel("颜色设置", colorFrame);
    colorTitle->setStyleSheet("QLabel { "
                             "    font-size: 12px; "
                             "    font-weight: 600; "
                             "    color: #e0e0e0; "
                             "    padding: 5px 0; "
                             "    border-bottom: 1px solid #404040; "
                             "}");
    colorTitle->setAlignment(Qt::AlignCenter);
    colorLayout->addWidget(colorTitle);
    
    // 将三个颜色设置放在同一行
    QHBoxLayout *colorRowLayout = new QHBoxLayout();
    colorRowLayout->setSpacing(20);
    
    // 背景颜色
    QHBoxLayout *bgLayout = new QHBoxLayout();
    bgLayout->setSpacing(10);
    
    QLabel *bgLabel = new QLabel("背景颜色", colorFrame);
    bgLabel->setStyleSheet("QLabel { "
                          "    font-size: 11px; "
                          "    color: #cccccc; "
                          "    min-width: 80px; "
                          "}");
    
    QPushButton *bgCustomBtn = new QPushButton("自定义", colorFrame);
    bgCustomBtn->setFixedSize(80, 28);
    bgCustomBtn->setStyleSheet("QPushButton { "
                              "    background-color: #4a7c59; "
                              "    border: 1px solid #5a8c69; "
                              "    border-radius: 4px; "
                              "    color: white; "
                              "    font-size: 11px; "
                              "} "
                              "QPushButton:hover { "
                              "    background-color: #5a8c69; "
                              "    border-color: #6a9c79; "
                              "} "
                              "QPushButton:pressed { "
                              "    background-color: #3a6c49; "
                              "}");
    
    bgLayout->addWidget(bgLabel);
    bgLayout->addWidget(bgCustomBtn);
    colorRowLayout->addLayout(bgLayout);
    
    // 边框颜色
    QHBoxLayout *borderColorLayout = new QHBoxLayout();
    borderColorLayout->setSpacing(10);
    
    QLabel *borderColorLabel = new QLabel("边框颜色", colorFrame);
    borderColorLabel->setStyleSheet("QLabel { "
                                   "    font-size: 11px; "
                                   "    color: #cccccc; "
                                   "    min-width: 80px; "
                                   "}");
    
    QPushButton *borderCustomBtn = new QPushButton("自定义", colorFrame);
    borderCustomBtn->setFixedSize(80, 28);
    borderCustomBtn->setStyleSheet("QPushButton { "
                                  "    background-color: #4a7c59; "
                                  "    border: 1px solid #5a8c69; "
                                  "    border-radius: 4px; "
                                  "    color: white; "
                                  "    font-size: 11px; "
                                  "} "
                                  "QPushButton:hover { "
                                  "    background-color: #5a8c69; "
                                  "    border-color: #6a9c79; "
                                  "} "
                                  "QPushButton:pressed { "
                                  "    background-color: #3a6c49; "
                                  "}");
    
    borderColorLayout->addWidget(borderColorLabel);
    borderColorLayout->addWidget(borderCustomBtn);
    colorRowLayout->addLayout(borderColorLayout);
    
    // 文本颜色
    QHBoxLayout *textColorLayout = new QHBoxLayout();
    textColorLayout->setSpacing(10);
    
    QLabel *textColorLabel = new QLabel("文本颜色", colorFrame);
    textColorLabel->setStyleSheet("QLabel { "
                                 "    font-size: 11px; "
                                 "    color: #cccccc; "
                                 "    min-width: 80px; "
                                 "}");
    
    QPushButton *textCustomBtn = new QPushButton("自定义", colorFrame);
    textCustomBtn->setFixedSize(80, 28);
    textCustomBtn->setStyleSheet("QPushButton { "
                                "    background-color: #4a7c59; "
                                "    border: 1px solid #5a8c69; "
                                "    border-radius: 4px; "
                                "    color: white; "
                                "    font-size: 11px; "
                                "} "
                                "QPushButton:hover { "
                                "    background-color: #5a8c69; "
                                "    border-color: #6a9c79; "
                                "} "
                                "QPushButton:pressed { "
                                "    background-color: #3a6c49; "
                                "}");
    
    textColorLayout->addWidget(textColorLabel);
    textColorLayout->addWidget(textCustomBtn);
    colorRowLayout->addLayout(textColorLayout);
    
    colorLayout->addLayout(colorRowLayout);
    scrollLayout->addWidget(colorFrame);
    
    // 创建边框设置区域
    QFrame *borderFrame = new QFrame(scrollContent);
    borderFrame->setStyleSheet("QFrame { "
                              "    background-color: #333333; "
                              "    border: 1px solid #404040; "
                              "    border-radius: 6px; "
                              "    padding: 5px; "
                              "}");
    
    QVBoxLayout *borderLayout = new QVBoxLayout(borderFrame);
    borderLayout->setSpacing(15);
    
    // 边框设置标题
    QLabel *borderTitleLabel = new QLabel("边框设置", borderFrame);
    borderTitleLabel->setStyleSheet("QLabel { "
                                   "    font-size: 12px; "
                                   "    font-weight: 800; "
                                   "    color: #e0e0e0; "
                                   "    padding: 5px 0; "
                                   "    border-bottom: 1px solid #404040; "
                                   "}");
    borderLayout->addWidget(borderTitleLabel);
    
    // 边框宽度设置
    QHBoxLayout *borderWidthLayout = new QHBoxLayout();
    QLabel *borderWidthLabel = new QLabel("边框宽度", borderFrame);
    borderWidthLabel->setStyleSheet("QLabel { "
                                   "    font-size: 11px; "
                                   "    color: #cccccc; "
                                   "    min-width: 80px; "
                                   "}");
    
    QSpinBox *borderWidthSpinBox = new QSpinBox(borderFrame);
    borderWidthSpinBox->setRange(1, 20);
    borderWidthSpinBox->setValue(2);
    borderWidthSpinBox->setFixedSize(80, 28);
    borderWidthSpinBox->setStyleSheet("QSpinBox { "
                                     "    background-color: #4a4a4a; "
                                     "    border: 1px solid #555555; "
                                     "    border-radius: 4px; "
                                     "    color: #e0e0e0; "
                                     "    font-size: 11px; "
                                     "    padding: 4px; "
                                     "} "
                                     "QSpinBox::up-button, QSpinBox::down-button { "
                                     "    background-color: #555555; "
                                     "    border: none; "
                                     "    border-radius: 2px; "
                                     "    width: 16px; "
                                     "} "
                                     "QSpinBox::up-button:hover, QSpinBox::down-button:hover { "
                                     "    background-color: #666666; "
                                     "}");
    
    borderWidthLayout->addWidget(borderWidthLabel);
    borderWidthLayout->addWidget(borderWidthSpinBox);
    borderWidthLayout->addStretch();
    borderLayout->addLayout(borderWidthLayout);
    
    scrollLayout->addWidget(borderFrame);
    
    // 创建字体设置区域
    QFrame *fontFrame = new QFrame(scrollContent);
    fontFrame->setStyleSheet("QFrame { "
                            "    background-color: #333333; "
                            "    border: 1px solid #404040; "
                            "    border-radius: 6px; "
                            "    padding: 5px; "
                            "}");
    
    QVBoxLayout *fontLayout = new QVBoxLayout(fontFrame);
    fontLayout->setSpacing(15);
    
    // 字体设置标题
    QLabel *fontTitleLabel = new QLabel("字体设置", fontFrame);
    fontTitleLabel->setStyleSheet("QLabel { "
                                 "    font-size: 12px; "
                                 "    font-weight: 600; "
                                 "    color: #e0e0e0; "
                                 "    padding: 5px 0; "
                                 "    border-bottom: 1px solid #404040; "
                                 "}");
    fontLayout->addWidget(fontTitleLabel);
    
    // 字体族设置
    QHBoxLayout *fontFamilyLayout = new QHBoxLayout();
    QLabel *fontFamilyLabel = new QLabel("字体族", fontFrame);
    fontFamilyLabel->setStyleSheet("QLabel { "
                                  "    font-size: 11px; "
                                  "    color: #cccccc; "
                                  "    min-width: 80px; "
                                  "}");
    
    QComboBox *fontFamilyCombo = new QComboBox(fontFrame);
    fontFamilyCombo->addItems({"Arial", "Times New Roman", "Courier New", "Verdana", "微软雅黑", "宋体", "黑体"});
    fontFamilyCombo->setFixedSize(120, 28);
    fontFamilyCombo->setStyleSheet("QComboBox { "
                                  "    background-color: #4a4a4a; "
                                  "    border: 1px solid #555555; "
                                  "    border-radius: 4px; "
                                  "    color: #e0e0e0; "
                                  "    font-size: 11px; "
                                  "    padding: 4px; "
                                  "} "
                                  "QComboBox::drop-down { "
                                  "    border: none; "
                                  "    width: 20px; "
                                  "} "
                                  "QComboBox::down-arrow { "
                                  "    image: none; "
                                  "    border-left: 5px solid transparent; "
                                  "    border-right: 5px solid transparent; "
                                  "    border-top: 5px solid #e0e0e0; "
                                  "} "
                                  "QComboBox QAbstractItemView { "
                                  "    background-color: #4a4a4a; "
                                  "    border: 1px solid #555555; "
                                  "    color: #e0e0e0; "
                                  "    selection-background-color: #5a5a5a; "
                                  "}");
    
    fontFamilyLayout->addWidget(fontFamilyLabel);
    fontFamilyLayout->addWidget(fontFamilyCombo);
    fontFamilyLayout->addStretch();
    fontLayout->addLayout(fontFamilyLayout);
    
    // 字体大小设置
    QHBoxLayout *fontSizeLayout = new QHBoxLayout();
    QLabel *fontSizeLabel = new QLabel("字体大小", fontFrame);
    fontSizeLabel->setStyleSheet("QLabel { "
                                "    font-size: 11px; "
                                "    color: #cccccc; "
                                "    min-width: 80px; "
                                "}");
    
    QSpinBox *fontSizeSpinBox = new QSpinBox(fontFrame);
    fontSizeSpinBox->setRange(8, 72);
    fontSizeSpinBox->setValue(12);
    fontSizeSpinBox->setFixedSize(80, 28);
    fontSizeSpinBox->setStyleSheet("QSpinBox { "
                                  "    background-color: #4a4a4a; "
                                  "    border: 1px solid #555555; "
                                  "    border-radius: 4px; "
                                  "    color: #e0e0e0; "
                                  "    font-size: 11px; "
                                  "    padding: 4px; "
                                  "} "
                                  "QSpinBox::up-button, QSpinBox::down-button { "
                                  "    background-color: #555555; "
                                  "    border: none; "
                                  "    border-radius: 2px; "
                                  "    width: 16px; "
                                  "} "
                                  "QSpinBox::up-button:hover, QSpinBox::down-button:hover { "
                                  "    background-color: #666666; "
                                  "}");
    
    fontSizeLayout->addWidget(fontSizeLabel);
    fontSizeLayout->addWidget(fontSizeSpinBox);
    fontSizeLayout->addStretch();
    fontLayout->addLayout(fontSizeLayout);
    
    // 字体样式设置
    QHBoxLayout *fontStyleLayout = new QHBoxLayout();
    QLabel *fontStyleLabel = new QLabel("字体样式", fontFrame);
    fontStyleLabel->setStyleSheet("QLabel { "
                                 "    font-size: 13px; "
                                 "    color: #cccccc; "
                                 "    min-width: 80px; "
                                 "}");
    
    QCheckBox *boldCheckBox = new QCheckBox("粗体", fontFrame);
    QCheckBox *italicCheckBox = new QCheckBox("斜体", fontFrame);
    QCheckBox *underlineCheckBox = new QCheckBox("下划线", fontFrame);
    
    QString checkBoxStyle = "QCheckBox { "
                           "    color: #cccccc; "
                           "    font-size: 12px; "
                           "    spacing: 8px; "
                           "} "
                           "QCheckBox::indicator { "
                           "    width: 16px; "
                           "    height: 16px; "
                           "    border: 1px solid #555555; "
                           "    border-radius: 3px; "
                           "    background-color: #4a4a4a; "
                           "} "
                           "QCheckBox::indicator:checked { "
                           "    background-color: #4a7c59; "
                           "    border-color: #5a8c69; "
                           "} "
                           "QCheckBox::indicator:checked::after { "
                           "    content: '✓'; "
                           "    color: white; "
                           "    font-size: 12px; "
                           "    font-weight: bold; "
                           "}";
    
    boldCheckBox->setStyleSheet(checkBoxStyle);
    italicCheckBox->setStyleSheet(checkBoxStyle);
    underlineCheckBox->setStyleSheet(checkBoxStyle);
    
    fontStyleLayout->addWidget(fontStyleLabel);
    fontStyleLayout->addWidget(boldCheckBox);
    fontStyleLayout->addWidget(italicCheckBox);
    fontStyleLayout->addWidget(underlineCheckBox);
    fontStyleLayout->addStretch();
    fontLayout->addLayout(fontStyleLayout);
    
    scrollLayout->addWidget(fontFrame);
    
    // 设置滚动区域内容
    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);
    
    // 按钮区域
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);
    
    QPushButton *applyBtn = new QPushButton("应用", &dialog);
    applyBtn->setFixedSize(100, 32);
    applyBtn->setStyleSheet("QPushButton { "
                           "    background-color: #4a7c59; "
                           "    border: 1px solid #5a8c69; "
                           "    border-radius: 6px; "
                           "    color: white; "
                           "    font-size: 11px; "
                           "    font-weight: 500; "
                           "} "
                           "QPushButton:hover { "
                           "    background-color: #5a8c69; "
                           "    border-color: #6a9c79; "
                           "} "
                           "QPushButton:pressed { "
                           "    background-color: #3a6c49; "
                           "}");
    
    QPushButton *cancelBtn = new QPushButton("取消", &dialog);
    cancelBtn->setFixedSize(100, 32);
    cancelBtn->setStyleSheet("QPushButton { "
                            "    background-color: #5a5a5a; "
                            "    border: 1px solid #666666; "
                            "    border-radius: 6px; "
                            "    color: #e0e0e0; "
                            "    font-size: 11px; "
                            "} "
                            "QPushButton:hover { "
                            "    background-color: #666666; "
                            "    border-color: #777777; "
                            "} "
                            "QPushButton:pressed { "
                            "    background-color: #4a4a4a; "
                            "}");
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(applyBtn);
    buttonLayout->addWidget(cancelBtn);
    mainLayout->addLayout(buttonLayout);
    
    // 存储选择的颜色和字体设置
    QColor selectedBgColor = Qt::transparent;
    QColor selectedBorderColor = Qt::black;
    QColor selectedTextColor = Qt::black;
    
    // 连接颜色选择按钮
    connect(bgCustomBtn, &QPushButton::clicked, [&]() {
        QColor customColor = QColorDialog::getColor(Qt::white, &dialog, "选择自定义背景颜色");
        if (customColor.isValid()) {
            selectedBgColor = customColor;
            bgCustomBtn->setText("已选择");
            bgCustomBtn->setStyleSheet(QString("QPushButton { "
                                            "    background-color: %1; "
                                            "    border: 1px solid %2; "
                                            "    border-radius: 4px; "
                                            "    color: white; "
                                            "    font-size: 11px; "
                                            "} "
                                            "QPushButton:hover { "
                                            "    background-color: %3; "
                                            "    border-color: %4; "
                                            "} "
                                            "QPushButton:pressed { "
                                            "    background-color: %5; "
                                            "}")
                                            .arg(customColor.name())
                                            .arg(customColor.darker(120).name())
                                            .arg(customColor.lighter(120).name())
                                            .arg(customColor.lighter(140).name())
                                            .arg(customColor.darker(140).name()));
        }
    });

    connect(borderCustomBtn, &QPushButton::clicked, [&]() {
        QColor customColor = QColorDialog::getColor(Qt::black, &dialog, "选择自定义边框颜色");
        if (customColor.isValid()) {
            selectedBorderColor = customColor;
            borderCustomBtn->setText("已选择");
            borderCustomBtn->setStyleSheet(QString("QPushButton { "
                                                "    background-color: %1; "
                                                "    border: 1px solid %2; "
                                                "    border-radius: 4px; "
                                                "    color: white; "
                                                "    font-size: 11px; "
                                                "} "
                                                "QPushButton:hover { "
                                                "    background-color: %3; "
                                                "    border-color: %4; "
                                                "} "
                                                "QPushButton:pressed { "
                                                "    background-color: %5; "
                                                "}")
                                                .arg(customColor.name())
                                                .arg(customColor.darker(120).name())
                                                .arg(customColor.lighter(120).name())
                                                .arg(customColor.lighter(140).name())
                                                .arg(customColor.darker(140).name()));
        }
    });
    
    connect(textCustomBtn, &QPushButton::clicked, [&]() {
        QColor customColor = QColorDialog::getColor(Qt::black, &dialog, "选择自定义文本颜色");
        if (customColor.isValid()) {
            selectedTextColor = customColor;
            textCustomBtn->setText("已选择");
            textCustomBtn->setStyleSheet(QString("QPushButton { "
                                              "    background-color: %1; "
                                              "    border: 1px solid %2; "
                                              "    border-radius: 4px; "
                                              "    color: white; "
                                              "    font-size: 11px; "
                                              "} "
                                              "QPushButton:hover { "
                                              "    background-color: %3; "
                                              "    border-color: %4; "
                                              "} "
                                              "QPushButton:pressed { "
                                              "    background-color: %5; "
                                              "}")
                                              .arg(customColor.name())
                                              .arg(customColor.darker(120).name())
                                              .arg(customColor.lighter(120).name())
                                              .arg(customColor.lighter(140).name())
                                              .arg(customColor.darker(140).name()));
        }
    });
    
    // 连接按钮信号
    connect(applyBtn, &QPushButton::clicked, [&]() {
        // 应用样式到选中的图形
        QList<QGraphicsItem*> selectedItems = m_graphicsScene->selectedItems();
        for (QGraphicsItem* item : selectedItems) {
            // 应用背景色
            if (selectedBgColor.isValid()) {
                if (QGraphicsEllipseItem* ellipse = dynamic_cast<QGraphicsEllipseItem*>(item)) {
                    ellipse->setBrush(selectedBgColor);
                } else if (QGraphicsRectItem* rect = dynamic_cast<QGraphicsRectItem*>(item)) {
                    rect->setBrush(selectedBgColor);
                } else if (QGraphicsPolygonItem* polygon = dynamic_cast<QGraphicsPolygonItem*>(item)) {
                    polygon->setBrush(selectedBgColor);
                }
            }
            
            // 应用边框色和宽度（只对支持pen的图形项）
            if (selectedBorderColor.isValid()) {
                if (QGraphicsEllipseItem* ellipse = dynamic_cast<QGraphicsEllipseItem*>(item)) {
                    QPen pen = ellipse->pen();
                    pen.setColor(selectedBorderColor);
                    pen.setWidth(borderWidthSpinBox->value());
                    ellipse->setPen(pen);
                } else if (QGraphicsRectItem* rect = dynamic_cast<QGraphicsRectItem*>(item)) {
                    QPen pen = rect->pen();
                    pen.setColor(selectedBorderColor);
                    pen.setWidth(borderWidthSpinBox->value());
                    rect->setPen(pen);
                } else if (QGraphicsLineItem* line = dynamic_cast<QGraphicsLineItem*>(item)) {
                    QPen pen = line->pen();
                    pen.setColor(selectedBorderColor);
                    pen.setWidth(borderWidthSpinBox->value());
                    line->setPen(pen);
                } else if (QGraphicsPolygonItem* polygon = dynamic_cast<QGraphicsPolygonItem*>(item)) {
                    QPen pen = polygon->pen();
                    pen.setColor(selectedBorderColor);
                    pen.setWidth(borderWidthSpinBox->value());
                    polygon->setPen(pen);
                }
            }
            
            // 应用文本颜色和字体
            if (QGraphicsTextItem* textItem = dynamic_cast<QGraphicsTextItem*>(item)) {
                if (selectedTextColor.isValid()) {
                    textItem->setDefaultTextColor(selectedTextColor);
                }
                
                QFont font = textItem->font();
                font.setFamily(fontFamilyCombo->currentText());
                font.setPointSize(fontSizeSpinBox->value());
                font.setBold(boldCheckBox->isChecked());
                font.setItalic(italicCheckBox->isChecked());
                font.setUnderline(underlineCheckBox->isChecked());
                textItem->setFont(font);
            }
        }
        dialog.accept();
    });
    
    connect(cancelBtn, &QPushButton::clicked, [&]() {
        dialog.reject();
    });
    
    // 显示对话框
    if (dialog.exec() == QDialog::Accepted) {
        qDebug() << "图形样式设置已应用";
    }
} 