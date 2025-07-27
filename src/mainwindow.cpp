#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_gridWidget(nullptr)
{
    ui->setupUi(this);
    
    // 设置窗口标题
    setWindowTitle("网格页面 - 右键拖拽移动网格");
    
    // 创建网格组件
    m_gridWidget = new GridWidget(this);
    
    // 设置中央组件
    setCentralWidget(m_gridWidget);
    
    // 设置窗口大小
    resize(1000, 700);
}

MainWindow::~MainWindow()
{
    delete ui;
}
