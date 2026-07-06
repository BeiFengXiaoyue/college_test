import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../theme"

/* ============================================================
   课程列表页
   ============================================================ */
Page {
    id: root
    property string filter: ""

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
                onClicked: mainLoader.setSource("pages/DashboardPage.qml")
            }

            Text {
                text: "课程列表"
                font.pixelSize: Theme.fontSizeXl
                font.bold: true
                color: Theme.textPrimary
                Layout.fillWidth: true
            }

            // 搜索框
            TextField {
                id: searchField
                Layout.preferredWidth: 240
                Layout.preferredHeight: 36
                placeholderText: "搜索课程..."
                font.pixelSize: Theme.fontSizeMd
                leftPadding: 12

                background: Rectangle {
                    radius: Theme.radiusSm
                    color: Theme.backgroundColor
                    border.color: searchField.activeFocus ? Theme.primaryColor : Theme.borderColor
                    border.width: 1
                }

                onAccepted: CourseService.searchCourses(searchField.text)
            }

            // 新建课程（教师）
            Button {
                visible: AuthService.isTeacher
                text: "+ 新建课程"
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
                onClicked: createCourseDialog.open()
            }
        }
    }

    // 课程列表
    ListView {
        id: courseList
        anchors.top: topBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: Theme.spacingXl
        spacing: Theme.spacingLg
        clip: true

        model: ListModel { id: courseListModel }

        delegate: Rectangle {
            width: courseList.width
            height: 120
            color: Theme.surfaceColor
            radius: Theme.radiusMd

            border.color: Theme.borderColor
            border.width: 1

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: mainLoader.setSource("pages/CourseDetailPage.qml", {courseId: model.id})
            }

            RowLayout {
                anchors.fill: parent
                anchors.margins: Theme.spacingLg
                spacing: Theme.spacingLg

                // 封面占位
                Rectangle {
                    Layout.preferredWidth: 160
                    Layout.preferredHeight: 90
                    radius: Theme.radiusSm
                    color: Theme.primaryLight

                    Text {
                        anchors.centerIn: parent
                        text: "📚"
                        font.pixelSize: 36
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: Theme.spacingXs

                    Text {
                        text: model.title
                        font.pixelSize: Theme.fontSizeLg
                        font.bold: true
                        color: Theme.textPrimary
                        elide: Text.ElideRight
                    }

                    Text {
                        text: model.teacherName ? "讲师：" + model.teacherName : ""
                        font.pixelSize: Theme.fontSizeSm
                        color: Theme.textSecondary
                    }

                    Text {
                        text: model.description
                        font.pixelSize: Theme.fontSizeSm
                        color: Theme.textSecondary
                        elide: Text.ElideRight
                        maximumLineCount: 2
                        wrapMode: Text.WordWrap
                        Layout.fillWidth: true
                    }

                    RowLayout {
                        spacing: Theme.spacingMd

                        Rectangle {
                            visible: model.status === "published"
                            height: 22
                            width: labelText.width + 16
                            radius: 11
                            color: Theme.successColor

                            Text {
                                id: labelText
                                anchors.centerIn: parent
                                text: "已发布"
                                font.pixelSize: 10
                                color: Theme.textOnPrimary
                            }
                        }

                        Text {
                            text: model.category
                            font.pixelSize: Theme.fontSizeXs
                            color: Theme.textLight
                            visible: model.category !== ""
                        }
                    }
                }
            }
        }

        // 空状态
        Text {
            anchors.centerIn: parent
            visible: courseListModel.count === 0
            text: "暂无课程\n点击右上角创建第一门课程吧！"
            font.pixelSize: Theme.fontSizeMd
            color: Theme.textLight
            horizontalAlignment: Text.AlignHCenter
            lineHeight: 1.8
        }
    }

    // 新建课程对话框
    Dialog {
        id: createCourseDialog
        title: "新建课程"
        standardButtons: Dialog.Ok | Dialog.Cancel
        modal: true
        width: 480

        ColumnLayout {
            anchors.fill: parent
            spacing: Theme.spacingMd

            TextField {
                id: courseTitleField
                Layout.fillWidth: true
                placeholderText: "课程标题"
            }

            TextArea {
                id: courseDescField
                Layout.fillWidth: true
                Layout.preferredHeight: 80
                placeholderText: "课程描述"
            }

            TextField {
                id: courseCategoryField
                Layout.fillWidth: true
                placeholderText: "分类（如：编程、设计）"
            }
        }

        onAccepted: {
            CourseService.createCourse(courseTitleField.text, courseDescField.text,
                                        AuthService.currentUser.id, courseCategoryField.text)
        }
    }

    // 课程数据加载
    Component.onCompleted: {
        if (AuthService.isTeacher) {
            CourseService.loadCoursesByTeacher(AuthService.currentUser.id)
        } else {
            CourseService.loadPublishedCourses()
        }
    }

    Connections {
        target: CourseService
        function onCoursesLoaded(courses) {
            courseListModel.clear()
            for (var i = 0; i < courses.length; i++) {
                var c = courses[i]
                courseListModel.append({
                    id: c.id,
                    title: c.title,
                    description: c.description,
                    teacherName: c.teacherName,
                    status: c.status,
                    category: c.category
                })
            }
        }
        function onCourseCreated(courseId) {
            createCourseDialog.close()
            CourseService.loadCoursesByTeacher(AuthService.currentUser.id)
        }
    }
}
