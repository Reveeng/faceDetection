QT += quick multimedia
TEMPLATE = app
CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        capturethread.cpp \
        main.cpp \
        videohandler.cpp

RESOURCES += qml.qrc

QMAKE_CXXFLAGS_RELEASE += -mavx

win32: LIBS += -L$$PWD/lib
LIBS+= -llibopencv_core453 -llibopencv_imgproc453 \
        -llibopencv_imgcodecs453 -llibopencv_video453 \
        -llibopencv_videoio453 -llibopencv_objdetect453 \
        -llibopencv_face453

INCLUDEPATH += $$PWD/include

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    capturethread.h \
    videohandler.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -ldlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -ldlib

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
