import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../theme"
import "../components"

/* ============================================================
   作业管理页面
   学生：查看作业列表、提价作业
   教师：布置作业、批改作业
   ============================================================ */
Page {
    id: root

    background: Rectangle { color: Theme.backgroundColor }

    // 顶部
    Rectangle {
        id: topBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 60
        color: Theme.surfaceColor

        RowLayout {
            anchors.fill: parent
            anchors.margins: Theme.spacingLg

            Button {
                text: "← 返回"
                flat: true
                font.pixelSize: Theme.fontSizeMd
                onClicked: appWindow.navigateTo("qrc:/qml/pages/DashboardPage.qml")
            }

            Text {
                text: "📝 在线作业"
                font.pixelSize: Theme.fontSizeXl
                font.bold: true
                color: Theme.textPrimary
                Layout.fillWidth: true
            }
        }
    }

    // 作业列表
    ListView {
        id: homeworkList
        anchors.top: topBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: Theme.spacingXl
        spacing: Theme.spacingMd
        clip: true

        model: ListModel { id: homeworkModel }

        delegate: Rectangle {
            width: homeworkList.width
            height: 160
            color: Theme.surfaceColor
            radius: Theme.radiusMd

            border.color: Theme.borderColor
            border.width: 1

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: Theme.spacingLg
                spacing: Theme.spacingSm

                Text {
                    text: model.title
                    font.pixelSize: Theme.fontSizeLg
                    font.bold: true
                    color: Theme.textPrimary
                }

                Text {
                    text: model.description
                    font.pixelSize: Theme.fontSizeSm
                    color: Theme.textSecondary
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                    maximumLineCount: 2
                    wrapMode: Text.WordWrap
                }

                RowLayout {
                    spacing: Theme.spacingLg

                    Text {
                        text: "截止：" + (model.deadline || "无截止日期")
                        font.pixelSize: Theme.fontSizeSm
                        color: Theme.warningColor
                    }

                    Text {
                        text: "满分：" + (model.maxScore || 100)
                        font.pixelSize: Theme.fontSizeSm
                        color: Theme.textSecondary
                    }

                    // 成绩显示
                    Text {
                        text: model.score >= 0 ? "得分：" + model.score : "未批改"
                        font.pixelSize: Theme.fontSizeSm
                        color: model.score >= 0 ? Theme.successColor : Theme.textSecondary
                        visible: !AuthService.isTeacher
                    }
                }

                // 操作按钮
                RowLayout {
                    spacing: Theme.spacingMd

                    // 学生：提价作业
                    Button {
                        visible: !AuthService.isTeacher
                        text: model.submitted ? "查看提交" : "提价作业"
                        font.pixelSize: Theme.fontSizeSm
                        contentItem: Text {
                            text: parent.text
                            color: Theme.textOnPrimary
                            font: parent.font
                        }
                        background: Rectangle {
                            radius: Theme.radiusSm
                            color: model.submitted ? Theme.successColor : Theme.primaryColor
                        }
                        onClicked: {
                            // 打开作业提价弹窗
                            var homeworkId = model.homeworkId
                            submissionDialog.homeworkId = homeworkId
                            submissionDialog.studentId = AuthService.currentUser.id
                            submissionDialog.open()
                        }
                    }

                    // 教师：批改作业
                    Button {
                        visible: AuthService.isTeacher
                        text: "批改作业"
                        font.pixelSize: Theme.fontSizeSm
                        contentItem: Text {
                            text: parent.text
                            color: Theme.textOnPrimary
                            font: parent.font
                        }
                        background: Rectangle {
                            radius: Theme.radiusSm
                            color: Theme.infoColor
                        }
                        onClicked: {
                            // 加载该作业的所有提交
                            var hwId = model.homeworkId
                            gradeDialog.hwId = hwId
                            HomeworkService.loadSubmissionsByHomework(hwId)
                            gradeDialog.open()
                        }
                    }
                }
            }
        }

        // 空状态
        Text {
            anchors.centerIn: parent
            visible: homeworkModel.count === 0
            text: "暂无作业\n联系教师获取作业信息"
            font.pixelSize: Theme.fontSizeMd
            color: Theme.textLight
            horizontalAlignment: Text.AlignHCenter
            lineHeight: 1.8
        }
    }

    // 作业提价对话框
    Dialog {
        id: submissionDialog
        property int homeworkId: 0
        property int studentId: 0
        title: "提价作业"
        modal: true
        width: 520
        height: 480

        HomeworkSubmission {
            anchors.fill: parent
            homeworkId: submissionDialog.homeworkId
            studentId: submissionDialog.studentId

            onSubmitRequested: {
                HomeworkService.submitHomework(homeworkId, studentId, content, fileUrl)
                submissionDialog.close()
            }
        }
    }

    // 批改对话框
    Dialog {
        id: gradeDialog
        property int hwId: 0
        title: "批改作业"
        modal: true
        width: 600
        height: 500

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: Theme.spacingLg
            spacing: Theme.spacingMd

            Text {
                text: "学生提价列表"
                font.pixelSize: Theme.fontSizeLg
                font.bold: true
                color: Theme.textPrimary
            }

            // 提交列表
            ListView {
                id: submissionList
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: Theme.spacingSm

                model: ListModel { id: submissionModel }

                delegate: Rectangle {
                    width: submissionList.width
                    height: 80
                    color: Theme.backgroundColor
                    radius: Theme.radiusSm

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: Theme.spacingMd
                        spacing: Theme.spacingMd

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: Theme.spacingXs

                            Text {
                                text: model.studentName + " 的提交"
                                font.pixelSize: Theme.fontSizeMd
                                font.bold: true
                                color: Theme.textPrimary
                            }

                            Text {
                                text: "提价时间：" + (model.submittedAt || "未知")
                                font.pixelSize: Theme.fontSizeSm
                                color: Theme.textSecondary
                            }

                            Text {
                                text: model.status === "graded" ? "得分：" + model.score : "待批改"
                                font.pixelSize: Theme.fontSizeSm
                                color: model.status === "graded" ? Theme.successColor : Theme.warningColor
                            }
                        }

                        // 批改按钮
                        Button {
                            text: model.status === "graded" ? "重新批改" : "批改"
                            font.pixelSize: Theme.fontSizeSm
                            onClicked: {
                                gradeSubmissionDialog.subId = model.submissionId
                                gradeSubmissionDialog.studentName = model.studentName
                                gradeSubmissionDialog.open()
                            }
                        }
                    }
                }
            }
        }
    }

    // 批改评分对话框
    Dialog {
        id: gradeSubmissionDialog
        property int subId: 0
        property string studentName: ""
        title: "批改 - " + studentName
        modal: true
        width: 400
        standardButtons: Dialog.Ok | Dialog.Cancel

        ColumnLayout {
            anchors.fill: parent
            spacing: Theme.spacingMd

            Text {
                text: "学生：" + gradeSubmissionDialog.studentName
                font.pixelSize: Theme.fontSizeMd
            }

            RowLayout {
                spacing: Theme.spacingMd

                Text { text: "评分：" }
                SpinBox {
                    id: scoreSpin
                    from: 0
                    to: 100
                    value: 80
                }
                Text { text: "/ 100" }
            }

            TextArea {
                id: commentArea
                Layout.fillWidth: true
                Layout.preferredHeight: 100
                placeholderText: "评语..."
            }
        }

        onAccepted: {
            HomeworkService.gradeSubmission(gradeSubmissionDialog.subId, scoreSpin.value, commentArea.text)
        }
    }

    // 加载作业数据
    Component.onCompleted: {
        if (AuthService.isTeacher) {
            HomeworkService.loadHomeworkByCourse(0)  // 教师查看所有
        } else {
            HomeworkService.loadSubmissionsByStudent(AuthService.currentUser.id)
        }
    }

    Connections {
        target: HomeworkService
        function onHomeworkLoaded(list) {
            homeworkModel.clear()
            for (var i = 0; i < list.length; i++) {
                var h = list[i]
                homeworkModel.append({
                    homeworkId: h.id,
                    title: h.title,
                    description: h.description,
                    deadline: h.deadline ? h.deadline.toLocaleString() : "",
                    maxScore: h.maxScore,
                    lessonTitle: h.lessonTitle,
                    submitted: false,
                    score: -1
                })
            }
        }

        function onSubmissionsLoaded(list) {
            if (!AuthService.isTeacher) {
                // 学生端显示
                homeworkModel.clear()
                for (var i = 0; i < list.length; i++) {
                    var s = list[i]
                    homeworkModel.append({
                        homeworkId: s.homeworkId,
                        title: "作业 #" + s.homeworkId,
                        description: s.content || "已提价",
                        deadline: "",
                        maxScore: 100,
                        submitted: true,
                        score: s.score,
                        studentName: s.studentName
                    })
                }
            } else {
                // 教师端显示提交列表
                submissionModel.clear()
                for (var j = 0; j < list.length; j++) {
                    var sub = list[j]
                    submissionModel.append({
                        submissionId: sub.id,
                        studentName: sub.studentName,
                        submittedAt: sub.submittedAt ? sub.submittedAt.toLocaleString() : "",
                        score: sub.score,
                        status: sub.status
                    })
                }
            }
        }

        function onSubmissionSubmitted(id) {
            console.log("Submission submitted:", id)
        }

        function onSubmissionGraded(id) {
            gradeSubmissionDialog.close()
            gradeDialog.close()
            HomeworkService.loadSubmissionsByHomework(gradeDialog.hwId)
        }
    }
}
