#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QIcon>
#include <QDir>

#include "core/appcore.h"
#include "core/threadpoolmanager.h"
#include "services/authservice.h"
#include "services/courseservice.h"
#include "services/homeworkservice.h"
#include "services/livestreamservice.h"
#include "media/livestreammanager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setApplicationName("在线教育平台");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("EduTech");
    QQuickStyle::setStyle("Material");

    // 初始化核心模块
    AppCore appCore;
    appCore.initialize();

    // 初始化服务
    AuthService authService(appCore.databaseManager());
    CourseService courseService(appCore.databaseManager());
    HomeworkService homeworkService(appCore.databaseManager());
    LiveStreamService liveStreamService;

    // 初始化 QQmlApplicationEngine
    QQmlApplicationEngine engine;

    // 注册 C++ 单例到 QML 上下文
    QQmlContext *context = engine.rootContext();
    context->setContextProperty("AppCore", &appCore);
    context->setContextProperty("ThreadPoolManager", appCore.threadPoolManager());
    context->setContextProperty("AuthService", &authService);
    context->setContextProperty("CourseService", &courseService);
    context->setContextProperty("HomeworkService", &homeworkService);
    context->setContextProperty("LiveStreamService", &liveStreamService);

    // 注册可实例化的 C++ 类型到 QML
    qmlRegisterType<LiveStreamManager>("EduPlatform.Media", 1, 0, "LiveStreamManager");

    // 设置 QML 导入路径
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    engine.load(url);

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
