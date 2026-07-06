#ifndef THREADPOOLMANAGER_H
#define THREADPOOLMANAGER_H

#include <QObject>
#include <QThreadPool>
#include <QRunnable>
#include <QFuture>
#include <QtConcurrent>

class ThreadPoolManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int maxThreadCount READ maxThreadCount WRITE setMaxThreadCount NOTIFY maxThreadCountChanged)
    Q_PROPERTY(int activeThreadCount READ activeThreadCount NOTIFY activeThreadCountChanged)

public:
    explicit ThreadPoolManager(QObject *parent = nullptr);
    ~ThreadPoolManager();

    // 线程池配置
    int maxThreadCount() const;
    void setMaxThreadCount(int count);

    int activeThreadCount() const;

    // 提交任务
    void start(QRunnable *runnable, int priority = 0);
    template<typename Function>
    QFuture<typename QtPrivate::ResultOf<Function>::type> run(Function function);

    // 取消所有任务
    void clear();

signals:
    void maxThreadCountChanged();
    void activeThreadCountChanged();
    void taskStarted();
    void taskFinished();

private:
    QThreadPool *m_pool;
};

// 模板方法实现 (需要在头文件中)
template<typename Function>
QFuture<typename QtPrivate::ResultOf<Function>::type> ThreadPoolManager::run(Function function)
{
    auto future = QtConcurrent::run(m_pool, function);
    return future;
}

#endif // THREADPOOLMANAGER_H
