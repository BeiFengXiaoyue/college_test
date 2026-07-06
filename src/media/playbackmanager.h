#ifndef PLAYBACKMANAGER_H
#define PLAYBACKMANAGER_H

#include <QObject>
#include <QMediaPlayer>
#include <QVideoSink>

class PlaybackManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QMediaPlayer *player READ player CONSTANT)
    Q_PROPERTY(double playbackSpeed READ playbackSpeed WRITE setPlaybackSpeed NOTIFY playbackSpeedChanged)
    Q_PROPERTY(double volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(qint64 position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY isPlayingChanged)

public:
    explicit PlaybackManager(QObject *parent = nullptr);
    ~PlaybackManager();

    QMediaPlayer *player() const;

    double playbackSpeed() const;
    void setPlaybackSpeed(double speed);

    double volume() const;
    void setVolume(double volume);

    qint64 duration() const;
    qint64 position() const;
    void setPosition(qint64 pos);

    bool isPlaying() const;

    Q_INVOKABLE void play(const QString &videoUrl);
    Q_INVOKABLE void pause();
    Q_INVOKABLE void resume();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void seekForward(qint64 seconds);
    Q_INVOKABLE void seekBackward(qint64 seconds);

signals:
    void playbackSpeedChanged();
    void volumeChanged();
    void durationChanged();
    void positionChanged();
    void isPlayingChanged();
    void playbackStarted();
    void playbackPaused();
    void playbackStopped();
    void playbackFinished();
    void playbackError(const QString &errorMessage);
    void progressUpdated(qint64 position, qint64 duration);

private slots:
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);

private:
    QMediaPlayer *m_player;
};

#endif // PLAYBACKMANAGER_H
