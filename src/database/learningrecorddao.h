#ifndef LEARNINGRECORDDAO_H
#define LEARNINGRECORDDAO_H

#include <QObject>
#include <QSqlDatabase>
#include <QList>
#include "../models/learningrecordmodel.h"

class LearningRecordDao : public QObject
{
    Q_OBJECT

public:
    explicit LearningRecordDao(QSqlDatabase &db, QObject *parent = nullptr);

    // 学习记录
    bool updateProgress(int userId, int lessonId, double progress, int position);
    LearningRecordModel getRecord(int userId, int lessonId);
    QList<LearningRecordModel> getRecordsByUser(int userId);
    QList<LearningRecordModel> getRecordsByLesson(int lessonId);

    // 弹幕
    bool saveDanmaku(const DanmakuMessageModel &danmaku);
    QList<DanmakuMessageModel> getDanmakuByLesson(int lessonId, int afterTimestamp = 0);

    // 白板记录
    bool saveWhiteboardAction(const WhiteboardRecordModel &record);
    QList<WhiteboardRecordModel> getWhiteboardRecords(int lessonId);

signals:
    void progressUpdated(int userId, int lessonId);
    void danmakuSaved(int messageId);

private:
    QSqlDatabase &m_db;
};

#endif // LEARNINGRECORDDAO_H
