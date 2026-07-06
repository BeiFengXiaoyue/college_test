#include "playbackmanager.h"
#include <QDebug>

PlaybackManager::PlaybackManager(QObject *parent)
    : QObject(parent)
{
    m_player = new QMediaPlayer(this);

    connect(m_player, &QMediaPlayer::mediaStatusChanged,
            this, &PlaybackManager::onMediaStatusChanged);
    connect(m_player, &QMediaPlayer::durationChanged,
            this, &PlaybackManager::durationChanged);
    connect(m_player, &QMediaPlayer::positionChanged,
            this, &PlaybackManager::positionChanged);
    connect(m_player, &QMediaPlayer::playbackStateChanged,
            this, &PlaybackManager::isPlayingChanged);
    connect(m_player, &QMediaPlayer::positionChanged,
            this, [this](qint64 pos) {
        emit progressUpdated(pos, m_player->duration());
    });
}

PlaybackManager::~PlaybackManager()
{
    stop();
}

QMediaPlayer *PlaybackManager::player() const
{
    return m_player;
}

double PlaybackManager::playbackSpeed() const
{
    return m_player->playbackRate();
}

void PlaybackManager::setPlaybackSpeed(double speed)
{
    if (speed > 0.25 && speed <= 4.0) {
        m_player->setPlaybackRate(speed);
        emit playbackSpeedChanged();
    }
}

double PlaybackManager::volume() const
{
    return m_player->audioOutput()->volume();
}

void PlaybackManager::setVolume(double volume)
{
    m_player->audioOutput()->setVolume(volume);
    emit volumeChanged();
}

qint64 PlaybackManager::duration() const
{
    return m_player->duration();
}

qint64 PlaybackManager::position() const
{
    return m_player->position();
}

void PlaybackManager::setPosition(qint64 pos)
{
    m_player->setPosition(pos);
}

bool PlaybackManager::isPlaying() const
{
    return m_player->playbackState() == QMediaPlayer::PlayingState;
}

void PlaybackManager::play(const QString &videoUrl)
{
    if (videoUrl.isEmpty()) {
        emit playbackError("视频地址不能为空");
        return;
    }

    m_player->setSource(QUrl(videoUrl));
    m_player->play();
    emit playbackStarted();
    qDebug() << "[PlaybackManager] Playing:" << videoUrl;
}

void PlaybackManager::pause()
{
    m_player->pause();
    emit playbackPaused();
}

void PlaybackManager::resume()
{
    m_player->play();
}

void PlaybackManager::stop()
{
    m_player->stop();
    m_player->setSource(QUrl());
    emit playbackStopped();
}

void PlaybackManager::seekForward(qint64 seconds)
{
    qint64 newPos = m_player->position() + seconds * 1000;
    m_player->setPosition(qMin(newPos, m_player->duration()));
}

void PlaybackManager::seekBackward(qint64 seconds)
{
    qint64 newPos = m_player->position() - seconds * 1000;
    m_player->setPosition(qMax(newPos, 0LL));
}

void PlaybackManager::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    switch (status) {
    case QMediaPlayer::EndOfMedia:
        emit playbackFinished();
        qDebug() << "[PlaybackManager] Playback finished";
        break;
    case QMediaPlayer::InvalidMedia:
        emit playbackError("无效的媒体文件");
        break;
    case QMediaPlayer::LoadedMedia:
        emit durationChanged();
        break;
    default:
        break;
    }
}
