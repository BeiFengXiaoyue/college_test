QT += quick quickcontrols2 multimedia multimediawidgets sql concurrent network

CONFIG += c++17 qtquickcompiler

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/main.cpp \
    src/core/appcore.cpp \
    src/core/threadpoolmanager.cpp \
    src/core/configmanager.cpp \
    src/core/processmanager.cpp \
    src/database/databasemanager.cpp \
    src/database/userdao.cpp \
    src/database/coursedao.cpp \
    src/database/homeworkdao.cpp \
    src/database/learningrecorddao.cpp \
    src/media/livestreammanager.cpp \
    src/media/playbackmanager.cpp \
    src/media/streamrecorder.cpp \
    src/services/authservice.cpp \
    src/services/courseservice.cpp \
    src/services/homeworkservice.cpp \
    src/services/livestreamservice.cpp

HEADERS += \
    src/core/appcore.h \
    src/core/threadpoolmanager.h \
    src/core/configmanager.h \
    src/core/processmanager.h \
    src/database/databasemanager.h \
    src/database/userdao.h \
    src/database/coursedao.h \
    src/database/homeworkdao.h \
    src/database/learningrecorddao.h \
    src/media/livestreammanager.h \
    src/media/playbackmanager.h \
    src/media/streamrecorder.h \
    src/models/usermodel.h \
    src/models/coursemodel.h \
    src/models/homeworkmodel.h \
    src/models/learningrecordmodel.h \
    src/services/authservice.h \
    src/services/courseservice.h \
    src/services/homeworkservice.h \
    src/services/livestreamservice.h

RESOURCES += resources/resources.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import paths used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    qml/main.qml \
    qml/components/Whiteboard.qml \
    qml/components/DanmakuOverlay.qml \
    qml/components/VideoPlayer.qml \
    qml/components/LiveStreamView.qml \
    qml/components/HomeworkSubmission.qml \
    qml/pages/LoginPage.qml \
    qml/pages/DashboardPage.qml \
    qml/pages/CourseListPage.qml \
    qml/pages/CourseDetailPage.qml \
    qml/pages/LiveClassroomPage.qml \
    qml/pages/PlaybackPage.qml \
    qml/pages/HomeworkPage.qml \
    qml/pages/RegisterPage.qml \
    qml/theme/Theme.qml \
    qml/theme/qmldir \
    resources/sql/init.sql
