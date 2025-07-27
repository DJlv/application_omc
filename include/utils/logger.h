#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QMutex>

class Logger : public QObject
{
    Q_OBJECT

public:
    enum LogLevel {
        Debug,
        Info,
        Warning,
        Error,
        Fatal
    };

    explicit Logger(QObject *parent = nullptr);
    ~Logger();

    // 单例模式
    static Logger* instance();

    // 日志方法
    void log(LogLevel level, const QString &message);
    void debug(const QString &message);
    void info(const QString &message);
    void warning(const QString &message);
    void error(const QString &message);
    void fatal(const QString &message);

    // 设置
    void setLogFile(const QString &filePath);
    void setLogLevel(LogLevel level);
    void setConsoleOutput(bool enabled);

private:
    static Logger* s_instance;
    QFile m_logFile;
    QTextStream m_logStream;
    LogLevel m_logLevel;
    bool m_consoleOutput;
    QMutex m_mutex;

    QString levelToString(LogLevel level);
    QString getCurrentTimestamp();
};

// 便捷宏
#define LOG_DEBUG(msg) Logger::instance()->debug(msg)
#define LOG_INFO(msg) Logger::instance()->info(msg)
#define LOG_WARNING(msg) Logger::instance()->warning(msg)
#define LOG_ERROR(msg) Logger::instance()->error(msg)
#define LOG_FATAL(msg) Logger::instance()->fatal(msg)

#endif // LOGGER_H 