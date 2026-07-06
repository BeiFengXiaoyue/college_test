#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include <QObject>
#include <QFuture>
#include "../database/databasemanager.h"
#include "../database/userdao.h"
#include "../models/usermodel.h"

class AuthService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(UserModel currentUser READ currentUser NOTIFY currentUserChanged)
    Q_PROPERTY(bool loggedIn READ isLoggedIn NOTIFY loginStateChanged)
    Q_PROPERTY(bool isTeacher READ isTeacher NOTIFY loginStateChanged)

public:
    explicit AuthService(DatabaseManager *dbManager, QObject *parent = nullptr);

    UserModel currentUser() const;
    bool isLoggedIn() const;
    bool isTeacher() const;

    // 异步登录/注册
    Q_INVOKABLE void login(const QString &username, const QString &password);
    Q_INVOKABLE void registerUser(const QString &username, const QString &password,
                                   const QString &displayName, const QString &role);
    Q_INVOKABLE void logout();

signals:
    void currentUserChanged();
    void loginStateChanged();
    void loginSuccess(int userId);
    void loginFailed(const QString &error);
    void registerSuccess(int userId);
    void registerFailed(const QString &error);

private:
    QString hashPassword(const QString &password) const;

    DatabaseManager *m_dbManager;
    UserDao m_userDao;
    UserModel m_currentUser;
};

#endif // AUTHSERVICE_H
