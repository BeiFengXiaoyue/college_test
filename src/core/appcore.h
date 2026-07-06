#ifndef APPCORE_H
#define APPCORE_H

#include <QObject>
#include <QThreadPool>
#include "threadpoolmanager.h"
#include "configmanager.h"
#include "../database/databasemanager.h"

class AppCore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool initialized READ isInitialized NOTIFY initializedChanged)
    Q_PROPERTY(int maxThreadCount READ maxThreadCount WRITE setMaxThreadCount NOTIFY maxThreadCountChanged)

public:
    explicit AppCore(QObject *parent = nullptr);
    ~AppCore();

    void initialize();
    bool isInitialized() const;

    DatabaseManager *databaseManager();
    ThreadPoolManager *threadPoolManager();
    ConfigManager *configManager();

    int maxThreadCount() const;
    void setMaxThreadCount(int count);

signals:
    void initializedChanged();
    void maxThreadCountChanged();

private:
    bool m_initialized = false;
    DatabaseManager m_databaseManager;
    ThreadPoolManager m_threadPoolManager;
    ConfigManager m_configManager;
};

#endif // APPCORE_H
