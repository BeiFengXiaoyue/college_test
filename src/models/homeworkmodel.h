#ifndef HOMEWORKMODEL_H
#define HOMEWORKMODEL_H

#include <QString>
#include <QDateTime>

struct HomeworkModel
{
    int id = 0;
    int lessonId = 0;
    QString lessonTitle;     // JOIN 查询结果
    QString title;
    QString description;
    QDateTime deadline;
    QString attachmentUrl;
    int maxScore = 100;
    QDateTime createdAt;
    QDateTime updatedAt;

    bool isValid() const { return id > 0; }
    bool isDeadlinePassed() const { return deadline.isValid() && QDateTime::currentDateTime() > deadline; }
};

struct HomeworkSubmissionModel
{
    int id = 0;
    int homeworkId = 0;
    int studentId = 0;
    QString studentName;     // JOIN 查询结果
    QString content;
    QString fileUrl;
    int score = -1;          // -1 表示未批改
    QString comment;
    QString status;          // "submitted" | "graded"
    QDateTime submittedAt;
    QDateTime gradedAt;

    bool isValid() const { return id > 0; }
    bool isGraded() const { return status == "graded"; }
    bool hasScore() const { return score >= 0; }
};

#endif // HOMEWORKMODEL_H
