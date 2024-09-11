import QtQuick
import QtQuick.Controls

Rectangle {
    property alias title: titleLbl.text

    width: parent.width
    height: 40
    z: 100
    anchors {
        top: root.top
    }

    color: "floralwhite"

    BackButton {
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        onClicked: stackView.pop()
    }

    Label {
        id: titleLbl
        anchors.centerIn: parent
        text: "Proveedores"
        font.pointSize: 16
    }
}
