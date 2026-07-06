#ifndef COURSESERVICE_H
#define COURSESERVICE_H

#include <QObject>
#include <QList>
#include "../database/databasemanager.h"
#include "../database/coursedao.h"
#include "../database/learningrecorddao.h"
#include "../models/coursemodel.h"
#include "../models/learningrecordmodel.h"

class CourseService : public QObject
{
    Q_OBJECT

public:
    explicit CourseService(DatabaseManager *dbManager, QObject *parent = nullptr);

    // 课程操作
    Q_INVOKABLE void createCourse(const QString &title, const QString &description,
                                   int teacherId, const QString &category);
    Q_INVOKABLE void updateCourse(int courseId, const QString &title,
                                   const QString &description, const QString &status);
    Q_INVOKABLE void deleteCourse(int courseId);

    Q_INVOKABLE void loadPublishedCourses();
    Q_INVOKABLE void loadCoursesByTeacher(int teacherId);
    Q_INVOKABLE void loadCourseDetail(int courseId);
    Q_INVOKABLE void searchCourses(const QString &keyword);

    // 课时操作
    Q_INVOKABLE void createLesson(int courseId, const QString &title,
                                   const QString &type, int sortOrder);
    Q_INVOKABLE void loadLessons(int courseId);

    // 学习进度
    Q_INVOKABLE void updateProgress(int userId, int lessonId, double progress, int position);
    Q_INVOKABLE void loadLearningRecords(int userId);

signals:
    // 课程信号
    void courseCreated(int courseId);
    void courseUpdated(int courseId);
    void courseDeleted(int courseId);
    void coursesLoaded(const QList<CourseModel> &courses);
    void courseDetailLoaded(const CourseModel &course, const QList<CourseLessonModel> &lessons);
    void operationFailed(const QString &error);

    // 课时信号
    void lessonCreated(int lessonId);
    void lessonsLoaded(const QList<CourseLessonModel> &lessons);

    // 学习记录信号
    void progressUpdated(int userId, int lessonId);
    void learningRecordsLoaded(const QList<LearningRecordModel> &records);

private:
    DatabaseManager *m_dbManager;
    CourseDao m_courseDao;
    LearningRecordDao m_learningRecordDao;
};

#endif // COURSESERVICE_H
