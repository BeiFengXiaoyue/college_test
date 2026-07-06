#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>
#include <QJsonObject>
#include <QVariant>

class ConfigManager : public QObject
{
    Q_OBJECT

public:
    explicit ConfigManager(QObject *parent = nullptr);

    void load(const QString &filePath = QString());
    void save(const QString &filePath = QString());

    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;
    void setValue(const QString &key, const QVariant &value);

    QString configFilePath() const;

signals:
    void configChanged(const QString &key, const QVariant &value);

private:
    QJsonObject m_config;
    QString m_configFilePath;

    void setDefaultConfig();
};

#endif // CONFIGMANAGER_H
