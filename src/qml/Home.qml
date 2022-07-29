import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia


Page {
    id: home
    property int counter: 0

    header: Label {
        id: homeLbl
        text: "Inicio"
        font.pointSize: 20
        font.bold: true
    }
    CaptureSession {
            camera: Camera {
                id: camera

                focusMode: Camera.FocusModeAutoNear
                customFocusPoint: Qt.point(0.2, 0.2) // Focus relative to top-left corner
            }
            videoOutput: videoOutput
        }

        VideoOutput {
            id: videoOutput
            anchors.fill: parent
        }


    RowLayout {
        width: parent.width

        Label {
            id: lbl
        }


        Button {
            text: "Click me!"
            onClicked: {
                ++counter
                mainStackView.replace("qrc:/qml/Login.qml")
        }}
    }
    footer: ToolBar {
        //height: parent.height
        //anchors.fill: parent
        Rectangle {
            anchors.fill: parent
            color: "blue"
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
