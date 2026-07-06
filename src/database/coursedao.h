#ifndef COURSEDAO_H
#define COURSEDAO_H

#include <QObject>
#include <QSqlDatabase>
#include <QList>
#include "../models/coursemodel.h"

class CourseDao : public QObject
{
    Q_OBJECT

public:
    explicit CourseDao(QSqlDatabase &db, QObject *parent = nullptr);

    // 课程 CRUD
    bool insertCourse(CourseModel &course);
    bool updateCourse(const CourseModel &course);
    bool deleteCourse(int courseId);

    CourseModel getCourseById(int courseId);
    QList<CourseModel> getCoursesByTeacher(int teacherId);
    QList<CourseModel> getPublishedCourses();
    QList<CourseModel> getAllCourses();
    QList<CourseModel> searchCourses(const QString &keyword);

    // 课时 CRUD
    bool insertLesson(CourseLessonModel &lesson);
    bool updateLesson(const CourseLessonModel &lesson);
    bool deleteLesson(int lessonId);

    CourseLessonModel getLessonById(int lessonId);
    QList<CourseLessonModel> getLessonsByCourse(int courseId);

signals:
    void courseInserted(int courseId);
    void courseUpdated(int courseId);
    void courseDeleted(int courseId);
    void lessonInserted(int lessonId);

private:
    QSqlDatabase &m_db;

    UserModel rowToUser(const QSqlQuery &query);
};

#endif // COURSEDAO_H
