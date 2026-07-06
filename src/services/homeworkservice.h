#ifndef HOMEWORKSERVICE_H
#define HOMEWORKSERVICE_H

#include <QObject>
#include <QList>
#include "../database/databasemanager.h"
#include "../database/homeworkdao.h"
#include "../models/homeworkmodel.h"

class HomeworkService : public QObject
{
    Q_OBJECT

public:
    explicit HomeworkService(DatabaseManager *dbManager, QObject *parent = nullptr);

    // 作业布置
    Q_INVOKABLE void createHomework(int lessonId, const QString &title,
                                     const QString &description, const QDateTime &deadline);
    Q_INVOKABLE void loadHomeworkByLesson(int lessonId);
    Q_INVOKABLE void loadHomeworkByCourse(int courseId);

    // 作业提交
    Q_INVOKABLE void submitHomework(int homeworkId, int studentId,
                                     const QString &content, const QString &fileUrl);
    Q_INVOKABLE void gradeSubmission(int submissionId, int score, const QString &comment);
    Q_INVOKABLE void loadSubmissionsByHomework(int homeworkId);
    Q_INVOKABLE void loadSubmissionsByStudent(int studentId);

signals:
    void homeworkCreated(int homeworkId);
    void homeworkLoaded(const QList<HomeworkModel> &homeworkList);
    void submissionSubmitted(int submissionId);
    void submissionGraded(int submissionId);
    void submissionsLoaded(const QList<HomeworkSubmissionModel> &submissions);
    void operationFailed(const QString &error);

private:
    DatabaseManager *m_dbManager;
    HomeworkDao m_homeworkDao;
};

#endif // HOMEWORKSERVICE_H
