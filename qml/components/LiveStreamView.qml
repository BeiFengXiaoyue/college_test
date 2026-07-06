import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtMultimedia 5.15
import "../theme"

/* ============================================================
   直播视图组件
   封装 QMediaPlayer + 拉流播放
   ============================================================ */
Rectangle {
    id: root
    color: "black"

    property string streamUrl: ""
    property alias player: mediaPlayer
    property alias videoOutput: videoOut
    property bool isLive: false
    property bool showControls: true

    signal streamStarted()
    signal streamStopped()
    signal streamError(string errorMsg)

    // 媒体播放器（拉流端）
    MediaPlayer {
        id: mediaPlayer
        source: root.streamUrl
        audioOutput: AudioOutput {}

        videoOutput: videoOut

        onErrorOccurred: {
            if (error !== MediaPlayer.NoError) {
                root.streamError(errorString)
            }
        }
    }

    // 视频输出
    VideoOutput {
        id: videoOut
        anchors.fill: parent
    }

    // 直播状态标识
    Rectangle {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 12
        height: 28
        width: liveLabel.width + 24
        radius: 14
        color: Qt.rgba(0, 0, 0, 0.5)

        RowLayout {
            anchors.centerIn: parent
            spacing: 6

            Rectangle {
                width: 8
                height: 8
                radius: 4
                color: root.isLive ? Theme.dangerColor : "gray"
                Timer {
                    running: root.isLive
                    repeat: true
                    interval: 1000
                    onTriggered: parent.visible = !parent.visible
                }
            }

            Text {
                id: liveLabel
                text: root.isLive ? "直播中" : "未开播"
                color: "white"
                font.pixelSize: 12
                font.bold: true
            }
        }
    }

    // 控制按钮：播放/停止
    RowLayout {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 16
        spacing: 12
        visible: root.showControls

        Button {
            text: root.isLive ? "■ 停止" : "▶ 播放"
            font.pixelSize: 14
            contentItem: Text {
                text: parent.text
                color: "white"
                font: parent.font
            }
            background: Rectangle {
                radius: 6
                color: root.isLive ? Theme.dangerColor : Theme.successColor
            }
            onClicked: {
                if (root.isLive) {
                    mediaPlayer.stop()
                    root.isLive = false
                    root.streamStopped()
                } else if (root.streamUrl) {
                    mediaPlayer.play()
                    root.isLive = true
                    root.streamStarted()
                }
            }
        }

        // 音量
        Slider {
            implicitWidth: 80
            from: 0
            to: 1.0
            value: 1.0
            onMoved: mediaPlayer.audioOutput.volume = value
            background: Rectangle {
                x: parent.leftPadding
                y: parent.topPadding + parent.availableHeight / 2 - height / 2
                width: parent.availableWidth
                height: 3
                color: "#555"
                Rectangle {
                    width: parent.visualPosition * parent.width
                    height: parent.height
                    color: Theme.primaryColor
                }
            }
            handle: Rectangle {
                x: parent.leftPadding + parent.visualPosition * (parent.availableWidth - width)
                y: parent.topPadding + parent.availableHeight / 2 - height / 2
                width: 12
                height: 12
                radius: 6
                color: "white"
            }
        }
    }

    // 加载提示
    Text {
        anchors.centerIn: parent
        text: !root.streamUrl ? "等待推流中..." :
              root.isLive ? "" :
              "点击播放按钮开始观看直播"
        color: "white"
        font.pixelSize: 16
        opacity: 0.7
    }

    // 公开方法
    function play() {
        if (root.streamUrl) {
            mediaPlayer.play()
            root.isLive = true
        }
    }

    function stop() {
        mediaPlayer.stop()
        root.isLive = false
    }
}
