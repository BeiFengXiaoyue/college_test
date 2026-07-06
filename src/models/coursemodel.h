#ifndef COURSEMODEL_H
#define COURSEMODEL_H

#include <QString>
#include <QDateTime>
#include <QList>

struct CourseModel
{
    int id = 0;
    QString title;
    QString description;
    int teacherId = 0;
    QString teacherName;     // JOIN 查询结果
    QString cover;
    QString status;          // "draft" | "published" | "archived"
    QString category;
    QDateTime createdAt;
    QDateTime updatedAt;

    bool isValid() const { return id > 0; }
    bool isPublished() const { return status == "published"; }
};

struct CourseLessonModel
{
    int id = 0;
    int courseId = 0;
    QString title;
    QString description;
    QString videoUrl;
    QString type;            // "live" | "replay"
    int duration = 0;        // 秒
    QDateTime startTime;
    int sortOrder = 0;
    QString status;          // "draft" | "published" | "archived"
    QDateTime createdAt;
    QDateTime updatedAt;

    bool isValid() const { return id > 0; }
    bool isLive() const { return type == "live"; }
    bool isReplay() const { return type == "replay"; }
};

struct CourseWithLessons
{
    CourseModel course;
    QList<CourseLessonModel> lessons;
};

#endif // COURSEMODEL_H
