#ifndef VIDEOHANDLER_H
#define VIDEOHANDLER_H

#include <QObject>
#include <QMutex>
#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>
#include "opencv2/opencv.hpp"
#include "capturethread.h"

class VideoHandler: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QAbstractVideoSurface * videoSurface READ videoSurface WRITE setVideoSurface)
public:
    explicit VideoHandler(QObject *parent = nullptr);
    ~VideoHandler();
    QAbstractVideoSurface * videoSurface() const;

public slots:
    void setVideoSurface(QAbstractVideoSurface * videoSurface);
    void start();
    void stop();

private slots:
    void updateFrame(cv::Mat* data);

private:
    QAbstractVideoSurface * m_videoSurface;
    CaptureThread* capturer;
    QMutex* data_lock;
    cv::Mat currentFrame;
};

#endif // VIDEOHANDLER_H
