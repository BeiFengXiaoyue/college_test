#include "appcore.h"
#include <QDebug>

AppCore::AppCore(QObject *parent)
    : QObject(parent)
{
}

AppCore::~AppCore()
{
    m_databaseManager.close();
}

void AppCore::initialize()
{
    qDebug() << "[AppCore] Initializing application core...";

    // 1. 加载配置
    m_configManager.load();

    // 2. 初始化线程池
    int threadCount = m_configManager.value("threadPool/maxCount", QThreadPool::globalInstance()->maxThreadCount()).toInt();
    m_threadPoolManager.setMaxThreadCount(threadCount);
    qDebug() << "[AppCore] Thread pool initialized with max threads:" << threadCount;

    // 3. 初始化数据库
    QString dbPath = m_configManager.value("database/path", "edu_platform.db").toString();
    m_databaseManager.open(dbPath);
    m_databaseManager.initializeSchema();
    qDebug() << "[AppCore] Database initialized at:" << dbPath;

    m_initialized = true;
    emit initializedChanged();
    qDebug() << "[AppCore] Application core initialized successfully.";
}

bool AppCore::isInitialized() const
{
    return m_initialized;
}

DatabaseManager *AppCore::databaseManager()
{
    return &m_databaseManager;
}

ThreadPoolManager *AppCore::threadPoolManager()
{
    return &m_threadPoolManager;
}

ConfigManager *AppCore::configManager()
{
    return &m_configManager;
}

int AppCore::maxThreadCount() const
{
    return m_threadPoolManager.maxThreadCount();
}

void AppCore::setMaxThreadCount(int count)
{
    m_threadPoolManager.setMaxThreadCount(count);
    emit maxThreadCountChanged();
}
