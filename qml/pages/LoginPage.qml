import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../theme"

/* ============================================================
   登录页
   ============================================================ */
Page {
    id: root

    background: Rectangle {
        color: Theme.backgroundColor
    }

    // 居中卡片
    Rectangle {
        anchors.centerIn: parent
        width: 400
        height: 480
        radius: Theme.radiusLg
        color: Theme.surfaceColor

        // 阴影（使用边框模拟）
        border.color: "#15000000"
        border.width: 1

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: Theme.spacingXxl
            spacing: Theme.spacingLg

            // Logo & 标题
            ColumnLayout {
                Layout.fillWidth: true
                spacing: Theme.spacingSm

                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: "🎓"
                    font.pixelSize: 48
                }

                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: "在线教育平台"
                    font.pixelSize: Theme.fontSizeXxl
                    font.bold: true
                    color: Theme.textPrimary
                }

                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: "登录您的账户"
                    font.pixelSize: Theme.fontSizeMd
                    color: Theme.textSecondary
                }
            }

            Item { Layout.preferredHeight: Theme.spacingLg }

            // 用户名
            ColumnLayout {
                Layout.fillWidth: true
                spacing: Theme.spacingXs

                Text {
                    text: "用户名"
                    font.pixelSize: Theme.fontSizeSm
                    color: Theme.textSecondary
                }

                TextField {
                    id: usernameField
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    placeholderText: "请输入用户名"
                    font.pixelSize: Theme.fontSizeMd
                    leftPadding: 12

                    background: Rectangle {
                        radius: Theme.radiusSm
                        color: Theme.backgroundColor
                        border.color: usernameField.activeFocus ? Theme.primaryColor : Theme.borderColor
                        border.width: 1
                    }
                }
            }

            // 密码
            ColumnLayout {
                Layout.fillWidth: true
                spacing: Theme.spacingXs

                Text {
                    text: "密码"
                    font.pixelSize: Theme.fontSizeSm
                    color: Theme.textSecondary
                }

                TextField {
                    id: passwordField
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    placeholderText: "请输入密码"
                    echoMode: TextInput.Password
                    font.pixelSize: Theme.fontSizeMd
                    leftPadding: 12

                    background: Rectangle {
                        radius: Theme.radiusSm
                        color: Theme.backgroundColor
                        border.color: passwordField.activeFocus ? Theme.primaryColor : Theme.borderColor
                        border.width: 1
                    }

                    Keys.onReturnPressed: loginBtn.clicked()
                }
            }

            // 错误提示
            Text {
                id: errorText
                Layout.fillWidth: true
                visible: text.length > 0
                color: Theme.dangerColor
                font.pixelSize: Theme.fontSizeSm
                wrapMode: Text.WordWrap
            }

            Item { Layout.preferredHeight: Theme.spacingSm }

            // 登录按钮
            Button {
                id: loginBtn
                Layout.fillWidth: true
                Layout.preferredHeight: 44
                text: "登 录"
                font.pixelSize: Theme.fontSizeLg
                font.bold: true

                contentItem: Text {
                    text: parent.text
                    color: Theme.textOnPrimary
                    font: parent.font
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle {
                    radius: Theme.radiusMd
                    color: loginBtn.hovered ? Theme.primaryLight : Theme.primaryColor
                }

                onClicked: {
                    if (usernameField.text.trim() === "" || passwordField.text.trim() === "") {
                        errorText.text = "请输入用户名和密码"
                        return
                    }
                    errorText.text = ""
                    loginBtn.enabled = false
                    loginBtn.text = "登录中..."
                    AuthService.login(usernameField.text.trim(), passwordField.text)
                }
            }

            // 注册链接
            Item {
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredHeight: linkText.height + 4
                Layout.fillWidth: true

                Text {
                    id: linkText
                    anchors.centerIn: parent
                    text: "还没有账户？"
                    font.pixelSize: Theme.fontSizeSm
                    color: Theme.textSecondary
                }

                Text {
                    anchors.left: linkText.right
                    anchors.verticalCenter: linkText.verticalCenter
                    anchors.leftMargin: 2
                    text: "立即注册"
                    font.pixelSize: Theme.fontSizeSm
                    color: Theme.primaryColor
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        appWindow.navigateTo("qrc:/qml/pages/RegisterPage.qml")
                    }
                }
            }
        }
    }

    // 登录结果处理
    Connections {
        target: AuthService
        function onLoginSuccess(userId) {
            loginBtn.enabled = true
            loginBtn.text = "登 录"
        }
        function onLoginFailed(error) {
            errorText.text = error
            loginBtn.enabled = true
            loginBtn.text = "登 录"
        }
        function onRegisterSuccess(userId) {
            loginBtn.enabled = true
            loginBtn.text = "登 录"
        }
        function onRegisterFailed(error) {
            errorText.text = error
            loginBtn.enabled = true
            loginBtn.text = "注 册"
        }
    }
}
