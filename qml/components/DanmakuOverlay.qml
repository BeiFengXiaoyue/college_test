import QtQuick 2.15
import QtQuick.Controls 2.15
import "../theme"

/* ============================================================
   弹幕叠加层组件
   支持：滚动弹幕、顶部/底部固定弹幕
   支持：自定义颜色、字号
   ============================================================ */
Item {
    id: root
    anchors.fill: parent

    // 属性
    property bool danmakuEnabled: true
    property int maxDanmaku: 50
    property var danmakuQueue: []

    // 发送弹幕
    signal sendDanmaku(string content, string color, int fontSize, string position)

    // 添加弹幕（从外部调用）
    function addDanmaku(content, color, fontSize, position, userName) {
        if (!root.danmakuEnabled) return
        if (content.trim() === "") return

        var dmComponent = Qt.createComponent("DanmakuItem.qml")

        // 如果组件加载失败直接创建Item
        var danmakuItem = Qt.createQmlObject('import QtQuick 2.15; Text { color: "' + (color || "#FFFFFF") + '"; font.pixelSize: ' + (fontSize || 24) + '; text: "' + escapeText(content) + '"; font.bold: true; style: Text.Outline; styleColor: "#88000000" }', root)

        if (danmakuItem) {
            danmakuItem.x = root.width
            danmakuItem.y = position === "top" ? 30 + Math.random() * 80 :
                              position === "bottom" ? root.height - 60 - Math.random() * 80 :
                              Math.random() * (root.height - 100) + 30

            // 滚动弹幕
            if (position !== "top" && position !== "bottom") {
                var anim = Qt.createQmlObject('import QtQuick 2.15; NumberAnimation { from: ' + root.width + '; to: -' + (danmakuItem.width + 50) + '; duration: ' + (Theme.danmakuSpeed * 20 + Math.random() * 1000) + '; loops: 1 }', root)
                anim.target = danmakuItem
                anim.property = "x"
                anim.start()

                // 自动销毁
                var timer = Qt.createQmlObject('import QtQuick 2.15; Timer { interval: ' + (Theme.danmakuSpeed * 20 + 1500) + '; running: true; repeat: false }', root)
                timer.triggered.connect(function() {
                    danmakuItem.destroy()
                    timer.destroy()
                })
            } else {
                // 固定弹幕（淡入淡出）
                danmakuItem.opacity = 0
                var fadeIn = Qt.createQmlObject('import QtQuick 2.15; NumberAnimation { from: 0; to: 1; duration: 500 }', root)
                fadeIn.target = danmakuItem
                fadeIn.property = "opacity"
                fadeIn.start()

                var fadeTimer = Qt.createQmlObject('import QtQuick 2.15; Timer { interval: 3000; running: true; repeat: false }', root)
                fadeTimer.triggered.connect(function() {
                    var fadeOut = Qt.createQmlObject('import QtQuick 2.15; NumberAnimation { from: 1; to: 0; duration: 500 }', root)
                    fadeOut.target = danmakuItem
                    fadeOut.property = "opacity"
                    fadeOut.onFinished = function() {
                        danmakuItem.destroy()
                        fadeTimer.destroy()
                    }
                    fadeOut.start()
                })
            }
        }
    }

    // 转义文本中的特殊字符
    function escapeText(text) {
        return text.replace(/"/g, '\\"').replace(/\n/g, " ").replace(/</g, "&lt;").replace(/>/g, "&gt;")
    }

    // 弹幕输入条
    Rectangle {
        id: inputBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 40
        color: Qt.rgba(0, 0, 0, 0.5)
        visible: root.danmakuEnabled

        RowLayout {
            anchors.fill: parent
            anchors.margins: 4
            spacing: 4

            TextField {
                id: danmakuInput
                Layout.fillWidth: true
                Layout.preferredHeight: 32
                placeholderText: "发送弹幕..."
                color: "white"
                font.pixelSize: 14
                leftPadding: 8

                background: Rectangle {
                    color: Qt.rgba(255, 255, 255, 0.15)
                    radius: 4
                }

                Keys.onReturnPressed: sendBtn.clicked()
            }

            Button {
                id: sendBtn
                text: "发送"
                font.pixelSize: 12
                implicitWidth: 50
                implicitHeight: 32

                contentItem: Text {
                    text: parent.text
                    color: "white"
                    font: parent.font
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle {
                    color: Theme.primaryColor
                    radius: 4
                }

                onClicked: {
                    var text = danmakuInput.text.trim()
                    if (text) {
                        root.sendDanmaku(text, "#FFFFFF", 24, "scroll")
                        root.addDanmaku(text, "#FFFFFF", 24, "scroll", "我")
                        danmakuInput.text = ""
                    }
                }
            }

            // 弹幕开关
            Button {
                text: root.danmakuEnabled ? "📺" : "🚫"
                font.pixelSize: 14
                flat: true
                implicitWidth: 32
                implicitHeight: 32
                onClicked: root.danmakuEnabled = !root.danmakuEnabled
            }
        }
    }
}
