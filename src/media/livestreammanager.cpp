#include "livestreammanager.h"
#include <QDebug>

// ============================================================
// LiveStreamManager - 直播推拉流管理
// 支持 RTMP 协议的推流与拉流
// ============================================================

LiveStreamManager::LiveStreamManager(QObject *parent)
    : QObject(parent)
{
    m_player = new QMediaPlayer(this);
    m_captureSession = new QMediaCaptureSession(this);
    m_recorder = new QMediaRecorder(this);
    m_audioInput = new QAudioInput(this);

    // 连接信号
    connect(m_player, &QMediaPlayer::errorOccurred,
            this, &LiveStreamManager::onPlayerError);
    connect(m_player, &QMediaPlayer::positionChanged,
            this, &LiveStreamManager::streamProgress);
    connect(m_recorder, &QMediaRecorder::errorOccurred,
            this, &LiveStreamManager::onRecorderError);

    // 初始化捕获会话
    m_captureSession->setAudioInput(m_audioInput);
    m_captureSession->setRecorder(m_recorder);
}

LiveStreamManager::~LiveStreamManager()
{
    stopStream();
    stopPublish();
}

QString LiveStreamManager::streamUrl() const
{
    return m_streamUrl;
}

void LiveStreamManager::setStreamUrl(const QString &url)
{
    if (m_streamUrl != url) {
        m_streamUrl = url;
        emit streamUrlChanged();
    }
}

LiveStreamManager::StreamRole LiveStreamManager::role() const
{
    return m_role;
}

void LiveStreamManager::setRole(StreamRole role)
{
    if (m_role != role) {
        m_role = role;
        emit roleChanged();
    }
}

bool LiveStreamManager::isStreaming() const
{
    return m_isStreaming;
}

QMediaPlayer *LiveStreamManager::player() const
{
    return m_player;
}

QMediaCaptureSession *LiveStreamManager::captureSession() const
{
    return m_captureSession;
}

// 拉流：播放 RTMP 直播流
void LiveStreamManager::startStream(const QString &url)
{
    if (url.isEmpty()) {
        emit streamError("直播地址不能为空");
        return;
    }

    m_streamUrl = url;
    m_role = Player;

    m_player->setSource(QUrl(url));
    m_player->play();

    m_isStreaming = true;
    emit isStreamingChanged();
    emit streamStarted();
    qDebug() << "[LiveStreamManager] Started playing stream:" << url;
}

void LiveStreamManager::stopStream()
{
    if (m_player->playbackState() != QMediaPlayer::StoppedState) {
        m_player->stop();
        m_player->setSource(QUrl());
    }

    m_isStreaming = false;
    emit isStreamingChanged();
    emit streamStopped();
    qDebug() << "[LiveStreamManager] Stream playback stopped";
}

// 推流：使用摄像头/屏幕捕获推送 RTMP 流
void LiveStreamManager::startPublish(const QString &url, QCamera *camera)
{
    if (url.isEmpty()) {
        emit streamError("推流地址不能为空");
        return;
    }

    m_streamUrl = url;
    m_role = Publisher;

    if (camera) {
        m_captureSession->setCamera(camera);
    }

    // 设置 RTMP 输出
    QMediaFormat format;
    format.setVideoCodec(QMediaFormat::VideoCodec::H264);
    format.setAudioCodec(QMediaFormat::AudioCodec::AAC);
    format.setFileFormat(QMediaFormat::FileFormat::MPEG4);
    m_recorder->setMediaFormat(format);
    m_recorder->setOutputLocation(QUrl(url));

    // 编码参数（Qt 6.6+ API）
    m_recorder->setVideoResolution(QSize(1920, 1080));
    m_recorder->setVideoBitRate(2500000);   // 2.5 Mbps
    m_recorder->setAudioBitRate(128000);    // 128 kbps

    m_recorder->record();

    m_isStreaming = true;
    emit isStreamingChanged();
    emit streamStarted();
    qDebug() << "[LiveStreamManager] Started publishing to:" << url;
}

void LiveStreamManager::stopPublish()
{
    if (m_recorder->recorderState() != QMediaRecorder::StoppedState) {
        m_recorder->stop();
    }

    m_isStreaming = false;
    emit isStreamingChanged();
    emit streamStopped();
    qDebug() << "[LiveStreamManager] Publishing stopped";
}

void LiveStreamManager::onPlayerError(QMediaPlayer::Error error)
{
    if (error != QMediaPlayer::NoError) {
        QString errorMsg = m_player->errorString();
        qWarning() << "[LiveStreamManager] Player error:" << error << errorMsg;
        emit streamError(errorMsg);
        m_isStreaming = false;
        emit isStreamingChanged();
    }
}

void LiveStreamManager::onRecorderError(QMediaRecorder::Error error)
{
    if (error != QMediaRecorder::NoError) {
        QString errorMsg = m_recorder->errorString();
        qWarning() << "[LiveStreamManager] Recorder error:" << error << errorMsg;
        emit streamError(errorMsg);
        m_isStreaming = false;
        emit isStreamingChanged();
    }
}
