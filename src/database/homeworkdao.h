#ifndef HOMEWORKDAO_H
#define HOMEWORKDAO_H

#include <QObject>
#include <QSqlDatabase>
#include <QList>
#include "../models/homeworkmodel.h"

class HomeworkDao : public QObject
{
    Q_OBJECT

public:
    explicit HomeworkDao(QSqlDatabase &db, QObject *parent = nullptr);

    // 作业布置 CRUD
    bool insertHomework(HomeworkModel &homework);
    bool updateHomework(const HomeworkModel &homework);
    bool deleteHomework(int homeworkId);

    HomeworkModel getHomeworkById(int homeworkId);
    QList<HomeworkModel> getHomeworkByLesson(int lessonId);
    QList<HomeworkModel> getHomeworkByCourse(int courseId);

    // 作业提交 CRUD
    bool submitHomework(HomeworkSubmissionModel &submission);
    bool gradeHomework(int submissionId, int score, const QString &comment);
    bool deleteSubmission(int submissionId);

    HomeworkSubmissionModel getSubmissionById(int submissionId);
    HomeworkSubmissionModel getSubmission(int homeworkId, int studentId);
    QList<HomeworkSubmissionModel> getSubmissionsByHomework(int homeworkId);
    QList<HomeworkSubmissionModel> getSubmissionsByStudent(int studentId);

signals:
    void homeworkInserted(int homeworkId);
    void homeworkUpdated(int homeworkId);
    void submissionReceived(int submissionId);
    void submissionGraded(int submissionId);

private:
    QSqlDatabase &m_db;
};

#endif // HOMEWORKDAO_H
