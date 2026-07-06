#ifndef STREAMRECORDER_H
#define STREAMRECORDER_H

#include <QObject>
#include <QMediaRecorder>
#include <QMediaCaptureSession>
#include <QMediaFormat>
#include <QMediaPlayer>
#include <QSize>
#include <QUrl>

class StreamRecorder : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isRecording READ isRecording NOTIFY isRecordingChanged)
    Q_PROPERTY(QString outputPath READ outputPath WRITE setOutputPath NOTIFY outputPathChanged)
    Q_PROPERTY(qint64 recordingDuration READ recordingDuration NOTIFY recordingDurationChanged)

public:
    explicit StreamRecorder(QObject *parent = nullptr);
    ~StreamRecorder();

    bool isRecording() const;
    QString outputPath() const;
    void setOutputPath(const QString &path);
    qint64 recordingDuration() const;

    Q_INVOKABLE void startRecording(QMediaCaptureSession *captureSession);
    Q_INVOKABLE void stopRecording();
    Q_INVOKABLE void startRecordingWithPlayer(QMediaPlayer *player);

signals:
    void isRecordingChanged();
    void outputPathChanged();
    void recordingDurationChanged();
    void recordingStarted();
    void recordingStopped(const QString &filePath);
    void recordingError(const QString &errorMessage);

private slots:
    void onRecorderError(QMediaRecorder::Error error, const QString &errorString);

private:
    QMediaRecorder *m_recorder = nullptr;
    QMediaCaptureSession *m_captureSession = nullptr;
    QString m_outputPath;
    bool m_isRecording = false;
};

#endif // STREAMRECORDER_H
