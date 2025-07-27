#include "utils/settingsmanager.h"
#include <QDebug>
#include <QPoint>
#include <QRect>

// 静态成员初始化
SettingsManager* SettingsManager::s_instance = nullptr;
const QColor SettingsManager::DEFAULT_GRID_COLOR(48, 48, 48);
const QColor SettingsManager::DEFAULT_BACKGROUND_COLOR(20, 20, 20);
const QSize SettingsManager::DEFAULT_WINDOW_SIZE(1000, 700);

SettingsManager::SettingsManager(QObject *parent)
    : QObject(parent)
    , m_settings(new QSettings("ApplicationOMC", "GridApp", this))
{
    loadSettings();
}

SettingsManager::~SettingsManager()
{
    saveSettings();
}

SettingsManager* SettingsManager::instance()
{
    if (!s_instance) {
        s_instance = new SettingsManager();
    }
    return s_instance;
}

// 网格设置
void SettingsManager::setGridSize(int size)
{
    if (size > 0) {
        m_settings->setValue("Grid/size", size);
        emit settingsChanged();
    }
}

int SettingsManager::getGridSize() const
{
    return m_settings->value("Grid/size", DEFAULT_GRID_SIZE).toInt();
}

void SettingsManager::setGridOffset(int offsetX, int offsetY)
{
    m_settings->setValue("Grid/offsetX", offsetX);
    m_settings->setValue("Grid/offsetY", offsetY);
    emit settingsChanged();
}

QPoint SettingsManager::getGridOffset() const
{
    int x = m_settings->value("Grid/offsetX", 0).toInt();
    int y = m_settings->value("Grid/offsetY", 0).toInt();
    return QPoint(x, y);
}

void SettingsManager::setGridColor(const QColor &color)
{
    m_settings->setValue("Grid/color", color);
    emit settingsChanged();
}

QColor SettingsManager::getGridColor() const
{
    return m_settings->value("Grid/color", DEFAULT_GRID_COLOR).value<QColor>();
}

void SettingsManager::setBackgroundColor(const QColor &color)
{
    m_settings->setValue("Grid/backgroundColor", color);
    emit settingsChanged();
}

QColor SettingsManager::getBackgroundColor() const
{
    return m_settings->value("Grid/backgroundColor", DEFAULT_BACKGROUND_COLOR).value<QColor>();
}

// 窗口设置
void SettingsManager::setWindowGeometry(const QRect &geometry)
{
    m_settings->setValue("Window/geometry", QVariant::fromValue(geometry));
    emit settingsChanged();
}

QRect SettingsManager::getWindowGeometry() const
{
    return m_settings->value("Window/geometry").value<QRect>();
}

void SettingsManager::setWindowSize(const QSize &size)
{
    m_settings->setValue("Window/size", size);
    emit settingsChanged();
}

QSize SettingsManager::getWindowSize() const
{
    return m_settings->value("Window/size", DEFAULT_WINDOW_SIZE).toSize();
}

// 应用程序设置
void SettingsManager::setAutoSave(bool enabled)
{
    m_settings->setValue("App/autoSave", enabled);
    emit settingsChanged();
}

bool SettingsManager::getAutoSave() const
{
    return m_settings->value("App/autoSave", true).toBool();
}

void SettingsManager::setDebugMode(bool enabled)
{
    m_settings->setValue("App/debugMode", enabled);
    emit settingsChanged();
}

bool SettingsManager::getDebugMode() const
{
    return m_settings->value("App/debugMode", false).toBool();
}

// 保存和加载
void SettingsManager::saveSettings()
{
    m_settings->sync();
    if (getDebugMode()) {
        qDebug() << "设置已保存";
    }
}

void SettingsManager::loadSettings()
{
    if (getDebugMode()) {
        qDebug() << "设置已加载";
    }
}

void SettingsManager::resetToDefaults()
{
    m_settings->clear();
    
    // 设置默认值
    setGridSize(DEFAULT_GRID_SIZE);
    setGridColor(DEFAULT_GRID_COLOR);
    setBackgroundColor(DEFAULT_BACKGROUND_COLOR);
    setWindowSize(DEFAULT_WINDOW_SIZE);
    setAutoSave(true);
    setDebugMode(false);
    
    emit settingsChanged();
    
    if (getDebugMode()) {
        qDebug() << "设置已重置为默认值";
    }
} 