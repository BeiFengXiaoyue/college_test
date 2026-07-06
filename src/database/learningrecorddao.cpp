#include "learningrecorddao.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

LearningRecordDao::LearningRecordDao(QSqlDatabase &db, QObject *parent)
    : QObject(parent), m_db(db)
{
}

bool LearningRecordDao::updateProgress(int userId, int lessonId, double progress, int position)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO learning_records (user_id, lesson_id, progress, last_watch_position, last_watch_time, completed)
        VALUES (:uid, :lid, :progress, :pos, CURRENT_TIMESTAMP, :completed)
        ON CONFLICT(user_id, lesson_id) DO UPDATE SET
            progress=:progress2, last_watch_position=:pos2,
            last_watch_time=CURRENT_TIMESTAMP, completed=:completed2
    )");
    bool completed = progress >= 0.95;
    query.bindValue(":uid", userId);
    query.bindValue(":lid", lessonId);
    query.bindValue(":progress", progress);
    query.bindValue(":progress2", progress);
    query.bindValue(":pos", position);
    query.bindValue(":pos2", position);
    query.bindValue(":completed", completed ? 1 : 0);
    query.bindValue(":completed2", completed ? 1 : 0);

    if (!query.exec()) {
        qWarning() << "[LearningRecordDao] updateProgress failed:" << query.lastError().text();
        return false;
    }

    emit progressUpdated(userId, lessonId);
    return true;
}

LearningRecordModel LearningRecordDao::getRecord(int userId, int lessonId)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT lr.*, cl.title as lesson_title, c.title as course_title
        FROM learning_records lr
        LEFT JOIN course_lessons cl ON lr.lesson_id = cl.id
        LEFT JOIN courses c ON cl.course_id = c.id
        WHERE lr.user_id=:uid AND lr.lesson_id=:lid
    )");
    query.bindValue(":uid", userId);
    query.bindValue(":lid", lessonId);

    LearningRecordModel record;
    if (query.exec() && query.next()) {
        record.id = query.value("id").toInt();
        record.userId = query.value("user_id").toInt();
        record.lessonId = query.value("lesson_id").toInt();
        record.lessonTitle = query.value("lesson_title").toString();
        record.courseTitle = query.value("course_title").toString();
        record.progress = query.value("progress").toDouble();
        record.watchDuration = query.value("watch_duration").toInt();
        record.lastWatchPosition = query.value("last_watch_position").toInt();
        record.lastWatchTime = query.value("last_watch_time").toDateTime();
        record.completed = query.value("completed").toBool();
    }
    return record;
}

QList<LearningRecordModel> LearningRecordDao::getRecordsByUser(int userId)
{
    QList<LearningRecordModel> records;
    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT lr.*, cl.title as lesson_title, c.title as course_title
        FROM learning_records lr
        LEFT JOIN course_lessons cl ON lr.lesson_id = cl.id
        LEFT JOIN courses c ON cl.course_id = c.id
        WHERE lr.user_id=:uid
        ORDER BY lr.last_watch_time DESC
    )");
    query.bindValue(":uid", userId);

    if (query.exec()) {
        while (query.next()) {
            LearningRecordModel record;
            record.id = query.value("id").toInt();
            record.userId = query.value("user_id").toInt();
            record.lessonId = query.value("lesson_id").toInt();
            record.lessonTitle = query.value("lesson_title").toString();
            record.courseTitle = query.value("course_title").toString();
            record.progress = query.value("progress").toDouble();
            record.watchDuration = query.value("watch_duration").toInt();
            record.lastWatchPosition = query.value("last_watch_position").toInt();
            record.lastWatchTime = query.value("last_watch_time").toDateTime();
            record.completed = query.value("completed").toBool();
            records.append(record);
        }
    }
    return records;
}

