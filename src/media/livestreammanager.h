#ifndef LIVESTREAMMANAGER_H
#define LIVESTREAMMANAGER_H

#include <QObject>
#include <QMediaPlayer>
#include <QMediaCaptureSession>
#include <QMediaRecorder>
#include <QVideoSink>
#include <QAudioInput>
#include <QCamera>
#include <QUrl>

class LiveStreamManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString streamUrl READ streamUrl WRITE setStreamUrl NOTIFY streamUrlChanged)
    Q_PROPERTY(StreamRole role READ role WRITE setRole NOTIFY roleChanged)
    Q_PROPERTY(bool isStreaming READ isStreaming NOTIFY isStreamingChanged)
    Q_PROPERTY(QMediaPlayer *player READ player CONSTANT)
    Q_PROPERTY(QMediaCaptureSession *captureSession READ captureSession CONSTANT)

public:
    enum StreamRole { Player, Publisher };
    Q_ENUM(StreamRole)

    explicit LiveStreamManager(QObject *parent = nullptr);
    ~LiveStreamManager();

    QString streamUrl() const;
    void setStreamUrl(const QString &url);

    StreamRole role() const;
    void setRole(StreamRole role);

    bool isStreaming() const;

    QMediaPlayer *player() const;
    QMediaCaptureSession *captureSession() const;

    Q_INVOKABLE void startStream(const QString &url);
    Q_INVOKABLE void stopStream();
    Q_INVOKABLE void startPublish(const QString &url, QCamera *camera = nullptr);
    Q_INVOKABLE void stopPublish();

signals:
    void streamUrlChanged();
    void roleChanged();
    void isStreamingChanged();
    void streamStarted();
    void streamStopped();
    void streamError(const QString &errorMessage);
    void streamProgress(qint64 position);

private slots:
    void onPlayerError(QMediaPlayer::Error error);
    void onRecorderError(QMediaRecorder::Error error);

private:
    QString m_streamUrl;
    StreamRole m_role = Player;
    bool m_isStreaming = false;

    // 拉流（观众端）
    QMediaPlayer *m_player = nullptr;
    QVideoSink *m_videoSink = nullptr;

    // 推流（主播端）
    QMediaCaptureSession *m_captureSession = nullptr;
    QMediaRecorder *m_recorder = nullptr;
    QAudioInput *m_audioInput = nullptr;
};

#endif // LIVESTREAMMANAGER_H
