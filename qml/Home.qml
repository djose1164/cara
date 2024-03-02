import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T
import Utils

Page {
    id: root
    readonly property T.StackView stackView: StackView.view

    header: HomeHeader { stackView: root.stackView }
    Component.onCompleted: {
        internetStatus.checkInternetConnection();
    }

    Rectangle {
        id: noInternetRec
        anchors.fill: parent
        z: 100
        color: "floralwhite"
        visible: {
            if (internetStatus.status === internetStatus.InternetNotAvailable)
                return true;
            else
                return false;
        }

        Label {
            anchors.centerIn: parent
            text: "Conexion a internet no disponible."
            font.pointSize: 14
        }
    }

    InternetStatus {
        id: internetStatus
    }
}
