#include "processmanager.h"
#include <QProcess>
#include <QTimer>
#include <QCoreApplication>
#include <QDebug>

ProcessManager::ProcessManager(QObject *parent)
    : QObject(parent)
{
}

bool ProcessManager::isRestarting() const
{
    return m_isRestarting;
}

void ProcessManager::restartApplication()
{
    QString appPath = QCoreApplication::applicationFilePath();
    QString appDir  = QCoreApplication::applicationDirPath();

    qDebug() << "[ProcessManager] Restarting application:" << appPath;
    qDebug() << "[ProcessManager] Working directory:" << appDir;

    // 先设标志，阻止 QML 在重启过程中做不必要的页面切换
    m_isRestarting = true;

    // 启动新进程，显式指定工作目录为 exe 所在目录（确保找到 Qt DLL）
    bool launched = QProcess::startDetached(appPath, QStringList(), appDir);

    if (!launched) {
        QString errorMsg = "重启应用失败，请手动启动: " + appPath;
        qWarning() << "[ProcessManager]" << errorMsg;
        m_isRestarting = false;
        emit restartFailed(errorMsg);
        return;
    }

    qDebug() << "[ProcessManager] New process launched, quitting current instance.";
    emit restartStarted();

    // 延迟退出，确保 QML pending 事件处理完毕后再退出
    QTimer::singleShot(500, this, []() {
        QCoreApplication::quit();
    });
}

void ProcessManager::quitApplication()
{
    qDebug() << "[ProcessManager] Quitting application.";
    QCoreApplication::quit();
}
