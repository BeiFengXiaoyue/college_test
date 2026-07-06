#include "configmanager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <QThreadPool>

ConfigManager::ConfigManager(QObject *parent)
    : QObject(parent)
{
    setDefaultConfig();
}

void ConfigManager::setDefaultConfig()
{
    m_config["database/path"] = "edu_platform.db";
    m_config["threadPool/maxCount"] = QThreadPool::globalInstance()->maxThreadCount();
    m_config["media/rtmpUrl"] = "rtmp://localhost/live/";
    m_config["media/recordPath"] = "records/";
}

void ConfigManager::load(const QString &filePath)
{
    QString path = filePath;
    if (path.isEmpty()) {
        path = QCoreApplication::applicationDirPath() + QDir::separator() + "config.json";
    }
    m_configFilePath = path;

    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (doc.isObject()) {
            m_config = doc.object();
            qDebug() << "[ConfigManager] Loaded config from:" << path;
        }
        file.close();
    } else {
        qDebug() << "[ConfigManager] No config file found, using defaults. Path:" << path;
        save(path);
    }
}

void ConfigManager::save(const QString &filePath)
{
    QString path = filePath.isEmpty() ? m_configFilePath : filePath;
    if (path.isEmpty()) {
        path = QCoreApplication::applicationDirPath() + QDir::separator() + "config.json";
    }

    QFile file(path);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(m_config);
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
        qDebug() << "[ConfigManager] Saved config to:" << path;
    }
}

QVariant ConfigManager::value(const QString &key, const QVariant &defaultValue) const
{
    // 支持点分隔路径: "database/path"
    QStringList keys = key.split('/');
    QJsonObject obj = m_config;
    for (int i = 0; i < keys.size() - 1; ++i) {
        if (obj.contains(keys[i]) && obj[keys[i]].isObject()) {
            obj = obj[keys[i]].toObject();
        } else {
            return defaultValue;
        }
    }
    if (obj.contains(keys.last())) {
        return obj[keys.last()].toVariant();
    }
    return defaultValue;
}

void ConfigManager::setValue(const QString &key, const QVariant &value)
{
    // 简单实现：直接存储平铺键
    m_config[key] = QJsonValue::fromVariant(value);
    emit configChanged(key, value);
}

QString ConfigManager::configFilePath() const
{
    return m_configFilePath;
}
