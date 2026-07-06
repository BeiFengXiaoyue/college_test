#include "threadpoolmanager.h"
#include <QDebug>

ThreadPoolManager::ThreadPoolManager(QObject *parent)
    : QObject(parent)
    , m_pool(QThreadPool::globalInstance())
{
    connect(m_pool, &QThreadPool::activeThreadCountChanged,
            this, &ThreadPoolManager::activeThreadCountChanged);
}

ThreadPoolManager::~ThreadPoolManager()
{
}

int ThreadPoolManager::maxThreadCount() const
{
    return m_pool->maxThreadCount();
}

void ThreadPoolManager::setMaxThreadCount(int count)
{
    if (count != m_pool->maxThreadCount()) {
        m_pool->setMaxThreadCount(count);
        emit maxThreadCountChanged();
        qDebug() << "[ThreadPoolManager] Max thread count set to:" << count;
    }
}

int ThreadPoolManager::activeThreadCount() const
{
    return m_pool->activeThreadCount();
}

void ThreadPoolManager::start(QRunnable *runnable, int priority)
{
    m_pool->start(runnable, priority);
    emit taskStarted();
}

void ThreadPoolManager::clear()
{
    m_pool->clear();
}
