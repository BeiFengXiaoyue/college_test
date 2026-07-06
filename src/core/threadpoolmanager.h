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
    auto run(Function function) -> QFuture<std::invoke_result_t<Function>>
    {
        return QtConcurrent::run(m_pool, std::forward<Function>(function));
    }

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

#endif // THREADPOOLMANAGER_H
