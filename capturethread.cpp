#include "capturethread.h"
#include <QGuiApplication>
#include <QDebug>

CaptureThread::CaptureThread(QMutex* lock):
    m_data_lock(lock),m_running(false)
{

}

CaptureThread::~CaptureThread(){}

void CaptureThread::run() {
    m_running = true;
    cv::VideoCapture cap;
    cv::Mat frame;
    QString classif = QGuiApplication::instance()->applicationDirPath() +"\\haarcascade_frontalface_default.xml";
    m_classifier->load(classif.toStdString());
    mark_detector = cv::face::createFacemarkLBF();
    QString model_data = QGuiApplication::instance()->applicationDirPath() + "\\lbfmodel.yaml";
    mark_detector->loadModel(model_data.toStdString());
    cap.open("ksvideosrc do-stats=TRUE ! videoconvert ! autovideosink", cv::CAP_GSTREAMER);
    if (!cap.isOpened()){
        qDebug() << "cap isnt open";
    }
    while (m_running){
        cap.read(frame);
        detectFaces(frame);

        cvtColor(frame, frame, cv::COLOR_BGR2RGB);
        m_data_lock->lock();
        m_frame = frame;
        emit frameCaptured(&frame);
    }
    cap.release();
    delete m_classifier;
    m_classifier = nullptr;
    m_running = false;
}

void CaptureThread::detectFaces(cv::Mat &frame){
    std::vector<cv::Rect> faces;
    cv::Mat gray_frame;
    cv::cvtColor(frame, gray_frame, cv::COLOR_BGR2GRAY);
    m_classifier->detectMultiScale(gray_frame, faces, 1.3, 5);

    cv::Scalar color = cv::Scalar(0,0,255);
    for (size_t i = 0; i < faces.size(); i++){
        cv::rectangle(frame, faces[i], color, 1);
    }
}
