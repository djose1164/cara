import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T
import QtNetwork

Page {
    id: root
    readonly property T.StackView stackView: StackView.view

    header: HomeHeader { stackView: root.stackView }

    Rectangle {
        id: noInternetRec
        anchors.fill: parent
        z: 100
        color: "floralwhite"
        visible: NetworkInformation.reachability !== NetworkInformation.Reachability.Online

        Label {
            anchors.centerIn: parent
            text: "Conexión a internet no disponible."
            font.pointSize: 14
        }
    }
}
