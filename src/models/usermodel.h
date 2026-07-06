#ifndef USERMODEL_H
#define USERMODEL_H

#include <QString>
#include <QDateTime>

struct UserModel
{
    int id = 0;
    QString username;
    QString passwordHash;
    QString displayName;
    QString role;             // "teacher" | "student"
    QString avatar;
    QString email;
    QDateTime createdAt;
    QDateTime updatedAt;

    bool isValid() const { return id > 0; }
    bool isTeacher() const { return role == "teacher"; }
    bool isStudent() const { return role == "student"; }
};

#endif // USERMODEL_H
