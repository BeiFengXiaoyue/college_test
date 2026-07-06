#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <QObject>

class ProcessManager : public QObject
{
    Q_OBJECT

public:
    explicit ProcessManager(QObject *parent = nullptr);

    Q_INVOKABLE void restartApplication();
    Q_INVOKABLE void quitApplication();

signals:
    void restartStarted();
    void restartFailed(const QString &error);
};

#endif // PROCESSMANAGER_H
