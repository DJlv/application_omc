#include "utils/logger.h"
#include <QDateTime>
#include <QDir>
#include <QDebug>
#include <QStandardPaths>

// 静态成员初始化
Logger* Logger::s_instance = nullptr;

Logger::Logger(QObject *parent)
    : QObject(parent)
    , m_logLevel(Info)
    , m_consoleOutput(true)
{
    // 创建日志目录
    QString logDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ApplicationOMC/logs";
    QDir().mkpath(logDir);
    
    // 设置日志文件
    QString logFile = logDir + "/app_" + QDateTime::currentDateTime().toString("yyyyMMdd") + ".log";
    setLogFile(logFile);
}

Logger::~Logger()
{
    if (m_logFile.isOpen()) {
        m_logFile.close();
    }
}

Logger* Logger::instance()
{
    if (!s_instance) {
        s_instance = new Logger();
    }
    return s_instance;
}

void Logger::log(LogLevel level, const QString &message)
{
    if (level < m_logLevel) {
        return;
    }

    QMutexLocker locker(&m_mutex);
    
    QString timestamp = getCurrentTimestamp();
    QString levelStr = levelToString(level);
    QString logMessage = QString("[%1] [%2] %3").arg(timestamp, levelStr, message);
    
    // 写入文件
    if (m_logFile.isOpen()) {
        m_logStream << logMessage << Qt::endl;
        m_logStream.flush();
    }
    
    // 控制台输出
    if (m_consoleOutput) {
        qDebug().noquote() << logMessage;
    }
}

void Logger::debug(const QString &message)
{
    log(Debug, message);
}

void Logger::info(const QString &message)
{
    log(Info, message);
}

void Logger::warning(const QString &message)
{
    log(Warning, message);
}

void Logger::error(const QString &message)
{
    log(Error, message);
}

void Logger::fatal(const QString &message)
{
    log(Fatal, message);
}

void Logger::setLogFile(const QString &filePath)
{
    QMutexLocker locker(&m_mutex);
    
    if (m_logFile.isOpen()) {
        m_logFile.close();
    }
    
    m_logFile.setFileName(filePath);
    if (m_logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        m_logStream.setDevice(&m_logFile);
        // Qt6中移除了setCodec方法，默认使用UTF-8
    }
}

void Logger::setLogLevel(LogLevel level)
{
    m_logLevel = level;
}

void Logger::setConsoleOutput(bool enabled)
{
    m_consoleOutput = enabled;
}

QString Logger::levelToString(LogLevel level)
{
    switch (level) {
    case Debug:   return "DEBUG";
    case Info:    return "INFO";
    case Warning: return "WARN";
    case Error:   return "ERROR";
    case Fatal:   return "FATAL";
    default:      return "UNKNOWN";
    }
}

QString Logger::getCurrentTimestamp()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
} 