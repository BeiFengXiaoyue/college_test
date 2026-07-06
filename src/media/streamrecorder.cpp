#include "streamrecorder.h"
#include <QDateTime>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

StreamRecorder::StreamRecorder(QObject *parent)
    : QObject(parent)
{
    m_recorder = new QMediaRecorder(this);

    connect(m_recorder, &QMediaRecorder::errorOccurred,
            this, &StreamRecorder::onRecorderError);
    connect(m_recorder, &QMediaRecorder::recorderStateChanged,
            this, [this]() {
        bool recording = m_recorder->recorderState() == QMediaRecorder::RecordingState;
        if (m_isRecording != recording) {
            m_isRecording = recording;
            emit isRecordingChanged();
        }
    });
    connect(m_recorder, &QMediaRecorder::durationChanged,
            this, &StreamRecorder::recordingDurationChanged);

    // 默认输出路径
    QString defaultDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                         + "/EduPlatform/Recordings";
    QDir().mkpath(defaultDir);
    m_outputPath = defaultDir;
}

StreamRecorder::~StreamRecorder()
{
    if (m_isRecording) {
        stopRecording();
    }
}

bool StreamRecorder::isRecording() const
{
    return m_isRecording;
}

QString StreamRecorder::outputPath() const
{
    return m_outputPath;
}

void StreamRecorder::setOutputPath(const QString &path)
{
    if (m_outputPath != path) {
        m_outputPath = path;
        QDir().mkpath(path);
        emit outputPathChanged();
    }
}

qint64 StreamRecorder::recordingDuration() const
{
    return m_recorder->duration();
}

void StreamRecorder::startRecording(QMediaCaptureSession *captureSession)
{
    if (!captureSession) {
        emit recordingError("捕获会话不能为空");
        return;
    }

    m_captureSession = captureSession;

    // 生成文件名
    QString fileName = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss") + ".mp4";
    QString fullPath = m_outputPath + "/" + fileName;

    QMediaFormat format;
    format.setVideoCodec(QMediaFormat::VideoCodec::H264);
    format.setAudioCodec(QMediaFormat::AudioCodec::AAC);
    format.setFileFormat(QMediaFormat::FileFormat::MPEG4);
    m_recorder->setMediaFormat(format);

    m_recorder->setVideoResolution(QSize(1920, 1080));
    m_recorder->setVideoBitRate(2500000);

    m_recorder->setOutputLocation(QUrl::fromLocalFile(fullPath));
    m_recorder->record();

    m_isRecording = true;
    emit isRecordingChanged();
    emit recordingStarted();
    qDebug() << "[StreamRecorder] Recording started:" << fullPath;
}

void StreamRecorder::stopRecording()
{
    if (m_recorder->recorderState() != QMediaRecorder::StoppedState) {
        QString filePath = m_recorder->outputLocation().toLocalFile();
        m_recorder->stop();
        m_isRecording = false;
        emit isRecordingChanged();
        emit recordingStopped(filePath);
        qDebug() << "[StreamRecorder] Recording stopped:" << filePath;
    }
}

void StreamRecorder::startRecordingWithPlayer(QMediaPlayer *player)
{
    Q_UNUSED(player)
    // 录制播放中的视频流（需要操作系统级音频输出捕获，或屏幕录制权限）
    emit recordingError("播放器录制需要系统级权限，请使用 startRecording 方法");
}

void StreamRecorder::onRecorderError(QMediaRecorder::Error error, const QString &errorString)
{
    Q_UNUSED(error)
    qWarning() << "[StreamRecorder] Error:" << errorString;
    emit recordingError(errorString);
}
