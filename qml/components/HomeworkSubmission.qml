import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.1
import "../theme"

/* ============================================================
   作业提价表单组件
   支持文本输入 + 文件上传
   ============================================================ */
Rectangle {
    id: root
    color: Theme.surfaceColor
    radius: Theme.radiusMd

    property int homeworkId: 0
    property int studentId: 0
    property bool submitted: false
    property string submissionContent: ""
    property string fileUrl: ""

    signal submitRequested(int homeworkId, int studentId, string content, string fileUrl)

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Theme.spacingXl
        spacing: Theme.spacingLg

        Text {
            text: submitted ? "✅ 作业已提价" : "📝 提价作业"
            font.pixelSize: Theme.fontSizeXl
            font.bold: true
            color: submitted ? Theme.successColor : Theme.textPrimary
        }

        // 文本内容
        ColumnLayout {
            Layout.fillWidth: true
            spacing: Theme.spacingSm

            Text {
                text: "作业内容"
                font.pixelSize: Theme.fontSizeSm
                color: Theme.textSecondary
            }

            TextArea {
                id: contentArea
                Layout.fillWidth: true
                Layout.preferredHeight: 150
                placeholderText: "请在此输入作业内容..."
                font.pixelSize: Theme.fontSizeMd
                leftPadding: 8
                topPadding: 8
                enabled: !root.submitted

                background: Rectangle {
                    radius: Theme.radiusSm
                    color: Theme.backgroundColor
                    border.color: contentArea.activeFocus ? Theme.primaryColor : Theme.borderColor
                    border.width: 1
                }
            }
        }

        // 文件上传
        ColumnLayout {
            Layout.fillWidth: true
            spacing: Theme.spacingSm

            Text {
                text: "附件（可选）"
                font.pixelSize: Theme.fontSizeSm
                color: Theme.textSecondary
            }

            RowLayout {
                spacing: Theme.spacingMd

                Button {
                    text: "📎 选择文件"
                    font.pixelSize: Theme.fontSizeSm
                    enabled: !root.submitted

                    contentItem: Text {
                        text: parent.text
                        color: root.submitted ? Theme.textLight : Theme.textPrimary
                        font: parent.font
                    }

                    background: Rectangle {
                        radius: Theme.radiusSm
                        color: root.submitted ? "#F0F0F0" : Theme.backgroundColor
                        border.color: Theme.borderColor
                        border.width: 1
                    }

                    onClicked: fileDialog.open()
                }

                Text {
                    id: fileNameText
                    text: root.fileUrl ? root.fileUrl.split('/').pop() : "未选择文件"
                    font.pixelSize: Theme.fontSizeSm
                    color: Theme.textSecondary
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                }
            }
        }

        Item { Layout.fillHeight: true }

        // 提价按钮
        Button {
            Layout.fillWidth: true
            Layout.preferredHeight: 44
            text: root.submitted ? "已提价" : "提价作业"
            enabled: !root.submitted && (contentArea.text.trim().length > 0 || root.fileUrl.length > 0)
            font.pixelSize: Theme.fontSizeLg

            contentItem: Text {
                text: parent.text
                color: parent.enabled ? Theme.textOnPrimary : Theme.textLight
                font: parent.font
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                radius: Theme.radiusMd
                color: parent.enabled ? Theme.primaryColor : "#E0E0E0"
            }

            onClicked: {
                root.submissionContent = contentArea.text
                root.submitRequested(homeworkId, studentId, contentArea.text, fileUrl)
            }
        }
    }

    // 文件选择对话框
    FileDialog {
        id: fileDialog
        title: "选择作业文件"
        nameFilters: ["所有文件 (*)", "PDF (*.pdf)", "图片 (*.png *.jpg *.jpeg)", "压缩包 (*.zip *.rar)"]
        onAccepted: {
            root.fileUrl = fileDialog.file.toString()
            fileNameText.text = root.fileUrl.split('/').pop()
        }
    }
}
