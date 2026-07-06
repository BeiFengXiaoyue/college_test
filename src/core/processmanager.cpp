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
    QString appDir  = QCoreApplication::applicationDirPath();

    qDebug() << "[ProcessManager] Restarting application:" << appPath;
    qDebug() << "[ProcessManager] Working directory:" << appDir;

    // 启动新进程，显式指定工作目录为 exe 所在目录（确保找到 Qt DLL）
    bool launched = QProcess::startDetached(appPath, QStringList(), appDir);

    if (!launched) {
        QString errorMsg = "重启应用失败，请手动启动: " + appPath;
        qWarning() << "[ProcessManager]" << errorMsg;
        emit restartFailed(errorMsg);
        // 重启失败时不退出，让用户继续使用当前实例
        return;
    }

    qDebug() << "[ProcessManager] New process launched, quitting current instance.";
    emit restartStarted();

    // 延迟退出，确保 QML pending 事件（界面更新、导航）处理完毕后再退出
    QTimer::singleShot(500, this, []() {
        QCoreApplication::quit();
    });
}

void ProcessManager::quitApplication()
{
    qDebug() << "[ProcessManager] Quitting application.";
    QCoreApplication::quit();
}
