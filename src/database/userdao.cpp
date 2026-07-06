#include "userdao.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

UserDao::UserDao(QSqlDatabase &db, QObject *parent)
    : QObject(parent), m_db(db)
{
}

bool UserDao::insertUser(UserModel &user)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO users (username, password_hash, display_name, role, avatar, email)
        VALUES (:username, :password_hash, :display_name, :role, :avatar, :email)
    )");
    query.bindValue(":username", user.username);
    query.bindValue(":password_hash", user.passwordHash);
    query.bindValue(":display_name", user.displayName);
    query.bindValue(":role", user.role);
    query.bindValue(":avatar", user.avatar);
    query.bindValue(":email", user.email);

    if (!query.exec()) {
        qWarning() << "[UserDao] insertUser failed:" << query.lastError().text();
        return false;
    }

    user.id = query.lastInsertId().toInt();
    emit userInserted(user.id);
    return true;
}

bool UserDao::updateUser(const UserModel &user)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        UPDATE users SET username=:username, display_name=:display_name,
            role=:role, avatar=:avatar, email=:email, updated_at=CURRENT_TIMESTAMP
        WHERE id=:id
    )");
    query.bindValue(":id", user.id);
    query.bindValue(":username", user.username);
    query.bindValue(":display_name", user.displayName);
    query.bindValue(":role", user.role);
    query.bindValue(":avatar", user.avatar);
    query.bindValue(":email", user.email);

    if (!query.exec()) {
        qWarning() << "[UserDao] updateUser failed:" << query.lastError().text();
        return false;
    }

    emit userUpdated(user.id);
    return true;
}

bool UserDao::deleteUser(int userId)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM users WHERE id=:id");
    query.bindValue(":id", userId);

    if (!query.exec()) {
        qWarning() << "[UserDao] deleteUser failed:" << query.lastError().text();
        return false;
    }

    emit userDeleted(userId);
    return true;
}

UserModel UserDao::getUserById(int userId)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM users WHERE id=:id");
    query.bindValue(":id", userId);

    UserModel user;
    if (query.exec() && query.next()) {
        user.id = query.value("id").toInt();
        user.username = query.value("username").toString();
        user.passwordHash = query.value("password_hash").toString();
        user.displayName = query.value("display_name").toString();
        user.role = query.value("role").toString();
        user.avatar = query.value("avatar").toString();
        user.email = query.value("email").toString();
        user.createdAt = query.value("created_at").toDateTime();
        user.updatedAt = query.value("updated_at").toDateTime();
    }
    return user;
}

UserModel UserDao::getUserByUsername(const QString &username)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM users WHERE username=:username");
    query.bindValue(":username", username);

    UserModel user;
    if (query.exec() && query.next()) {
        user.id = query.value("id").toInt();
        user.username = query.value("username").toString();
        user.passwordHash = query.value("password_hash").toString();
        user.displayName = query.value("display_name").toString();
        user.role = query.value("role").toString();
        user.avatar = query.value("avatar").toString();
        user.email = query.value("email").toString();
        user.createdAt = query.value("created_at").toDateTime();
        user.updatedAt = query.value("updated_at").toDateTime();
    }
    return user;
}

QList<UserModel> UserDao::getAllUsers()
{
    QList<UserModel> users;
    QSqlQuery query(m_db);

    if (query.exec("SELECT * FROM users ORDER BY id")) {
        while (query.next()) {
            UserModel user;
            user.id = query.value("id").toInt();
            user.username = query.value("username").toString();
            user.passwordHash = query.value("password_hash").toString();
            user.displayName = query.value("display_name").toString();
            user.role = query.value("role").toString();
            user.avatar = query.value("avatar").toString();
            user.email = query.value("email").toString();
            user.createdAt = query.value("created_at").toDateTime();
            user.updatedAt = query.value("updated_at").toDateTime();
            users.append(user);
        }
    }
    return users;
}

QList<UserModel> UserDao::getUsersByRole(const QString &role)
{
    QList<UserModel> users;
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM users WHERE role=:role ORDER BY id");
    query.bindValue(":role", role);

    if (query.exec()) {
        while (query.next()) {
            UserModel user;
            user.id = query.value("id").toInt();
            user.username = query.value("username").toString();
            user.passwordHash = query.value("password_hash").toString();
            user.displayName = query.value("display_name").toString();
            user.role = query.value("role").toString();
            user.avatar = query.value("avatar").toString();
            user.email = query.value("email").toString();
            user.createdAt = query.value("created_at").toDateTime();
            user.updatedAt = query.value("updated_at").toDateTime();
            users.append(user);
        }
    }
    return users;
}

UserModel UserDao::authenticate(const QString &username, const QString &passwordHash)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM users WHERE username=:username AND password_hash=:password_hash");
    query.bindValue(":username", username);
    query.bindValue(":password_hash", passwordHash);

    UserModel user;
    if (query.exec() && query.next()) {
        user.id = query.value("id").toInt();
        user.username = query.value("username").toString();
        user.passwordHash = query.value("password_hash").toString();
        user.displayName = query.value("display_name").toString();
        user.role = query.value("role").toString();
        user.avatar = query.value("avatar").toString();
        user.email = query.value("email").toString();
        user.createdAt = query.value("created_at").toDateTime();
        user.updatedAt = query.value("updated_at").toDateTime();
    }
    return user;
}

bool UserDao::usernameExists(const QString &username)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT COUNT(*) FROM users WHERE username=:username");
    query.bindValue(":username", username);

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}
