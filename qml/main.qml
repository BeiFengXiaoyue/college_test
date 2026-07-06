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

    // 导航请求：子页面通过设置此属性来切换页面
    // 通过属性绑定 + Qt.callLater 延迟执行，避免在页面事件中销毁自身
    property string requestedPage: ""
    property var requestedPageProps: ({})

    onRequestedPageChanged: {
        if (requestedPage.length > 0) {
            var page = requestedPage
            var props = requestedPageProps
            requestedPage = ""
            requestedPageProps = ({})
            Qt.callLater(function() {
                mainLoader.setSource(page, props)
            })
        }
    }

    // 导航辅助函数（供 QML 页面调用）
    function navigateTo(page, props) {
        requestedPageProps = props || ({})
        requestedPage = page
    }

    // 初始化
    Component.onCompleted: {
        navigateTo("qrc:/qml/pages/LoginPage.qml")
    }

    // 监听登录状态变化
    Connections {
        target: AuthService
        function onLoginStateChanged() {
            if (ProcessManager.isRestarting) {
                return
            }
            if (AuthService.loggedIn) {
                navigateTo("qrc:/qml/pages/DashboardPage.qml")
            } else {
                navigateTo("qrc:/qml/pages/LoginPage.qml")
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
