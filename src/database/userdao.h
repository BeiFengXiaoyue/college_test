#ifndef USERDAO_H
#define USERDAO_H

#include <QObject>
#include <QSqlDatabase>
#include <QList>
#include "../models/usermodel.h"

class UserDao : public QObject
{
    Q_OBJECT

public:
    explicit UserDao(QSqlDatabase &db, QObject *parent = nullptr);

    // CRUD
    bool insertUser(UserModel &user);
    bool updateUser(const UserModel &user);
    bool deleteUser(int userId);

    // 查询
    UserModel getUserById(int userId);
    UserModel getUserByUsername(const QString &username);
    QList<UserModel> getAllUsers();
    QList<UserModel> getUsersByRole(const QString &role);

    // 认证
    UserModel authenticate(const QString &username, const QString &passwordHash);
    bool usernameExists(const QString &username);

signals:
    void userInserted(int userId);
    void userUpdated(int userId);
    void userDeleted(int userId);

private:
    QSqlDatabase &m_db;
};

#endif // USERDAO_H
