#include "processmanager.h"
#include <QProcess>
#include <QTimer>
#include <QCoreApplication>
#include <QDebug>

ProcessManager::ProcessManager(QObject *parent)
    : QObject(parent)
{
}

void ProcessManager::restartApplication()
{
    QString appPath = QCoreApplication::applicationFilePath();
    QStringList args = QCoreApplication::arguments();

    qDebug() << "[ProcessManager] Restarting application:" << appPath;

    // 启动新进程（detached 模式：新进程独立于当前进程生命周期）
    if (!QProcess::startDetached(appPath, args.mid(1))) {
        QString errorMsg = "重启应用失败，请手动启动: " + appPath;
        qWarning() << "[ProcessManager]" << errorMsg;
        emit restartFailed(errorMsg);
        return;
    }

    qDebug() << "[ProcessManager] New process launched, quitting current instance.";
    emit restartStarted();

    // 延迟退出，确保 QML pending 事件（界面更新、导航）处理完毕后再退出
    QTimer::singleShot(0, this, []() {
        QCoreApplication::quit();
    });
}

void ProcessManager::quitApplication()
{
    qDebug() << "[ProcessManager] Quitting application.";
    QCoreApplication::quit();
}
