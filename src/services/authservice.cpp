#include "authservice.h"
#include <QCryptographicHash>
#include <QtConcurrent>
#include <QDebug>

AuthService::AuthService(DatabaseManager *dbManager, QObject *parent)
    : QObject(parent)
    , m_dbManager(dbManager)
    , m_userDao(dbManager->database(), this)
{
}

UserModel AuthService::currentUser() const
{
    return m_currentUser;
}

bool AuthService::isLoggedIn() const
{
    return m_currentUser.isValid();
}

bool AuthService::isTeacher() const
{
    return m_currentUser.isTeacher();
}

void AuthService::login(const QString &username, const QString &password)
{
    QtConcurrent::run([this, username, password]() {
        QString hashedPwd = hashPassword(password);
        UserModel user = m_userDao.authenticate(username, hashedPwd);

        if (user.isValid()) {
            m_currentUser = user;
            emit currentUserChanged();
            emit loginStateChanged();
            emit loginSuccess(user.id);
            qDebug() << "[AuthService] User logged in:" << user.username << "role:" << user.role;
        } else {
            emit loginFailed("用户名或密码错误");
        }
    });
}

void AuthService::registerUser(const QString &username, const QString &password,
                                 const QString &displayName, const QString &role)
{
    // 同步注册 — 排除多线程信号传递问题
    if (m_userDao.usernameExists(username)) {
        emit registerFailed("用户名已存在");
        return;
    }

    UserModel user;
    user.username = username;
    user.passwordHash = hashPassword(password);
    user.displayName = displayName;
    user.role = role;

    if (m_userDao.insertUser(user)) {
        emit registerSuccess(user.id);
        qDebug() << "[AuthService] User registered:" << user.username;
    } else {
        emit registerFailed("注册失败，请稍后重试");
    }
}

void AuthService::logout()
{
    if (m_currentUser.isValid()) {
        qDebug() << "[AuthService] User logged out:" << m_currentUser.username;
        m_currentUser = UserModel();
        emit currentUserChanged();
        emit loginStateChanged();
    }
}

QString AuthService::hashPassword(const QString &password) const
{
    QByteArray data = password.toUtf8();
    QByteArray hash = QCryptographicHash::hash(data, QCryptographicHash::Sha256);
    return QString(hash.toHex());
}
