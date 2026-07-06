#ifndef LIVESTREAMSERVICE_H
#define LIVESTREAMSERVICE_H

#include <QObject>
#include "../media/livestreammanager.h"

class LiveStreamService : public QObject
{
    Q_OBJECT

public:
    explicit LiveStreamService(QObject *parent = nullptr);

    Q_INVOKABLE LiveStreamManager *createStream();
    Q_INVOKABLE void startStream(LiveStreamManager *manager, const QString &url);
    Q_INVOKABLE void stopStream(LiveStreamManager *manager);

signals:
    void streamStarted(const QString &url);
    void streamStopped(const QString &url);
    void streamError(const QString &error);

private:
    QList<LiveStreamManager *> m_streams;
};

#endif // LIVESTREAMSERVICE_H
