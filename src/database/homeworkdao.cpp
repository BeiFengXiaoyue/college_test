#include "homeworkdao.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

HomeworkDao::HomeworkDao(QSqlDatabase &db, QObject *parent)
    : QObject(parent), m_db(db)
{
}

// ==================== 作业布置 ====================

bool HomeworkDao::insertHomework(HomeworkModel &homework)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO homework (lesson_id, title, description, deadline, attachment_url, max_score)
        VALUES (:lesson_id, :title, :description, :deadline, :attachment_url, :max_score)
    )");
    query.bindValue(":lesson_id", homework.lessonId);
    query.bindValue(":title", homework.title);
    query.bindValue(":description", homework.description);
    query.bindValue(":deadline", homework.deadline);
    query.bindValue(":attachment_url", homework.attachmentUrl);
    query.bindValue(":max_score", homework.maxScore);

    if (!query.exec()) {
        qWarning() << "[HomeworkDao] insertHomework failed:" << query.lastError().text();
        return false;
    }

    homework.id = query.lastInsertId().toInt();
    emit homeworkInserted(homework.id);
    return true;
}

bool HomeworkDao::updateHomework(const HomeworkModel &homework)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        UPDATE homework SET title=:title, description=:description,
            deadline=:deadline, attachment_url=:attachment_url,
            max_score=:max_score, updated_at=CURRENT_TIMESTAMP
        WHERE id=:id
    )");
    query.bindValue(":id", homework.id);
    query.bindValue(":title", homework.title);
    query.bindValue(":description", homework.description);
    query.bindValue(":deadline", homework.deadline);
    query.bindValue(":attachment_url", homework.attachmentUrl);
    query.bindValue(":max_score", homework.maxScore);

    if (!query.exec()) {
        qWarning() << "[HomeworkDao] updateHomework failed:" << query.lastError().text();
        return false;
    }

    emit homeworkUpdated(homework.id);
    return true;
}

bool HomeworkDao::deleteHomework(int homeworkId)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM homework WHERE id=:id");
    query.bindValue(":id", homeworkId);

    if (!query.exec()) {
        qWarning() << "[HomeworkDao] deleteHomework failed:" << query.lastError().text();
        return false;
    }
    return true;
}

HomeworkModel HomeworkDao::getHomeworkById(int homeworkId)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT h.*, cl.title as lesson_title
        FROM homework h
        LEFT JOIN course_lessons cl ON h.lesson_id = cl.id
        WHERE h.id=:id
    )");
    query.bindValue(":id", homeworkId);

    HomeworkModel homework;
    if (query.exec() && query.next()) {
        homework.id = query.value("id").toInt();
        homework.lessonId = query.value("lesson_id").toInt();
        homework.lessonTitle = query.value("lesson_title").toString();
        homework.title = query.value("title").toString();
        homework.description = query.value("description").toString();
        homework.deadline = query.value("deadline").toDateTime();
        homework.attachmentUrl = query.value("attachment_url").toString();
        homework.maxScore = query.value("max_score").toInt();
        homework.createdAt = query.value("created_at").toDateTime();
        homework.updatedAt = query.value("updated_at").toDateTime();
    }
    return homework;
}

QList<HomeworkModel> HomeworkDao::getHomeworkByLesson(int lessonId)
{
    QList<HomeworkModel> list;
    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT h.*, cl.title as lesson_title
        FROM homework h
        LEFT JOIN course_lessons cl ON h.lesson_id = cl.id
        WHERE h.lesson_id=:lesson_id
        ORDER BY h.created_at DESC
    )");
    query.bindValue(":lesson_id", lessonId);

    if (query.exec()) {
        while (query.next()) {
            HomeworkModel hw;
            hw.id = query.value("id").toInt();
            hw.lessonId = query.value("lesson_id").toInt();
            hw.lessonTitle = query.value("lesson_title").toString();
            hw.title = query.value("title").toString();
            hw.description = query.value("description").toString();
            hw.deadline = query.value("deadline").toDateTime();
            hw.attachmentUrl = query.value("attachment_url").toString();
            hw.maxScore = query.value("max_score").toInt();
            hw.createdAt = query.value("created_at").toDateTime();
            hw.updatedAt = query.value("updated_at").toDateTime();
            list.append(hw);
        }
    }
    return list;
}

