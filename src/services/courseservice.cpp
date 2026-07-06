#include "courseservice.h"
#include <QtConcurrent>
#include <QDebug>

CourseService::CourseService(DatabaseManager *dbManager, QObject *parent)
    : QObject(parent)
    , m_dbManager(dbManager)
    , m_courseDao(dbManager->database(), this)
    , m_learningRecordDao(dbManager->database(), this)
{
}

void CourseService::createCourse(const QString &title, const QString &description,
                                  int teacherId, const QString &category)
{
    QtConcurrent::run([this, title, description, teacherId, category]() {
        CourseModel course;
        course.title = title;
        course.description = description;
        course.teacherId = teacherId;
        course.category = category;
        course.status = "draft";

        if (m_courseDao.insertCourse(course)) {
            emit courseCreated(course.id);
            qDebug() << "[CourseService] Course created:" << course.id << course.title;
        } else {
            emit operationFailed("创建课程失败");
        }
    });
}

void CourseService::updateCourse(int courseId, const QString &title,
                                  const QString &description, const QString &status)
{
    QtConcurrent::run([this, courseId, title, description, status]() {
        CourseModel course = m_courseDao.getCourseById(courseId);
        if (!course.isValid()) {
            emit operationFailed("课程不存在");
            return;
        }
        course.title = title;
        course.description = description;
        course.status = status;

        if (m_courseDao.updateCourse(course)) {
            emit courseUpdated(courseId);
        } else {
            emit operationFailed("更新课程失败");
        }
    });
}

void CourseService::deleteCourse(int courseId)
{
    QtConcurrent::run([this, courseId]() {
        if (m_courseDao.deleteCourse(courseId)) {
            emit courseDeleted(courseId);
        } else {
            emit operationFailed("删除课程失败");
        }
    });
}

void CourseService::loadPublishedCourses()
{
    QtConcurrent::run([this]() {
        auto courses = m_courseDao.getPublishedCourses();
        emit coursesLoaded(courses);
    });
}

void CourseService::loadCoursesByTeacher(int teacherId)
{
    QtConcurrent::run([this, teacherId]() {
        auto courses = m_courseDao.getCoursesByTeacher(teacherId);
        emit coursesLoaded(courses);
    });
}

void CourseService::loadCourseDetail(int courseId)
{
    QtConcurrent::run([this, courseId]() {
        CourseModel course = m_courseDao.getCourseById(courseId);
        auto lessons = m_courseDao.getLessonsByCourse(courseId);
        emit courseDetailLoaded(course, lessons);
    });
}

void CourseService::searchCourses(const QString &keyword)
{
    QtConcurrent::run([this, keyword]() {
        auto courses = m_courseDao.searchCourses(keyword);
        emit coursesLoaded(courses);
    });
}

void CourseService::createLesson(int courseId, const QString &title,
                                  const QString &type, int sortOrder)
{
    QtConcurrent::run([this, courseId, title, type, sortOrder]() {
        CourseLessonModel lesson;
        lesson.courseId = courseId;
        lesson.title = title;
        lesson.type = type;
        lesson.sortOrder = sortOrder;
        lesson.status = "draft";

        if (m_courseDao.insertLesson(lesson)) {
            emit lessonCreated(lesson.id);
        } else {
            emit operationFailed("创建课时失败");
        }
    });
}

void CourseService::loadLessons(int courseId)
{
    QtConcurrent::run([this, courseId]() {
        auto lessons = m_courseDao.getLessonsByCourse(courseId);
        emit lessonsLoaded(lessons);
    });
}

void CourseService::updateProgress(int userId, int lessonId, double progress, int position)
{
    QtConcurrent::run([this, userId, lessonId, progress, position]() {
        if (m_learningRecordDao.updateProgress(userId, lessonId, progress, position)) {
            emit progressUpdated(userId, lessonId);
        }
    });
}

void CourseService::loadLearningRecords(int userId)
{
    QtConcurrent::run([this, userId]() {
        auto records = m_learningRecordDao.getRecordsByUser(userId);
        emit learningRecordsLoaded(records);
    });
}
