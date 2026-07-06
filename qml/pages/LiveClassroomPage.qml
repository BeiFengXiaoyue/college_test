import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtMultimedia 5.15
import "../theme"
import "../components"

/* ============================================================
   直播教室页面
   左侧：视频 + 弹幕
   右侧：互动白板
   ============================================================ */
Page {
    id: root
    property int lessonId: 0
    property string lessonTitle: ""
    property string videoUrl: ""

    background: Rectangle { color: Theme.backgroundColor }

    // 顶部
    Rectangle {
        id: topBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 50
        color: Theme.surfaceColor

        RowLayout {
            anchors.fill: parent
            anchors.margins: Theme.spacingMd

            Button {
                text: "← 返回"
                flat: true
                font.pixelSize: Theme.fontSizeMd
                onClicked: appWindow.navigateTo("qrc:/qml/pages/CourseDetailPage.qml", {courseId: root.lessonId})
            }

            Text {
                text: "🔴 " + root.lessonTitle
                font.pixelSize: Theme.fontSizeLg
                font.bold: true
                color: Theme.textPrimary
            }

            Item { Layout.fillWidth: true }

            Text {
                text: "直播中"
                color: Theme.dangerColor
                font.pixelSize: Theme.fontSizeSm
                font.bold: true
            }
        }
    }

    // 内容区：左右分栏
    RowLayout {
        anchors.top: topBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: Theme.spacingMd
        spacing: Theme.spacingMd

        // 左侧：视频 + 弹幕
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "black"
            radius: Theme.radiusMd
            clip: true

            // 直播视图
            LiveStreamView {
                id: liveView
                anchors.fill: parent
                streamUrl: root.videoUrl
                showControls: true

                Component.onCompleted: {
                    if (root.videoUrl) {
                        liveView.play()
                    }
                }
            }

            // 弹幕叠加层
            DanmakuOverlay {
                id: danmakuLayer
                anchors.fill: parent
                anchors.bottomMargin: 0

                onSendDanmaku: {
                    // 发送弹幕到服务端（C++ 后端）
                    // learningRecordDao.saveDanmaku(...)
                }
            }
        }

        // 右侧：互动白板
        Rectangle {
            Layout.preferredWidth: 420
            Layout.fillHeight: true
            color: Theme.surfaceColor
            radius: Theme.radiusMd

            // 阴影
            border.color: Theme.borderColor
            border.width: 1

            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                Text {
                    Layout.fillWidth: true
                    Layout.margins: Theme.spacingMd
                    text: "互动白板"
                    font.pixelSize: Theme.fontSizeLg
                    font.bold: true
                    color: Theme.textPrimary
                }

                Whiteboard {
                    id: whiteboard
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }
        }
    }

    // 功能快捷键
    Shortcut {
        sequence: "Escape"
        onActivated: appWindow.navigateTo("qrc:/qml/pages/CourseDetailPage.qml", {courseId: root.lessonId})
    }
}
