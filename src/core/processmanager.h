#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <QObject>

class ProcessManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isRestarting READ isRestarting NOTIFY restartStarted)

public:
    explicit ProcessManager(QObject *parent = nullptr);

    bool isRestarting() const;

    Q_INVOKABLE void restartApplication();
    Q_INVOKABLE void quitApplication();

signals:
    void restartStarted();
    void restartFailed(const QString &error);

private:
    bool m_isRestarting = false;
};

#endif // PROCESSMANAGER_H
