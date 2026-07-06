#include "homeworkservice.h"
#include <QtConcurrent>
#include <QDebug>

HomeworkService::HomeworkService(DatabaseManager *dbManager, QObject *parent)
    : QObject(parent)
    , m_dbManager(dbManager)
    , m_homeworkDao(dbManager->database(), this)
{
}

void HomeworkService::createHomework(int lessonId, const QString &title,
                                      const QString &description, const QDateTime &deadline)
{
    QtConcurrent::run([this, lessonId, title, description, deadline]() {
        HomeworkModel hw;
        hw.lessonId = lessonId;
        hw.title = title;
        hw.description = description;
        hw.deadline = deadline;

        if (m_homeworkDao.insertHomework(hw)) {
            emit homeworkCreated(hw.id);
        } else {
            emit operationFailed("创建作业失败");
        }
    });
}

void HomeworkService::loadHomeworkByLesson(int lessonId)
{
    QtConcurrent::run([this, lessonId]() {
        auto list = m_homeworkDao.getHomeworkByLesson(lessonId);
        emit homeworkLoaded(list);
    });
}

void HomeworkService::loadHomeworkByCourse(int courseId)
{
    QtConcurrent::run([this, courseId]() {
        auto list = m_homeworkDao.getHomeworkByCourse(courseId);
        emit homeworkLoaded(list);
    });
}

void HomeworkService::submitHomework(int homeworkId, int studentId,
                                      const QString &content, const QString &fileUrl)
{
    QtConcurrent::run([this, homeworkId, studentId, content, fileUrl]() {
        HomeworkSubmissionModel sub;
        sub.homeworkId = homeworkId;
        sub.studentId = studentId;
        sub.content = content;
        sub.fileUrl = fileUrl;

        if (m_homeworkDao.submitHomework(sub)) {
            emit submissionSubmitted(sub.id);
        } else {
            emit operationFailed("提价作业失败");
        }
    });
}

void HomeworkService::gradeSubmission(int submissionId, int score, const QString &comment)
{
    QtConcurrent::run([this, submissionId, score, comment]() {
        if (m_homeworkDao.gradeHomework(submissionId, score, comment)) {
            emit submissionGraded(submissionId);
        } else {
            emit operationFailed("批改作业失败");
        }
    });
}

void HomeworkService::loadSubmissionsByHomework(int homeworkId)
{
    QtConcurrent::run([this, homeworkId]() {
        auto list = m_homeworkDao.getSubmissionsByHomework(homeworkId);
        emit submissionsLoaded(list);
    });
}

void HomeworkService::loadSubmissionsByStudent(int studentId)
{
    QtConcurrent::run([this, studentId]() {
        auto list = m_homeworkDao.getSubmissionsByStudent(studentId);
        emit submissionsLoaded(list);
    });
}
