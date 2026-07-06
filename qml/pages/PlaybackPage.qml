import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../theme"
import "../components"

/* ============================================================
   录播回放页面
   视频播放 + 弹幕 + 学习进度追踪
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
                onClicked: mainLoader.setSource("pages/CourseDetailPage.qml", {courseId: root.lessonId})
            }

            Text {
                text: "▶️ " + root.lessonTitle
                font.pixelSize: Theme.fontSizeLg
                font.bold: true
                color: Theme.textPrimary
            }

            Item { Layout.fillWidth: true }

            // 学习进度
            Text {
                id: progressText
                text: "学习进度：0%"
                font.pixelSize: Theme.fontSizeSm
                color: Theme.textSecondary
            }
        }
    }

    // 视频播放器
    Rectangle {
        anchors.top: topBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: Theme.spacingMd
        color: "black"
        radius: Theme.radiusMd
        clip: true

        VideoPlayer {
            id: videoPlayer
            anchors.fill: parent
            source: root.videoUrl

            Component.onCompleted: {
                if (root.videoUrl) {
                    play()
                }
            }

            onPositionChanged: {
                // 更新学习进度
                if (duration > 0) {
                    var progress = position / duration
                    progressText.text = "学习进度：" + Math.round(progress * 100) + "%"

                    // 每5秒同步一次进度到服务端
                    if (Math.floor(position / 5000) > Math.floor((position - 100) / 5000)) {
                        CourseService.updateProgress(AuthService.currentUser.id, root.lessonId, progress, position)
                    }
                }
            }

            onPlaybackFinished: {
                // 标记完成
                CourseService.updateProgress(AuthService.currentUser.id, root.lessonId, 1.0, duration)
            }
        }

        // 弹幕
        DanmakuOverlay {
            id: danmakuLayer
            anchors.fill: parent
            anchors.bottomMargin: 60  // 给控件条留空间
        }
    }

    // 快捷键
    Shortcut {
        sequence: "Space"
        onActivated: {
            if (videoPlayer.player.playing) videoPlayer.pause()
            else videoPlayer.play()
        }
    }

    Shortcut {
        sequence: "Escape"
        onActivated: mainLoader.setSource("pages/CourseDetailPage.qml", {courseId: root.lessonId})
    }
}