QList<HomeworkModel> HomeworkDao::getHomeworkByCourse(int courseId)
{
    QList<HomeworkModel> list;
    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT h.*, cl.title as lesson_title
        FROM homework h
        JOIN course_lessons cl ON h.lesson_id = cl.id
        WHERE cl.course_id=:course_id
        ORDER BY h.created_at DESC
    )");
    query.bindValue(":course_id", courseId);

    if (query.exec()) {
        while (query.next()) {
            HomeworkModel hw;
            hw.id = query.value("id").toInt();
            hw.lessonId = query.value("lesson_id").toInt();
            hw.lessonTitle = query.value("lesson_title").toString();
            hw.title = query.value("title").toString();
            hw.description = query.value("description").toString();
            hw.deadline = query.value("deadline").toDateTime();
            hw.attachmentUrl = query.value("attachment_url").toString();
            hw.maxScore = query.value("max_score").toInt();
            hw.createdAt = query.value("created_at").toDateTime();
            hw.updatedAt = query.value("updated_at").toDateTime();
            list.append(hw);
        }
    }
    return list;
}

// ==================== 作业提交 ====================

bool HomeworkDao::submitHomework(HomeworkSubmissionModel &submission)
{
    QSqlQuery query(m_db);
    // 检查是否已经提价过
    QSqlQuery checkQuery(m_db);
    checkQuery.prepare("SELECT id FROM homework_submissions WHERE homework_id=:hw_id AND student_id=:sid");
    checkQuery.bindValue(":hw_id", submission.homeworkId);
    checkQuery.bindValue(":sid", submission.studentId);
    if (checkQuery.exec() && checkQuery.next()) {
        // 更新已有提交
        submission.id = checkQuery.value(0).toInt();
        query.prepare(R"(
            UPDATE homework_submissions SET content=:content, file_url=:file_url,
                status='submitted', submitted_at=CURRENT_TIMESTAMP
            WHERE id=:id
        )");
        query.bindValue(":id", submission.id);
    } else {
        query.prepare(R"(
            INSERT INTO homework_submissions (homework_id, student_id, content, file_url, status)
            VALUES (:homework_id, :student_id, :content, :file_url, 'submitted')
        )");
        query.bindValue(":homework_id", submission.homeworkId);
        query.bindValue(":student_id", submission.studentId);
    }
    query.bindValue(":content", submission.content);
    query.bindValue(":file_url", submission.fileUrl);

    if (!query.exec()) {
        qWarning() << "[HomeworkDao] submitHomework failed:" << query.lastError().text();
        return false;
    }

    if (submission.id == 0) {
        submission.id = query.lastInsertId().toInt();
    }
    emit submissionReceived(submission.id);
    return true;
}

bool HomeworkDao::gradeHomework(int submissionId, int score, const QString &comment)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        UPDATE homework_submissions SET score=:score, comment=:comment,
            status='graded', graded_at=CURRENT_TIMESTAMP
        WHERE id=:id
    )");
    query.bindValue(":id", submissionId);
    query.bindValue(":score", score);
    query.bindValue(":comment", comment);

    if (!query.exec()) {
        qWarning() << "[HomeworkDao] gradeHomework failed:" << query.lastError().text();
        return false;
    }

    emit submissionGraded(submissionId);
    return true;
}

bool HomeworkDao::deleteSubmission(int submissionId)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM homework_submissions WHERE id=:id");
    query.bindValue(":id", submissionId);
    return query.exec();
}

