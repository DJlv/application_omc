#include "controllers/appcontroller.h"
#include <QApplication>
#include <QSettings>
#include <QMessageBox>
#include <QDebug>

AppController::AppController(QMainWindow *mainWindow, QObject *parent)
    : QObject(parent)
    , m_mainWindow(mainWindow)
    , m_gridWidget(nullptr)
{
}

AppController::~AppController()
{
    saveSettings();
}

void AppController::initialize()
{
    setupUI();
    setupConnections();
    loadSettings();
}

void AppController::setupUI()
{
    if (!m_mainWindow) {
        qWarning() << "MainWindow is null!";
        return;
    }

    // 设置窗口标题
    m_mainWindow->setWindowTitle("Application OMC - 网格图形显示");
    
    // 创建网格组件（已集成图形显示功能）
    m_gridWidget = new GridWidget(m_mainWindow);
    
    // 设置中央组件
    m_mainWindow->setCentralWidget(m_gridWidget);
    
    // 设置窗口大小
    m_mainWindow->resize(1000, 700);
    
    // 设置窗口图标（如果有的话）
    // m_mainWindow->setWindowIcon(QIcon(":/icons/app_icon.png"));
}

void AppController::setupConnections()
{
    if (!m_mainWindow) return;
    
    // 连接应用程序状态变化信号
    connect(qApp, &QApplication::applicationStateChanged,
            this, &AppController::onApplicationStateChanged);
}

void AppController::loadSettings()
{
    QSettings settings("ApplicationOMC", "GridApp");
    
    // 加载窗口几何信息
    QRect geometry = settings.value("MainWindow/geometry").toRect();
    if (geometry.isValid()) {
        m_mainWindow->setGeometry(geometry);
    }
    
    // 加载网格设置
    if (m_gridWidget) {
        int gridSize = settings.value("Grid/size", 20).toInt();
        int offsetX = settings.value("Grid/offsetX", 0).toInt();
        int offsetY = settings.value("Grid/offsetY", 0).toInt();
        
        // 这里可以添加设置网格参数的方法
        // m_gridWidget->setGridSize(gridSize);
        // m_gridWidget->setGridOffset(offsetX, offsetY);
    }
}

void AppController::saveSettings()
{
    QSettings settings("ApplicationOMC", "GridApp");
    
    // 保存窗口几何信息
    if (m_mainWindow) {
        settings.setValue("MainWindow/geometry", m_mainWindow->geometry());
    }
    
    // 保存网格设置
    if (m_gridWidget) {
        // 这里可以添加获取网格参数的方法
        // settings.setValue("Grid/size", m_gridWidget->getGridSize());
        // settings.setValue("Grid/offsetX", m_gridWidget->getGridOffsetX());
        // settings.setValue("Grid/offsetY", m_gridWidget->getGridOffsetY());
    }
}

void AppController::onApplicationStateChanged(Qt::ApplicationState state)
{
    switch (state) {
    case Qt::ApplicationActive:
        qDebug() << "应用程序激活";
        break;
    case Qt::ApplicationInactive:
        qDebug() << "应用程序非激活";
        break;
    case Qt::ApplicationHidden:
        qDebug() << "应用程序隐藏";
        break;
    case Qt::ApplicationSuspended:
        qDebug() << "应用程序挂起";
        break;
    }
} 