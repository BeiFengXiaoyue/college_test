#include "livestreamservice.h"
#include <QDebug>

LiveStreamService::LiveStreamService(QObject *parent)
    : QObject(parent)
{
}

LiveStreamManager *LiveStreamService::createStream()
{
    auto *manager = new LiveStreamManager(this);
    m_streams.append(manager);
    return manager;
}

void LiveStreamService::startStream(LiveStreamManager *manager, const QString &url)
{
    if (manager) {
        manager->startStream(url);
        emit streamStarted(url);
        qDebug() << "[LiveStreamService] Stream started:" << url;
    }
}

void LiveStreamService::stopStream(LiveStreamManager *manager)
{
    if (manager) {
        manager->stopStream();
        emit streamStopped(manager->streamUrl());
        qDebug() << "[LiveStreamService] Stream stopped";
    }
}
