import QtQuick 2.15
import QtQuick.Controls 2.15

Page {
    background: Rectangle { color: "#F5F7FA" }

    Rectangle {
        anchors.centerIn: parent
        width: 300
        height: 200
        color: "#FFFFFF"
        radius: 8
        border.color: "#E2E8F0"
        border.width: 1

        Text {
            anchors.centerIn: parent
            text: "测试页面加载成功！"
            font.pixelSize: 18
            color: "#2D3748"
        }

        Button {
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontal
            anchors.bottomMargin: 20
            text: "返回登录"
            onClicked: {
                appWindow.navigateTo("qrc:/qml/pages/LoginPage.qml")
            }
        }
    }
}
