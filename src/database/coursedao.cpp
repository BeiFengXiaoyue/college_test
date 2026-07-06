#include "coursedao.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

CourseDao::CourseDao(QSqlDatabase &db, QObject *parent)
    : QObject(parent), m_db(db)
{
}

// ==================== 课程 ====================

bool CourseDao::insertCourse(CourseModel &course)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO courses (title, description, teacher_id, cover, status, category)
        VALUES (:title, :description, :teacher_id, :cover, :status, :category)
    )");
    query.bindValue(":title", course.title);
    query.bindValue(":description", course.description);
    query.bindValue(":teacher_id", course.teacherId);
    query.bindValue(":cover", course.cover);
    query.bindValue(":status", course.status);
    query.bindValue(":category", course.category);

    if (!query.exec()) {
        qWarning() << "[CourseDao] insertCourse failed:" << query.lastError().text();
        return false;
    }

    course.id = query.lastInsertId().toInt();
    emit courseInserted(course.id);
    return true;
}

bool CourseDao::updateCourse(const CourseModel &course)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        UPDATE courses SET title=:title, description=:description,
            cover=:cover, status=:status, category=:category, updated_at=CURRENT_TIMESTAMP
        WHERE id=:id
    )");
    query.bindValue(":id", course.id);
    query.bindValue(":title", course.title);
    query.bindValue(":description", course.description);
    query.bindValue(":cover", course.cover);
    query.bindValue(":status", course.status);
    query.bindValue(":category", course.category);

    if (!query.exec()) {
        qWarning() << "[CourseDao] updateCourse failed:" << query.lastError().text();
        return false;
    }

    emit courseUpdated(course.id);
    return true;
}

bool CourseDao::deleteCourse(int courseId)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM courses WHERE id=:id");
    query.bindValue(":id", courseId);

    if (!query.exec()) {
        qWarning() << "[CourseDao] deleteCourse failed:" << query.lastError().text();
        return false;
    }

    emit courseDeleted(courseId);
    return true;
}

CourseModel CourseDao::getCourseById(int courseId)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT c.*, u.display_name as teacher_name
        FROM courses c
        LEFT JOIN users u ON c.teacher_id = u.id
        WHERE c.id=:id
    )");
    query.bindValue(":id", courseId);

    CourseModel course;
    if (query.exec() && query.next()) {
        course.id = query.value("id").toInt();
        course.title = query.value("title").toString();
        course.description = query.value("description").toString();
        course.teacherId = query.value("teacher_id").toInt();
        course.teacherName = query.value("teacher_name").toString();
        course.cover = query.value("cover").toString();
        course.status = query.value("status").toString();
        course.category = query.value("category").toString();
        course.createdAt = query.value("created_at").toDateTime();
        course.updatedAt = query.value("updated_at").toDateTime();
    }
    return course;
}

QList<CourseModel> CourseDao::getCoursesByTeacher(int teacherId)
{
    QList<CourseModel> courses;
    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT c.*, u.display_name as teacher_name
        FROM courses c
        LEFT JOIN users u ON c.teacher_id = u.id
        WHERE c.teacher_id=:teacher_id
        ORDER BY c.updated_at DESC
    )");
    query.bindValue(":teacher_id", teacherId);

    if (query.exec()) {
        while (query.next()) {
            CourseModel course;
            course.id = query.value("id").toInt();
            course.title = query.value("title").toString();
            course.description = query.value("description").toString();
            course.teacherId = query.value("teacher_id").toInt();
            course.teacherName = query.value("teacher_name").toString();
            course.cover = query.value("cover").toString();
            course.status = query.value("status").toString();
            course.category = query.value("category").toString();
            course.createdAt = query.value("created_at").toDateTime();
            course.updatedAt = query.value("updated_at").toDateTime();
            courses.append(course);
        }
    }
    return courses;
}

QList<CourseModel> CourseDao::getPublishedCourses()
{
    QList<CourseModel> courses;
    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT c.*, u.display_name as teacher_name
        FROM courses c
        LEFT JOIN users u ON c.teacher_id = u.id
        WHERE c.status='published'
        ORDER BY c.updated_at DESC
    )");

    if (query.exec()) {
        while (query.next()) {
            CourseModel course;
            course.id = query.value("id").toInt();
            course.title = query.value("title").toString();
            course.description = query.value("description").toString();
            course.teacherId = query.value("teacher_id").toInt();
            course.teacherName = query.value("teacher_name").toString();
            course.cover = query.value("cover").toString();
            course.status = query.value("status").toString();
            course.category = query.value("category").toString();
            course.createdAt = query.value("created_at").toDateTime();
            course.updatedAt = query.value("updated_at").toDateTime();
            courses.append(course);
        }
    }
    return courses;
}

QList<CourseModel> CourseDao::getAllCourses()
{
    QList<CourseModel> courses;
    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT c.*, u.display_name as teacher_name
        FROM courses c
        LEFT JOIN users u ON c.teacher_id = u.id
        ORDER BY c.updated_at DESC
    )");

    if (query.exec()) {
        while (query.next()) {
            CourseModel course;
            course.id = query.value("id").toInt();
            course.title = query.value("title").toString();
            course.description = query.value("description").toString();
            course.teacherId = query.value("teacher_id").toInt();
            course.teacherName = query.value("teacher_name").toString();
            course.cover = query.value("cover").toString();
            course.status = query.value("status").toString();
            course.category = query.value("category").toString();
            course.createdAt = query.value("created_at").toDateTime();
            course.updatedAt = query.value("updated_at").toDateTime();
            courses.append(course);
        }
    }
    return courses;
}