QList<LearningRecordModel> LearningRecordDao::getRecordsByLesson(int lessonId)
{
    QList<LearningRecordModel> records;
    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT lr.*, cl.title as lesson_title, c.title as course_title
        FROM learning_records lr
        LEFT JOIN course_lessons cl ON lr.lesson_id = cl.id
        LEFT JOIN courses c ON cl.course_id = c.id
        WHERE lr.lesson_id=:lid
        ORDER BY lr.last_watch_time DESC
    )");
    query.bindValue(":lid", lessonId);

    if (query.exec()) {
        while (query.next()) {
            LearningRecordModel record;
            record.id = query.value("id").toInt();
            record.userId = query.value("user_id").toInt();
            record.lessonId = query.value("lesson_id").toInt();
            record.lessonTitle = query.value("lesson_title").toString();
            record.courseTitle = query.value("course_title").toString();
            record.progress = query.value("progress").toDouble();
            record.watchDuration = query.value("watch_duration").toInt();
            record.lastWatchPosition = query.value("last_watch_position").toInt();
            record.lastWatchTime = query.value("last_watch_time").toDateTime();
            record.completed = query.value("completed").toBool();
            records.append(record);
        }
    }
    return records;
}

// ==================== 弹幕 ====================

bool LearningRecordDao::saveDanmaku(const DanmakuMessageModel &danmaku)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO danmaku_messages (lesson_id, user_id, content, timestamp, color, font_size, position)
        VALUES (:lid, :uid, :content, :ts, :color, :fs, :pos)
    )");
    query.bindValue(":lid", danmaku.lessonId);
    query.bindValue(":uid", danmaku.userId);
    query.bindValue(":content", danmaku.content);
    query.bindValue(":ts", danmaku.timestamp);
    query.bindValue(":color", danmaku.color);
    query.bindValue(":fs", danmaku.fontSize);
    query.bindValue(":pos", danmaku.position);

    if (!query.exec()) {
        qWarning() << "[LearningRecordDao] saveDanmaku failed:" << query.lastError().text();
        return false;
    }

    emit danmakuSaved(query.lastInsertId().toInt());
    return true;
}

QList<DanmakuMessageModel> LearningRecordDao::getDanmakuByLesson(int lessonId, int afterTimestamp)
{
    QList<DanmakuMessageModel> list;
    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT d.*, u.display_name as user_name
        FROM danmaku_messages d
        LEFT JOIN users u ON d.user_id = u.id
        WHERE d.lesson_id=:lid AND d.timestamp>=:ts
        ORDER BY d.timestamp ASC
    )");
    query.bindValue(":lid", lessonId);
    query.bindValue(":ts", afterTimestamp);

    if (query.exec()) {
        while (query.next()) {
            DanmakuMessageModel dm;
            dm.id = query.value("id").toInt();
            dm.lessonId = query.value("lesson_id").toInt();
            dm.userId = query.value("user_id").toInt();
            dm.userName = query.value("user_name").toString();
            dm.content = query.value("content").toString();
            dm.timestamp = query.value("timestamp").toInt();
            dm.color = query.value("color").toString();
            dm.fontSize = query.value("font_size").toInt();
            dm.position = query.value("position").toString();
            dm.createdAt = query.value("created_at").toDateTime();
            list.append(dm);
        }
    }
    return list;
}

// ==================== 白板 ====================

bool LearningRecordDao::saveWhiteboardAction(const WhiteboardRecordModel &record)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO whiteboard_records (lesson_id, user_id, action_data, action_type, timestamp)
        VALUES (:lid, :uid, :data, :type, :ts)
    )");
    query.bindValue(":lid", record.lessonId);
    query.bindValue(":uid", record.userId);
    query.bindValue(":data", record.actionData);
    query.bindValue(":type", record.actionType);
    query.bindValue(":ts", record.timestamp);

    if (!query.exec()) {
        qWarning() << "[LearningRecordDao] saveWhiteboardAction failed:" << query.lastError().text();
        return false;
    }
    return true;
}

QList<WhiteboardRecordModel> LearningRecordDao::getWhiteboardRecords(int lessonId)
{
    QList<WhiteboardRecordModel> list;
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM whiteboard_records WHERE lesson_id=:lid ORDER BY timestamp ASC");
    query.bindValue(":lid", lessonId);

    if (query.exec()) {
        while (query.next()) {
            WhiteboardRecordModel record;
            record.id = query.value("id").toInt();
            record.lessonId = query.value("lesson_id").toInt();
            record.userId = query.value("user_id").toInt();
            record.actionData = query.value("action_data").toString();
            record.actionType = query.value("action_type").toString();
            record.timestamp = query.value("timestamp").toInt();
            record.createdAt = query.value("created_at").toDateTime();
            list.append(record);
        }
    }
    return list;
}