HomeworkSubmissionModel HomeworkDao::getSubmissionById(int submissionId)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT s.*, u.display_name as student_name
        FROM homework_submissions s
        LEFT JOIN users u ON s.student_id = u.id
        WHERE s.id=:id
    )");
    query.bindValue(":id", submissionId);

    HomeworkSubmissionModel sub;
    if (query.exec() && query.next()) {
        sub.id = query.value("id").toInt();
        sub.homeworkId = query.value("homework_id").toInt();
        sub.studentId = query.value("student_id").toInt();
        sub.studentName = query.value("student_name").toString();
        sub.content = query.value("content").toString();
        sub.fileUrl = query.value("file_url").toString();
        sub.score = query.value("score").toInt();
        sub.comment = query.value("comment").toString();
        sub.status = query.value("status").toString();
        sub.submittedAt = query.value("submitted_at").toDateTime();
        sub.gradedAt = query.value("graded_at").toDateTime();
    }
    return sub;
}

HomeworkSubmissionModel HomeworkDao::getSubmission(int homeworkId, int studentId)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT s.*, u.display_name as student_name
        FROM homework_submissions s
        LEFT JOIN users u ON s.student_id = u.id
        WHERE s.homework_id=:hw_id AND s.student_id=:sid
    )");
    query.bindValue(":hw_id", homeworkId);
    query.bindValue(":sid", studentId);

    HomeworkSubmissionModel sub;
    if (query.exec() && query.next()) {
        sub.id = query.value("id").toInt();
        sub.homeworkId = query.value("homework_id").toInt();
        sub.studentId = query.value("student_id").toInt();
        sub.studentName = query.value("student_name").toString();
        sub.content = query.value("content").toString();
        sub.fileUrl = query.value("file_url").toString();
        sub.score = query.value("score").toInt();
        sub.comment = query.value("comment").toString();
        sub.status = query.value("status").toString();
        sub.submittedAt = query.value("submitted_at").toDateTime();
        sub.gradedAt = query.value("graded_at").toDateTime();
    }
    return sub;
}

QList<HomeworkSubmissionModel> HomeworkDao::getSubmissionsByHomework(int homeworkId)
{
    QList<HomeworkSubmissionModel> list;
    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT s.*, u.display_name as student_name
        FROM homework_submissions s
        LEFT JOIN users u ON s.student_id = u.id
        WHERE s.homework_id=:hw_id
        ORDER BY s.submitted_at DESC
    )");
    query.bindValue(":hw_id", homeworkId);

    if (query.exec()) {
        while (query.next()) {
            HomeworkSubmissionModel sub;
            sub.id = query.value("id").toInt();
            sub.homeworkId = query.value("homework_id").toInt();
            sub.studentId = query.value("student_id").toInt();
            sub.studentName = query.value("student_name").toString();
            sub.content = query.value("content").toString();
            sub.fileUrl = query.value("file_url").toString();
            sub.score = query.value("score").toInt();
            sub.comment = query.value("comment").toString();
            sub.status = query.value("status").toString();
            sub.submittedAt = query.value("submitted_at").toDateTime();
            sub.gradedAt = query.value("graded_at").toDateTime();
            list.append(sub);
        }
    }
    return list;
}

QList<HomeworkSubmissionModel> HomeworkDao::getSubmissionsByStudent(int studentId)
{
    QList<HomeworkSubmissionModel> list;
    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT s.*, u.display_name as student_name
        FROM homework_submissions s
        LEFT JOIN users u ON s.student_id = u.id
        WHERE s.student_id=:sid
        ORDER BY s.submitted_at DESC
    )");
    query.bindValue(":sid", studentId);

    if (query.exec()) {
        while (query.next()) {
            HomeworkSubmissionModel sub;
            sub.id = query.value("id").toInt();
            sub.homeworkId = query.value("homework_id").toInt();
            sub.studentId = query.value("student_id").toInt();
            sub.studentName = query.value("student_name").toString();
            sub.content = query.value("content").toString();
            sub.fileUrl = query.value("file_url").toString();
            sub.score = query.value("score").toInt();
            sub.comment = query.value("comment").toString();
            sub.status = query.value("status").toString();
            sub.submittedAt = query.value("submitted_at").toDateTime();
            sub.gradedAt = query.value("graded_at").toDateTime();
            list.append(sub);
        }
    }
    return list;
}
