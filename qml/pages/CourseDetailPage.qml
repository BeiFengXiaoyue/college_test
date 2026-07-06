import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../theme"

/* ============================================================
   课程详情页
   ============================================================ */
Page {
    id: root
    property int courseId: 0
    property var courseData: null
    property var lessons: []

    background: Rectangle {
        color: Theme.backgroundColor
    }

    // 顶部
    Rectangle {
        id: topBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 60
        color: Theme.surfaceColor

        Rectangle {
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 1
            color: Theme.borderColor
        }

        RowLayout {
            anchors.fill: parent
            anchors.margins: Theme.spacingLg

            Button {
                text: "← 返回"
                flat: true
                font.pixelSize: Theme.fontSizeMd
                onClicked: mainLoader.setSource("pages/CourseListPage.qml")
            }

            Text {
                text: courseData ? courseData.title : "课程详情"
                font.pixelSize: Theme.fontSizeXl
                font.bold: true
                color: Theme.textPrimary
                elide: Text.ElideRight
                Layout.fillWidth: true
            }

            // 教师：添加课时
            Button {
                visible: AuthService.isTeacher
                text: "+ 添加课时"
                font.pixelSize: Theme.fontSizeSm
                contentItem: Text {
                    text: parent.text
                    color: Theme.textOnPrimary
                    font: parent.font
                }
                background: Rectangle {
                    radius: Theme.radiusMd
                    color: Theme.primaryColor
                }
                onClicked: createLessonDialog.open()
            }
        }
    }

    // 内容区
    ScrollView {
        anchors.top: topBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: Theme.spacingXl
        clip: true

        ColumnLayout {
            width: parent.width
            spacing: Theme.spacingXl

            // 课程信息卡片
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: columnInfo.height + Theme.spacingXxl * 2
                color: Theme.surfaceColor
                radius: Theme.radiusMd

                ColumnLayout {
                    id: columnInfo
                    anchors.fill: parent
                    anchors.margins: Theme.spacingXl
                    spacing: Theme.spacingMd

                    Text {
                        text: courseData ? courseData.title : ""
                        font.pixelSize: Theme.fontSizeXxl
                        font.bold: true
                        color: Theme.textPrimary
                    }

                    RowLayout {
                        spacing: Theme.spacingLg

                        Text {
                            text: "讲师：" + (courseData ? courseData.teacherName : "")
                            font.pixelSize: Theme.fontSizeMd
                            color: Theme.textSecondary
                        }

                        Text {
                            text: courseData && courseData.category ? "分类：" + courseData.category : ""
                            font.pixelSize: Theme.fontSizeMd
                            color: Theme.textSecondary
                        }
                    }

                    Text {
                        text: courseData ? courseData.description : ""
                        font.pixelSize: Theme.fontSizeMd
                        color: Theme.textPrimary
                        wrapMode: Text.WordWrap
                        Layout.fillWidth: true
                        lineHeight: 1.6
                    }
                }

                border.color: Theme.borderColor
                border.width: 1
            }

            // 课程序列表
            Text {
                text: "课程内容（" + lessons.length + "个课时）"
                font.pixelSize: Theme.fontSizeXl
                font.bold: true
                color: Theme.textPrimary
            }

            Repeater {
                model: lessons
                delegate: Rectangle {
                    width: parent.width
                    height: 80
                    color: Theme.surfaceColor
                    radius: Theme.radiusMd

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: Theme.spacingLg
                        spacing: Theme.spacingLg

                        // 序号
                        Rectangle {
                            Layout.preferredWidth: 40
                            Layout.preferredHeight: 40
                            radius: 20
                            color: Theme.primaryColor

                            Text {
                                anchors.centerIn: parent
                                text: (index + 1).toString()
                                font.bold: true
                                color: Theme.textOnPrimary
                            }
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: Theme.spacingXs

                            Text {
                                text: modelData.title
                                font.pixelSize: Theme.fontSizeMd
                                font.bold: true
                                color: Theme.textPrimary
                            }

                            RowLayout {
                                spacing: Theme.spacingMd

                                Text {
                                    text: modelData.type === "live" ? "🔴 直播" : "▶️ 录播"
                                    font.pixelSize: Theme.fontSizeSm
                                    color: modelData.type === "live" ? Theme.dangerColor : Theme.primaryColor
                                }

                                Text {
                                    text: modelData.duration > 0 ? formatDuration(modelData.duration) : ""
                                    font.pixelSize: Theme.fontSizeSm
                                    color: Theme.textSecondary
                                }

                                Text {
                                    text: modelData.startTime ? "时间：" + modelData.startTime : ""
                                    font.pixelSize: Theme.fontSizeSm
                                    color: Theme.textSecondary
                                    visible: modelData.type === "live"
                                }
                            }
                        }

                        // 进入按钮
                        Button {
                            text: modelData.type === "live" ? "进入直播" : "观看回放"
                            font.pixelSize: Theme.fontSizeSm
                            contentItem: Text {
                                text: parent.text
                                color: Theme.textOnPrimary
                                font: parent.font
                            }
                            background: Rectangle {
                                radius: Theme.radiusSm
                                color: Theme.primaryColor
                            }
                            onClicked: {
                                if (modelData.type === "live") {
                                    mainLoader.setSource("pages/LiveClassroomPage.qml", {
                                        lessonId: modelData.id,
                                        lessonTitle: modelData.title,
                                        videoUrl: modelData.videoUrl
                                    })
                                } else {
                                    mainLoader.setSource("pages/PlaybackPage.qml", {
                                        lessonId: modelData.id,
                                        lessonTitle: modelData.title,
                                        videoUrl: modelData.videoUrl
                                    })
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // 新建课时对话框
    Dialog {
        id: createLessonDialog
        title: "添加课时"
        standardButtons: Dialog.Ok | Dialog.Cancel
        modal: true
        width: 480

        ColumnLayout {
            anchors.fill: parent
            spacing: Theme.spacingMd

            TextField {
                id: lessonTitleField
                Layout.fillWidth: true
                placeholderText: "课时标题"
            }

            RowLayout {
                spacing: Theme.spacingMd

                Label { text: "类型：" }

                ComboBox {
                    id: lessonTypeCombo
                    model: ["replay", "live"]
                    onCurrentTextChanged: {
                        if (currentText === "replay") currentIndex = 0
                        else currentIndex = 1
                    }
                }
            }

            RowLayout {
                Label { text: "排序：" }
                SpinBox {
                    id: sortOrderSpin
                    from: 1
                    to: 100
                    value: lessons.length + 1
                }
            }
        }

        onAccepted: {
            CourseService.createLesson(courseId, lessonTitleField.text,
                                        lessonTypeCombo.currentText, sortOrderSpin.value)
        }
    }

    // 工具函数
    function formatDuration(seconds) {
        var h = Math.floor(seconds / 3600)
        var m = Math.floor((seconds % 3600) / 60)
        if (h > 0) return h + "时" + m + "分"
        return m + "分钟"
    }

    // 加载课程详情
    Component.onCompleted: {
        CourseService.loadCourseDetail(courseId)
    }

    Connections {
        target: CourseService
        function onCourseDetailLoaded(course, loadedLessons) {
            courseData = course
            lessons = loadedLessons
        }
        function onLessonCreated(lessonId) {
            createLessonDialog.close()
            CourseService.loadCourseDetail(courseId)
        }
    }
}
