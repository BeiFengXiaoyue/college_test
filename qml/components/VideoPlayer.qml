import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtMultimedia 5.15
import "../theme"

/* ============================================================
   视频播放器组件
   封装 QMediaPlayer，提供播放控制界面
   ============================================================ */
Rectangle {
    id: root
    color: "black"

    property string source: ""
    property alias player: mediaPlayer
    property alias videoOutput: videoOut
    property bool showControls: true
    property real volume: 1.0
    property real playbackRate: 1.0

    signal playbackFinished()
    signal positionChanged(qint64 position, qint64 duration)

    // 媒体播放器
    MediaPlayer {
        id: mediaPlayer
        source: root.source
        audioOutput: AudioOutput {
            volume: root.volume
        }
        videoOutput: videoOut

        onPositionChanged: {
            root.positionChanged(position, duration)
        }
        onPlaybackStateChanged: {
            if (playbackState === MediaPlayer.StoppedState && position > 0) {
                root.playbackFinished()
            }
        }
    }

    // 视频输出
    VideoOutput {
        id: videoOut
        anchors.fill: parent
    }

    // 播放控制覆盖层
    Rectangle {
        id: controlsOverlay
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: root.showControls ? 60 : 0
        color: Qt.rgba(0, 0, 0, 0.6)
        visible: root.showControls

        // 鼠标悬浮显示控制条
        opacity: controlsMouseArea.containsMouse || !mediaPlayer.playing ? 1.0 : 0.0
        Behavior on opacity { NumberAnimation { duration: 300 } }

        MouseArea {
            id: controlsMouseArea
            anchors.fill: parent
            hoverEnabled: true
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 8
            spacing: 4

            // 进度条
            Slider {
                id: progressSlider
                Layout.fillWidth: true
                Layout.preferredHeight: 16
                from: 0
                to: mediaPlayer.duration > 0 ? mediaPlayer.duration : 1
                value: mediaPlayer.position
                onMoved: mediaPlayer.seek(value)

                background: Rectangle {
                    x: progressSlider.leftPadding
                    y: progressSlider.topPadding + progressSlider.availableHeight / 2 - height / 2
                    width: progressSlider.availableWidth
                    height: 4
                    radius: 2
                    color: "#555555"

                    Rectangle {
                        width: progressSlider.visualPosition * parent.width
                        height: parent.height
                        radius: 2
                        color: Theme.primaryColor
                    }
                }

                handle: Rectangle {
                    x: progressSlider.leftPadding + progressSlider.visualPosition * (progressSlider.availableWidth - width)
                    y: progressSlider.topPadding + progressSlider.availableHeight / 2 - height / 2
                    width: 14
                    height: 14
                    radius: 7
                    color: Theme.primaryColor
                    visible: progressSlider.pressed || controlsMouseArea.containsMouse
                }
            }

            // 按钮行
            RowLayout {
                Layout.fillWidth: true
                spacing: 8

                Button {
                    text: mediaPlayer.playing ? "⏸" : "▶"
                    font.pixelSize: 18
                    flat: true
                    onClicked: {
                        if (mediaPlayer.playing) mediaPlayer.pause()
                        else mediaPlayer.play()
                    }
                }

                Text {
                    text: formatTime(mediaPlayer.position) + " / " + formatTime(mediaPlayer.duration)
                    color: "white"
                    font.pixelSize: 12
                }

                Item { Layout.fillWidth: true }

                // 倍速
                ComboBox {
                    id: speedCombo
                    model: ["0.5x", "1.0x", "1.5x", "2.0x"]
                    currentIndex: 1
                    font.pixelSize: 11
                    implicitWidth: 60
                    implicitHeight: 24
                    onCurrentTextChanged: {
                        var speed = parseFloat(currentText)
                        mediaPlayer.playbackRate = speed
                    }
                    background: Rectangle {
                        color: "#333333"
                        radius: 4
                    }
                    contentItem: Text {
                        text: speedCombo.currentText
                        color: "white"
                        font: speedCombo.font
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                // 音量
                Slider {
                    id: volumeSlider
                    implicitWidth: 80
                    implicitHeight: 20
                    from: 0
                    to: 1.0
                    value: root.volume
                    onMoved: mediaPlayer.audioOutput.volume = value

                    background: Rectangle {
                        x: volumeSlider.leftPadding
                        y: volumeSlider.topPadding + volumeSlider.availableHeight / 2 - height / 2
                        width: volumeSlider.availableWidth
                        height: 3
                        radius: 1.5
                        color: "#555555"

                        Rectangle {
                            width: volumeSlider.visualPosition * parent.width
                            height: parent.height
                            radius: 1.5
                            color: Theme.primaryColor
                        }
                    }

                    handle: Rectangle {
                        x: volumeSlider.leftPadding + volumeSlider.visualPosition * (volumeSlider.availableWidth - width)
                        y: volumeSlider.topPadding + volumeSlider.availableHeight / 2 - height / 2
                        width: 10
                        height: 10
                        radius: 5
                        color: "white"
                    }
                }

                Text {
                    text: volumeSlider.value > 0 ? "🔊" : "🔇"
                    color: "white"
                    font.pixelSize: 14
                }
            }
        }
    }

    // 点击暂停/播放
    MouseArea {
        anchors.fill: parent
        enabled: !controlsMouseArea.containsMouse
        onClicked: {
            if (mediaPlayer.playing) mediaPlayer.pause()
            else mediaPlayer.play()
        }
    }

    // 工具函数
    function formatTime(ms) {
        if (ms <= 0) return "00:00"
        var totalSeconds = Math.floor(ms / 1000)
        var minutes = Math.floor(totalSeconds / 60)
        var seconds = totalSeconds % 60
        return (minutes < 10 ? "0" : "") + minutes + ":" + (seconds < 10 ? "0" : "") + seconds
    }

    // 公开接口
    function play() { mediaPlayer.play() }
    function pause() { mediaPlayer.pause() }
    function stop() { mediaPlayer.stop() }
    function seek(pos) { mediaPlayer.seek(pos) }
}
