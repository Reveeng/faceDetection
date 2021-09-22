#include "capturethread.h"
#include <QGuiApplication>
#include <QDebug>

using namespace dlib;

CaptureThread::CaptureThread(QMutex* lock):
    m_data_lock(lock),m_running(false)
{

}

CaptureThread::~CaptureThread(){}

void CaptureThread::run() {
    m_running = true;
    cv::VideoCapture cap;
    cv::Mat frame;
    frontal_face_detector detector = get_frontal_face_detector();
    cap.open("ksvideosrc do-stats=TRUE ! videoconvert ! queue ! appsink", cv::CAP_GSTREAMER);
    if (!cap.isOpened()){
        qDebug() << "cap isnt open";
    }
    while (m_running){
        cap.read(frame);
//        detectFaces(frame);
//        qDebug() << frame.total();
        cv::Mat small;
        cv::resize(frame, small, {640,480});
        array2d<unsigned char> imgGray;

        cv_image<rgb_pixel> cimg(small);
        assign_image(imgGray,cimg);

        std::vector<rectangle> faces = detector(imgGray);

        for (auto &f : faces){
           qDebug() << "left x" << f.left() << "top y" <<
                       "right x" << f.right() << "bottom y" << f.bottom();
        }
        cvtColor(frame, frame, cv::COLOR_RGB2BGRA);
        m_data_lock->lock();
        m_frame = frame;
        m_data_lock->unlock();
        emit frameCaptured(&frame);
    }
    cap.release();
    m_running = false;
}

void CaptureThread::detectFaces(cv::Mat &frame){
    std::vector<cv::Rect> faces;
    cv::Mat gray_frame;
    cv::cvtColor(frame, gray_frame, cv::COLOR_BGR2GRAY);
//    m_classifier->detectMultiScale(gray_frame, faces, 1.3, 5);

    cv::Scalar color = cv::Scalar(0,0,255);
    for (size_t i = 0; i < faces.size(); i++){
        cv::rectangle(frame, faces[i], color, 1);
    }
}
