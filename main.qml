import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.12
import QtMultimedia 5.0
import VideoHandler 0.1

Window {
    width: 1000
    height: 1000
    visible: true
    title: qsTr("Hello World")

    VideoHandler{
        id:vd

    }

    VideoOutput{
        source:vd
        width:640
        height:480
        anchors.left:parent.left
        anchors.top:parent.top
    }

    Button{
        width:100
        height:20
        text:"Пуск"
        anchors.right:parent.right
        anchors.top:parent.top
        onClicked: {
            vd.start()
        }
    }
}
