#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QSettings>
#include <QColor>
#include <QSize>

class SettingsManager : public QObject
{
    Q_OBJECT

public:
    explicit SettingsManager(QObject *parent = nullptr);
    ~SettingsManager();

    // 单例模式
    static SettingsManager* instance();

    // 网格设置
    void setGridSize(int size);
    int getGridSize() const;
    
    void setGridOffset(int offsetX, int offsetY);
    QPoint getGridOffset() const;
    
    void setGridColor(const QColor &color);
    QColor getGridColor() const;
    
    void setBackgroundColor(const QColor &color);
    QColor getBackgroundColor() const;

    // 窗口设置
    void setWindowGeometry(const QRect &geometry);
    QRect getWindowGeometry() const;
    
    void setWindowSize(const QSize &size);
    QSize getWindowSize() const;

    // 应用程序设置
    void setAutoSave(bool enabled);
    bool getAutoSave() const;
    
    void setDebugMode(bool enabled);
    bool getDebugMode() const;

    // 保存和加载
    void saveSettings();
    void loadSettings();
    void resetToDefaults();

signals:
    void settingsChanged();

private:
    static SettingsManager* s_instance;
    QSettings *m_settings;
    
    // 默认值
    static const int DEFAULT_GRID_SIZE = 20;
    static const QColor DEFAULT_GRID_COLOR;
    static const QColor DEFAULT_BACKGROUND_COLOR;
    static const QSize DEFAULT_WINDOW_SIZE;
};

#endif // SETTINGSMANAGER_H 