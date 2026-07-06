#include "databasemanager.h"
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject(parent)
{
    m_connectionName = "edu_platform_main";
}

DatabaseManager::~DatabaseManager()
{
    close();
}

bool DatabaseManager::open(const QString &dbPath)
{
    if (m_db.isOpen()) {
        close();
    }

    m_db = QSqlDatabase::addDatabase("QSQLITE", m_connectionName);
    m_db.setDatabaseName(dbPath);

    if (!m_db.open()) {
        m_lastError = m_db.lastError().text();
        qCritical() << "[DatabaseManager] Failed to open database:" << m_lastError;
        emit errorOccurred(m_lastError);
        return false;
    }

    // 启用 WAL 模式提升并发性能
    QSqlQuery query(m_db);
    query.exec("PRAGMA journal_mode=WAL");
    query.exec("PRAGMA foreign_keys=ON");

    qDebug() << "[DatabaseManager] Database opened:" << dbPath;
    emit databaseOpened(dbPath);
    return true;
}

void DatabaseManager::close()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
    if (QSqlDatabase::connectionNames().contains(m_connectionName)) {
        QSqlDatabase::removeDatabase(m_connectionName);
    }
    emit databaseClosed();
}

bool DatabaseManager::isOpen() const
{
    return m_db.isOpen();
}

bool DatabaseManager::initializeSchema()
{
    // 从资源文件加载 SQL 初始化脚本
    QString sqlPath = ":/resources/sql/init.sql";
    QFile file(sqlPath);

    // 如果资源文件不存在，回退到文件系统路径
    if (!file.exists()) {
        sqlPath = QCoreApplication::applicationDirPath() + "/../resources/sql/init.sql";
        qDebug() << "[DatabaseManager] SQL file not in resources, trying:" << sqlPath;
    }

    bool ok = executeSqlFile(sqlPath);
    if (!ok) {
        qWarning() << "[DatabaseManager] Failed to initialize database schema!";
        m_lastError = "无法加载数据库初始化脚本";
    }
    return ok;
}

QSqlDatabase &DatabaseManager::database()
{
    return m_db;
}

QString DatabaseManager::lastError() const
{
    return m_lastError;
}

bool DatabaseManager::executeSqlFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "[DatabaseManager] Cannot open SQL file:" << filePath;
        return false;
    }

    QTextStream stream(&file);
    QString sql = stream.readAll();
    file.close();

    return executeSql(sql);
}

bool DatabaseManager::executeSql(const QString &sql)
{
    // 按分号分割多条语句
    QStringList statements = sql.split(';', Qt::SkipEmptyParts);
    bool allOk = true;

    for (const QString &stmt : statements) {
        QString trimmed = stmt.trimmed();
        if (trimmed.isEmpty() || trimmed.startsWith("--")) {
            continue;
        }

        QSqlQuery query(m_db);
        if (!query.exec(trimmed)) {
            m_lastError = query.lastError().text();
            qWarning() << "[DatabaseManager] SQL error:" << m_lastError;
            qWarning() << "[DatabaseManager] Statement:" << trimmed.left(200);
            allOk = false;
            // 继续执行后续语句
        }
    }

    return allOk;
}
