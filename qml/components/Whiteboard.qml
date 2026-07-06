import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../theme"

/* ============================================================
   互动白板组件
   支持：铅笔、直线、矩形、圆形、文字、橡皮擦
   支持：撤销/重做、颜色选择、笔触大小调节
   ============================================================ */
Rectangle {
    id: root
    color: "white"

    // 属性
    property color penColor: "#FF0000"
    property real penWidth: 3
    property string currentTool: "pen"  // pen, line, rect, circle, text, eraser
    property var drawHistory: []       // 绘制历史（用于撤销）
    property int historyIndex: -1
    property bool canUndo: historyIndex >= 0
    property bool canRedo: historyIndex < drawHistory.length - 1

    signal drawingAction(var action)

    // 工具条
    Rectangle {
        id: toolbar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: 48
        color: "#F0F0F0"

        RowLayout {
            anchors.fill: parent
            anchors.margins: 4
            spacing: 4

            Repeater {
                model: [
                    {tool: "pen", icon: "✏️"},
                    {tool: "line", icon: "📏"},
                    {tool: "rect", icon: "⬜"},
                    {tool: "circle", icon: "⭕"},
                    {tool: "text", icon: "🔤"},
                    {tool: "eraser", icon: "🧹"}
                ]
                delegate: Button {
                    text: modelData.icon
                    font.pixelSize: 18
                    checkable: true
                    checked: root.currentTool === modelData.tool
                    flat: true
                    implicitWidth: 36
                    implicitHeight: 36

                    background: Rectangle {
                        color: checked ? Theme.primaryLight : "transparent"
                        radius: 6
                    }

                    onClicked: {
                        root.currentTool = modelData.tool
                    }
                }
            }

            Item { Layout.fillWidth: true }

            // 颜色选择器
            Rectangle {
                Layout.preferredWidth: 28
                Layout.preferredHeight: 28
                radius: 14
                border.width: 2
                border.color: Theme.borderColor
                color: root.penColor

                MouseArea {
                    anchors.fill: parent
                    onClicked: colorDialog.open()
                }
            }

            // 笔触大小
            SpinBox {
                id: widthSpin
                from: 1
                to: 20
                value: root.penWidth
                implicitWidth: 60
                implicitHeight: 28
                font.pixelSize: 11
                onValueChanged: root.penWidth = value
            }

            // 撤销/重做
            Button {
                text: "↩"
                font.pixelSize: 16
                enabled: root.canUndo
                flat: true
                onClicked: undo()
            }
            Button {
                text: "↪"
                font.pixelSize: 16
                enabled: root.canRedo
                flat: true
                onClicked: redo()
            }
            Button {
                text: "🗑"
                font.pixelSize: 16
                flat: true
                onClicked: clearAll()
            }
        }
    }

    // 画布
    Canvas {
        id: canvas
        anchors.top: toolbar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        property var currentPath: []
        property point startPoint: Qt.point(0, 0)
        property bool drawing: false

        // 鼠标事件
        MouseArea {
            id: canvasMouse
            anchors.fill: parent
            cursorShape: Qt.CrossCursor

            onPressed: function(mouse) {
                canvas.startPoint = Qt.point(mouse.x, mouse.y)
                canvas.drawing = true

                if (root.currentTool === "pen") {
                    canvas.currentPath = [Qt.point(mouse.x, mouse.y)]
                }
            }

            onPositionChanged: function(mouse) {
                if (!canvas.drawing) return
                var pos = Qt.point(mouse.x, mouse.y)

                if (root.currentTool === "pen") {
                    canvas.currentPath.push(pos)
                    canvas.requestPaint()
                } else if (root.currentTool === "line" || root.currentTool === "rect" || root.currentTool === "circle") {
                    // 临时绘制（直接在画布上画预览）
                    canvas.requestPaint()
                }
            }

            onReleased: function(mouse) {
                if (!canvas.drawing) return
                canvas.drawing = false

                var action = {
                    tool: root.currentTool,
                    color: root.penColor,
                    width: root.penWidth,
                    startX: canvas.startPoint.x,
                    startY: canvas.startPoint.y,
                    endX: mouse.x,
                    endY: mouse.y,
                    path: root.currentTool === "pen" ? canvas.currentPath.slice() : []
                }

                // 添加到历史
                if (historyIndex < drawHistory.length - 1) {
                    drawHistory = drawHistory.slice(0, historyIndex + 1)
                }
                drawHistory.push(action)
                historyIndex = drawHistory.length - 1

                // 重绘
                canvas.requestPaint()
                root.drawingAction(action)
            }
        }

        // 绘制
        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)

            // 绘制所有历史动作
            for (var i = 0; i <= historyIndex && i < drawHistory.length; i++) {
                drawAction(ctx, drawHistory[i])
            }

            // 如果是直线/矩形/圆形，绘制预览
            if (drawing) {
                var previewAction = {
                    tool: root.currentTool,
                    color: root.penColor,
                    width: root.penWidth,
                    startX: canvas.startPoint.x,
                    startY: canvas.startPoint.y,
                    endX: canvasMouse.mouseX,
                    endY: canvasMouse.mouseY
                }
                drawAction(ctx, previewAction)
            }
        }

        function drawAction(ctx, action) {
            if (!action) return

            ctx.save()
            ctx.strokeStyle = action.tool === "eraser" ? "white" : action.color
            ctx.lineWidth = action.width
            ctx.lineCap = "round"
            ctx.lineJoin = "round"
            ctx.fillStyle = action.color

            switch (action.tool) {
            case "pen":
                if (action.path && action.path.length > 1) {
                    ctx.beginPath()
                    ctx.moveTo(action.path[0].x, action.path[0].y)
                    for (var p = 1; p < action.path.length; p++) {
                        ctx.lineTo(action.path[p].x, action.path[p].y)
                    }
                    ctx.stroke()
                }
                break
            case "line":
                ctx.beginPath()
                ctx.moveTo(action.startX, action.startY)
                ctx.lineTo(action.endX, action.endY)
                ctx.stroke()
                break
            case "rect":
                var rx = Math.min(action.startX, action.endX)
                var ry = Math.min(action.startY, action.endY)
                var rw = Math.abs(action.endX - action.startX)
                var rh = Math.abs(action.endY - action.startY)
                ctx.strokeRect(rx, ry, rw, rh)
                break
            case "circle":
                var cx = (action.startX + action.endX) / 2
                var cy = (action.startY + action.endY) / 2
                var rad = Math.sqrt(Math.pow(action.endX - action.startX, 2) + Math.pow(action.endY - action.startY, 2)) / 2
                ctx.beginPath()
                ctx.arc(cx, cy, rad, 0, Math.PI * 2)
                ctx.stroke()
                break
            case "eraser":
                ctx.strokeStyle = "white"
                ctx.lineWidth = action.width * 3
                ctx.beginPath()
                ctx.moveTo(action.startX, action.startY)
                ctx.lineTo(action.endX, action.endY)
                ctx.stroke()
                break
            }

            ctx.restore()
        }
    }

    // 颜色选择弹窗
    Dialog {
        id: colorDialog
        title: "选择颜色"
        modal: true
        x: parent.width / 2 - width / 2
        y: parent.height / 2 - height / 2

        GridLayout {
            columns: 6
            spacing: 4

            Repeater {
                model: ["#FF0000", "#FF6600", "#FFCC00", "#00CC00", "#0066FF", "#6633FF",
                        "#FF3399", "#33CCFF", "#00CCCC", "#99CC00", "#CC6600", "#666666",
                        "#000000", "#993300", "#333333", "#6699FF", "#FF6666", "#66FF66"]
                delegate: Rectangle {
                    width: 30
                    height: 30
                    radius: 4
                    color: modelData
                    border.width: root.penColor === modelData ? 2 : 0
                    border.color: "black"

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            root.penColor = modelData
                            colorDialog.close()
                        }
                    }
                }
            }
        }
    }

    // 函数
    function undo() {
        if (historyIndex >= 0) {
            historyIndex--
            canvas.requestPaint()
        }
    }

    function redo() {
        if (historyIndex < drawHistory.length - 1) {
            historyIndex++
            canvas.requestPaint()
        }
    }

    function clearAll() {
        drawHistory = []
        historyIndex = -1
        canvas.requestPaint()
    }

    function loadActions(actions) {
        drawHistory = actions || []
        historyIndex = drawHistory.length - 1
        canvas.requestPaint()
    }
}
