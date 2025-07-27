#include "core/mainwindow.h"
#include "ui_mainwindow.h"
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_appController(nullptr)
{
    ui->setupUi(this);
    
    // 创建应用程序控制器
    m_appController = new AppController(this, this);
    
    // 初始化应用程序
    m_appController->initialize();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // 保存设置
    if (m_appController) {
        // 设置管理器会自动保存
    }
    
    event->accept();
}
