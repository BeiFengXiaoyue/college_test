import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../theme"

/* ============================================================
   快捷操作卡片组件
   ============================================================ */
Rectangle {
    id: root
    color: Theme.surfaceColor
    radius: Theme.radiusMd
    border.color: Theme.borderColor
    border.width: 1

    property string icon: "📦"
    property string title: "标题"
    property string description: "描述"
    signal clicked()

    // 鼠标区域
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: root.clicked()
        onEntered: root.color = Qt.lighter(Theme.surfaceColor, 1.02)
        onExited: root.color = Theme.surfaceColor
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Theme.spacingLg
        spacing: Theme.spacingSm

        Text {
            text: root.icon
            font.pixelSize: 36
        }

        Text {
            text: root.title
            font.pixelSize: Theme.fontSizeLg
            font.bold: true
            color: Theme.textPrimary
        }

        Text {
            text: root.description
            font.pixelSize: Theme.fontSizeSm
            color: Theme.textSecondary
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
        }
    }
}
