#ifndef CAPTURETHREAD_H
#define CAPTURETHREAD_H

#include <QObject>
#include <QThread>
#include <QMutex>

#include "opencv2/opencv.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/face/facemark.hpp"


class CaptureThread: public QThread
{
    Q_OBJECT
public:
    explicit CaptureThread(QMutex* lock);
    ~CaptureThread();
    void setRunning(bool run){m_running = run;};

protected:
    void run() override;
signals:
    void frameCaptured(cv::Mat* data);

private:
    void detectFaces(cv::Mat &frame);
    QString m_path;
    QMutex* m_data_lock;
    bool m_running;
    cv::CascadeClassifier* m_classifier;
    cv::Ptr<cv::face::Facemark> mark_detector;
    cv::Mat m_frame;
};

#endif // CAPTURETHREAD_H
