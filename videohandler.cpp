#include "videohandler.h"

VideoHandler::VideoHandler(QObject* parent): QObject(parent),
    m_videoSurface(nullptr)
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
    QSize size(currentFrame.cols,currentFrame.rows);
    int bytes = size.width()*size.height()*3;
    QVideoFrame frame(bytes,size,
                      currentFrame.step,
                      QVideoFrame::Format_RGB24);
    frame.map(QAbstractVideoBuffer::ReadWrite);
    memcpy(frame.bits(), currentFrame.data, bytes);
    frame.unmap();
    m_videoSurface->present(frame);
}

QAbstractVideoSurface * VideoHandler::videoSurface() const{
    return m_videoSurface;
}

void VideoHandler::setVideoSurface(QAbstractVideoSurface* surface){
    m_videoSurface = surface;
    QVideoSurfaceFormat format = QVideoSurfaceFormat(QSize(640,480), QVideoFrame::Format_RGB24);
    m_videoSurface->start(format);
}

void VideoHandler::start(){
    capturer->start();
}

void VideoHandler::stop(){
    capturer->setRunning(false);
}
