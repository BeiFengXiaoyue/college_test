import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../theme"

/* ============================================================
   主仪表盘
   ============================================================ */
Page {
    id: root

    background: Rectangle {
        color: Theme.backgroundColor
    }

    // 顶部导航栏
    Rectangle {
        id: topBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 60
        color: Theme.surfaceColor

        // 底部边框线
        Rectangle {
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 1
            color: Theme.borderColor
        }

        RowLayout {
            anchors.fill: parent
            anchors.margins: Theme.spacingLg

            Text {
                text: "🎓 在线教育平台"
                font.pixelSize: Theme.fontSizeXl
                font.bold: true
                color: Theme.textPrimary
            }

            Item { Layout.fillWidth: true }

            // 用户信息
            RowLayout {
                spacing: Theme.spacingMd

                Text {
                    text: AuthService.currentUser ? AuthService.currentUser.displayName : ""
                    font.pixelSize: Theme.fontSizeMd
                    color: Theme.textPrimary
                    verticalAlignment: Text.AlignVCenter
                }

                Rectangle {
                    width: 32
                    height: 32
                    radius: 16
                    color: Theme.primaryColor

                    Text {
                        anchors.centerIn: parent
                        text: AuthService.currentUser ? AuthService.currentUser.displayName.charAt(0) : "?"
                        color: Theme.textOnPrimary
                        font.bold: true
                    }
                }

                Button {
                    text: "退出"
                    flat: true
                    font.pixelSize: Theme.fontSizeSm
                    onClicked: AuthService.logout()
                }
            }
        }
    }

    // 主内容
    ScrollView {
        anchors.top: topBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: Theme.spacingXl
        clip: true

        ColumnLayout {
            width: parent.width
            spacing: Theme.spacingXl

            // 欢迎语
            ColumnLayout {
                spacing: Theme.spacingXs

                Text {
                    text: "欢迎回来，" + (AuthService.currentUser ? AuthService.currentUser.displayName : "")
                    font.pixelSize: Theme.fontSizeXxl
                    font.bold: true
                    color: Theme.textPrimary
                }

                Text {
                    text: AuthService.isTeacher ? "管理您的课程和学员" : "继续您的学习之旅"
                    font.pixelSize: Theme.fontSizeMd
                    color: Theme.textSecondary
                }
            }

            // 快捷操作卡片
            RowLayout {
                spacing: Theme.spacingLg

                // 课程
                ActionCard {
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 140
                    icon: "📚"
                    title: "课程列表"
                    description: AuthService.isTeacher ? "管理您的课程" : "浏览所有课程"
                    onClicked: appWindow.navigateTo("qrc:/qml/pages/CourseListPage.qml")
                }

                // 直播
                ActionCard {
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 140
                    icon: "📺"
                    title: "直播课堂"
                    description: "参加正在进行的直播课程"
                    onClicked: appWindow.navigateTo("qrc:/qml/pages/CourseListPage.qml", {filter: "live"})
                }

                // 作业
                ActionCard {
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 140
                    icon: "📝"
                    title: "在线作业"
                    description: AuthService.isTeacher ? "布置与批改作业" : "提价与查看作业"
                    onClicked: appWindow.navigateTo("qrc:/qml/pages/HomeworkPage.qml")
                }

                // 学习记录
                ActionCard {
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 140
                    icon: "📊"
                    title: "学习记录"
                    description: "查看学习进度与统计"
                    onClicked: CourseService.loadLearningRecords(AuthService.currentUser.id)
                }
            }

            // 最近课程
            ColumnLayout {
                spacing: Theme.spacingMd

                Text {
                    text: "最近课程"
                    font.pixelSize: Theme.fontSizeXl
                    font.bold: true
                    color: Theme.textPrimary
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 200
                    color: Theme.surfaceColor
                    radius: Theme.radiusMd
                    border.color: Theme.borderColor
                    border.width: 1

                    Text {
                        anchors.centerIn: parent
                        text: "暂无最近课程记录\n开始学习第一门课程吧！"
                        font.pixelSize: Theme.fontSizeMd
                        color: Theme.textLight
                        horizontalAlignment: Text.AlignHCenter
                        lineHeight: 1.8
                    }
                }
            }
        }
    }

    // 学习记录加载
    Connections {
        target: CourseService
        function onLearningRecordsLoaded(records) {
            console.log("Loaded learning records:", records.length)
        }
    }
}
