import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../theme"

/* ============================================================
   注册页
   ============================================================ */
Page {
    id: root

    background: Rectangle {
        color: Theme.backgroundColor
    }

    // 居中卡片
    Rectangle {
        anchors.centerIn: parent
        width: 420
        height: 560
        radius: Theme.radiusLg
        color: Theme.surfaceColor
        border.color: "#15000000"
        border.width: 1

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: Theme.spacingXxl
            spacing: Theme.spacingMd

            // 标题
            ColumnLayout {
                Layout.fillWidth: true
                spacing: Theme.spacingXs

                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: "📝"
                    font.pixelSize: 40
                }

                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: "创建新账户"
                    font.pixelSize: Theme.fontSizeXxl
                    font.bold: true
                    color: Theme.textPrimary
                }

                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: "注册加入在线教育平台"
                    font.pixelSize: Theme.fontSizeMd
                    color: Theme.textSecondary
                }
            }

            Item { Layout.preferredHeight: Theme.spacingSm }

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

            // 显示名称
            ColumnLayout {
                Layout.fillWidth: true
                spacing: Theme.spacingXs

                Text {
                    text: "显示名称"
                    font.pixelSize: Theme.fontSizeSm
                    color: Theme.textSecondary
                }

                TextField {
                    id: displayNameField
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    placeholderText: "请输入您的昵称"
                    font.pixelSize: Theme.fontSizeMd
                    leftPadding: 12

                    background: Rectangle {
                        radius: Theme.radiusSm
                        color: Theme.backgroundColor
                        border.color: displayNameField.activeFocus ? Theme.primaryColor : Theme.borderColor
                        border.width: 1
                    }
                }
            }

            // 角色选择
            RowLayout {
                Layout.fillWidth: true
                spacing: Theme.spacingMd

                Text {
                    text: "角色："
                    font.pixelSize: Theme.fontSizeSm
                    color: Theme.textSecondary
                    verticalAlignment: Text.AlignVCenter
                }

                Item { Layout.fillWidth: true }

                ComboBox {
                    id: roleCombo
                    implicitWidth: 120
                    implicitHeight: 36
                    model: ["学生", "教师"]
                    font.pixelSize: Theme.fontSizeMd

                    background: Rectangle {
                        radius: Theme.radiusSm
                        color: Theme.backgroundColor
                        border.color: roleCombo.activeFocus ? Theme.primaryColor : Theme.borderColor
                        border.width: 1
                    }

                    // 映射到 C++ 需要的 role 值
                    function roleValue() {
                        return currentIndex === 1 ? "teacher" : "student"
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
                    placeholderText: "请输入密码（至少6位）"
                    echoMode: TextInput.Password
                    font.pixelSize: Theme.fontSizeMd
                    leftPadding: 12

                    background: Rectangle {
                        radius: Theme.radiusSm
                        color: Theme.backgroundColor
                        border.color: passwordField.activeFocus ? Theme.primaryColor : Theme.borderColor
                        border.width: 1
                    }
                }
            }

            // 确认密码
            ColumnLayout {
                Layout.fillWidth: true
                spacing: Theme.spacingXs

                Text {
                    text: "确认密码"
                    font.pixelSize: Theme.fontSizeSm
                    color: Theme.textSecondary
                }

                TextField {
                    id: confirmPasswordField
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    placeholderText: "请再次输入密码"
                    echoMode: TextInput.Password
                    font.pixelSize: Theme.fontSizeMd
                    leftPadding: 12

                    background: Rectangle {
                        radius: Theme.radiusSm
                        color: Theme.backgroundColor
                        border.color: confirmPasswordField.activeFocus ? Theme.primaryColor : Theme.borderColor
                        border.width: 1
                    }

                    Keys.onReturnPressed: registerBtn.clicked()
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

            // 注册按钮
            Button {
                id: registerBtn
                Layout.fillWidth: true
                Layout.preferredHeight: 44
                text: "注 册"
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
                    color: registerBtn.hovered ? Theme.primaryLight : Theme.primaryColor
                }

                onClicked: {
                    // 前端校验
                    errorText.text = ""

                    if (usernameField.text.trim() === "") {
                        errorText.text = "请输入用户名"
                        return
                    }
                    if (displayNameField.text.trim() === "") {
                        errorText.text = "请输入显示名称"
                        return
                    }
                    if (passwordField.text.length < 6) {
                        errorText.text = "密码至少需要6位"
                        return
                    }
                    if (passwordField.text !== confirmPasswordField.text) {
                        errorText.text = "两次密码输入不一致"
                        return
                    }

                    registerBtn.enabled = false
                    registerBtn.text = "注册中..."
                    AuthService.registerUser(
                        usernameField.text.trim(),
                        passwordField.text,
                        displayNameField.text.trim(),
                        roleCombo.roleValue()
                    )
                }
            }

            // 返回登录链接
            Text {
                Layout.alignment: Qt.AlignHCenter
                text: "已有账户？<a href='login'>立即登录</a>"
                font.pixelSize: Theme.fontSizeSm
                color: Theme.textSecondary
                onLinkActivated: {
                    mainLoader.setSource("pages/LoginPage.qml")
                }
            }
        }
    }

    // 注册结果处理
    Connections {
        target: AuthService
        function onRegisterSuccess(userId) {
            registerBtn.enabled = true
            registerBtn.text = "注 册"
            // main.qml 的 onLoginStateChanged 会自动跳转到仪表盘
        }
        function onRegisterFailed(error) {
            errorText.text = error
            registerBtn.enabled = true
            registerBtn.text = "注 册"
        }
    }
}