QList<CourseModel> CourseDao::searchCourses(const QString &keyword)
{
    QList<CourseModel> courses;
    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT c.*, u.display_name as teacher_name
        FROM courses c
        LEFT JOIN users u ON c.teacher_id = u.id
        WHERE c.title LIKE :keyword OR c.description LIKE :keyword
        ORDER BY c.updated_at DESC
    )");
    query.bindValue(":keyword", "%" + keyword + "%");

    if (query.exec()) {
        while (query.next()) {
            CourseModel course;
            course.id = query.value("id").toInt();
            course.title = query.value("title").toString();
            course.description = query.value("description").toString();
            course.teacherId = query.value("teacher_id").toInt();
            course.teacherName = query.value("teacher_name").toString();
            course.cover = query.value("cover").toString();
            course.status = query.value("status").toString();
            course.category = query.value("category").toString();
            course.createdAt = query.value("created_at").toDateTime();
            course.updatedAt = query.value("updated_at").toDateTime();
            courses.append(course);
        }
    }
    return courses;
}

// ==================== 课时 ====================

bool CourseDao::insertLesson(CourseLessonModel &lesson)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO course_lessons (course_id, title, description, video_url, type, duration, start_time, sort_order, status)
        VALUES (:course_id, :title, :description, :video_url, :type, :duration, :start_time, :sort_order, :status)
    )");
    query.bindValue(":course_id", lesson.courseId);
    query.bindValue(":title", lesson.title);
    query.bindValue(":description", lesson.description);
    query.bindValue(":video_url", lesson.videoUrl);
    query.bindValue(":type", lesson.type);
    query.bindValue(":duration", lesson.duration);
    query.bindValue(":start_time", lesson.startTime);
    query.bindValue(":sort_order", lesson.sortOrder);
    query.bindValue(":status", lesson.status);

    if (!query.exec()) {
        qWarning() << "[CourseDao] insertLesson failed:" << query.lastError().text();
        return false;
    }

    lesson.id = query.lastInsertId().toInt();
    emit lessonInserted(lesson.id);
    return true;
}

bool CourseDao::updateLesson(const CourseLessonModel &lesson)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        UPDATE course_lessons SET title=:title, description=:description,
            video_url=:video_url, type=:type, duration=:duration,
            start_time=:start_time, sort_order=:sort_order, status=:status,
            updated_at=CURRENT_TIMESTAMP
        WHERE id=:id
    )");
    query.bindValue(":id", lesson.id);
    query.bindValue(":title", lesson.title);
    query.bindValue(":description", lesson.description);
    query.bindValue(":video_url", lesson.videoUrl);
    query.bindValue(":type", lesson.type);
    query.bindValue(":duration", lesson.duration);
    query.bindValue(":start_time", lesson.startTime);
    query.bindValue(":sort_order", lesson.sortOrder);
    query.bindValue(":status", lesson.status);

    if (!query.exec()) {
        qWarning() << "[CourseDao] updateLesson failed:" << query.lastError().text();
        return false;
    }
    return true;
}

bool CourseDao::deleteLesson(int lessonId)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM course_lessons WHERE id=:id");
    query.bindValue(":id", lessonId);

    if (!query.exec()) {
        qWarning() << "[CourseDao] deleteLesson failed:" << query.lastError().text();
        return false;
    }
    return true;
}

CourseLessonModel CourseDao::getLessonById(int lessonId)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM course_lessons WHERE id=:id");
    query.bindValue(":id", lessonId);

    CourseLessonModel lesson;
    if (query.exec() && query.next()) {
        lesson.id = query.value("id").toInt();
        lesson.courseId = query.value("course_id").toInt();
        lesson.title = query.value("title").toString();
        lesson.description = query.value("description").toString();
        lesson.videoUrl = query.value("video_url").toString();
        lesson.type = query.value("type").toString();
        lesson.duration = query.value("duration").toInt();
        lesson.startTime = query.value("start_time").toDateTime();
        lesson.sortOrder = query.value("sort_order").toInt();
        lesson.status = query.value("status").toString();
        lesson.createdAt = query.value("created_at").toDateTime();
        lesson.updatedAt = query.value("updated_at").toDateTime();
    }
    return lesson;
}

QList<CourseLessonModel> CourseDao::getLessonsByCourse(int courseId)
{
    QList<CourseLessonModel> lessons;
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM course_lessons WHERE course_id=:course_id ORDER BY sort_order ASC");
    query.bindValue(":course_id", courseId);

    if (query.exec()) {
        while (query.next()) {
            CourseLessonModel lesson;
            lesson.id = query.value("id").toInt();
            lesson.courseId = query.value("course_id").toInt();
            lesson.title = query.value("title").toString();
            lesson.description = query.value("description").toString();
            lesson.videoUrl = query.value("video_url").toString();
            lesson.type = query.value("type").toString();
            lesson.duration = query.value("duration").toInt();
            lesson.startTime = query.value("start_time").toDateTime();
            lesson.sortOrder = query.value("sort_order").toInt();
            lesson.status = query.value("status").toString();
            lesson.createdAt = query.value("created_at").toDateTime();
            lesson.updatedAt = query.value("updated_at").toDateTime();
            lessons.append(lesson);
        }
    }
    return lessons;
}
