#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>
#include <QMainWindow>
#include "widgets/gridwidget.h"

class AppController : public QObject
{
    Q_OBJECT

public:
    explicit AppController(QMainWindow *mainWindow, QObject *parent = nullptr);
    ~AppController();

    // 初始化应用程序
    void initialize();
    
    // 获取网格组件
    GridWidget* getGridWidget() const { return m_gridWidget; }

private slots:
    // 处理应用程序事件
    void onApplicationStateChanged(Qt::ApplicationState state);

private:
    void setupUI();
    void setupConnections();
    void loadSettings();
    void saveSettings();

private:
    QMainWindow *m_mainWindow;
    GridWidget *m_gridWidget;
};

#endif // APPCONTROLLER_H 