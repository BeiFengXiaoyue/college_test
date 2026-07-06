#ifndef LEARNINGRECORDMODEL_H
#define LEARNINGRECORDMODEL_H

#include <QString>
#include <QDateTime>

struct LearningRecordModel
{
    int id = 0;
    int userId = 0;
    int lessonId = 0;
    QString lessonTitle;       // JOIN 查询结果
    QString courseTitle;       // JOIN 查询结果
    double progress = 0.0;     // 0.0 ~ 1.0
    int watchDuration = 0;     // 秒
    int lastWatchPosition = 0; // 秒
    QDateTime lastWatchTime;
    bool completed = false;
};

struct DanmakuMessageModel
{
    int id = 0;
    int lessonId = 0;
    int userId = 0;
    QString userName;       // JOIN 查询结果
    QString content;
    int timestamp = 0;      // 毫秒
    QString color;
    int fontSize = 24;
    QString position;       // "scroll" | "top" | "bottom"
    QDateTime createdAt;
};

struct WhiteboardRecordModel
{
    int id = 0;
    int lessonId = 0;
    int userId = 0;
    QString actionData;     // JSON 序列化的绘制操作
    QString actionType;     // "draw" | "erase" | "clear" | "undo"
    int timestamp = 0;
    QDateTime createdAt;
};

#endif // LEARNINGRECORDMODEL_H
