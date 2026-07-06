#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class DatabaseManager : public QObject
{
    Q_OBJECT

public:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();

    bool open(const QString &dbPath);
    void close();
    bool isOpen() const;

    bool initializeSchema();

    QSqlDatabase &database();
    QString lastError() const;

signals:
    void databaseOpened(const QString &path);
    void databaseClosed();
    void errorOccurred(const QString &error);

private:
    bool executeSqlFile(const QString &filePath);
    bool executeSql(const QString &sql);

    QSqlDatabase m_db;
    QString m_lastError;
    QString m_connectionName;
};

#endif // DATABASEMANAGER_H
