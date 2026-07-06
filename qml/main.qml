import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "./theme"

/* ============================================================
   在线教育平台 - 主入口
   ============================================================ */
ApplicationWindow {
    id: appWindow
    visible: true
    width: 1280
    height: 800
    minimumWidth: 960
    minimumHeight: 600
    title: "在线教育平台"

    // 全局字体
    font.family: "Microsoft YaHei, PingFang SC, sans-serif"

    // 页面管理
    readonly property bool isLoggedIn: AuthService.loggedIn
    readonly property bool isTeacher: AuthService.isTeacher

    // 当前页面栈
    property var pageStack: []

    // 导航到指定页面
    function navigateTo(page, properties) {
        mainLoader.setSource(page, properties || {})
    }

    // 初始化
    Component.onCompleted: {
        if (isLoggedIn) {
            navigateTo("pages/DashboardPage.qml")
        } else {
            navigateTo("pages/LoginPage.qml")
        }
    }

    // 监听登录状态变化
    Connections {
        target: AuthService
        function onLoginStateChanged() {
            if (AuthService.loggedIn) {
                navigateTo("pages/DashboardPage.qml")
            } else {
                navigateTo("pages/LoginPage.qml")
            }
        }
    }

    // 主布局
    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // 主内容区
        Loader {
            id: mainLoader
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
