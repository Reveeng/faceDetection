#include "videohandler.h"


#include "QDebug"


VideoHandler::VideoHandler(QObject* parent): QObject(parent),
    m_videoSurface(0)
{
    data_lock = new QMutex();
    capturer = new CaptureThread(data_lock);
    connect(capturer, &CaptureThread::frameCaptured, this, &VideoHandler::updateFrame);
}

VideoHandler::~VideoHandler(){

}

void VideoHandler::updateFrame(cv::Mat* mat){
    data_lock->lock();
    currentFrame = *mat;
    data_lock->unlock();
//    qDebug() << currentFrame.total()*currentFrame.elemSize();
    QSize size(currentFrame.cols,currentFrame.rows);
    int bytes = currentFrame.total()*currentFrame.elemSize();

    QVideoFrame frame(bytes,size,currentFrame.step,QVideoFrame::Format_BGRA32);
    frame.map(QAbstractVideoBuffer::ReadWrite);
    memcpy(frame.bits(), currentFrame.data, bytes);
    frame.unmap();

//    qDebug() << frame.size();
    if (!m_videoSurface->present(frame))
        qDebug() << "frame not presented";
}

QAbstractVideoSurface * VideoHandler::videoSurface() const{
    return m_videoSurface;
}

void VideoHandler::setVideoSurface(QAbstractVideoSurface* surface){
    if (m_videoSurface){
        qDebug() << "?";
        m_videoSurface->stop();
    }
    m_videoSurface = surface;
    format = QVideoSurfaceFormat(QSize(1280,720), QVideoFrame::Format_BGRA32);
//    QVideoSurfaceFormat new_format = m_videoSurface->nearestFormat(format);
    qDebug() << m_videoSurface->start(format) /*<< m_videoSurface->supportedPixelFormats()*/;
//    qDebug() << "surface shoul start";
}

void VideoHandler::start(){
    capturer->start();
}

void VideoHandler::stop(){
    capturer->setRunning(false);
}
