import QtQuick
import QtQuick.Controls

Item {
    property alias text: label.text
    required property string message
    required property int code

    anchors.fill: parent

    Rectangle {
        width: parent.width * .9
        height: parent.height/2
        anchors.centerIn: parent
        border.width: 1
        radius: 10
        color: "floralwhite"

        Label {
            id: label
            width: parent.width * .9
            anchors.centerIn: parent
            anchors.margins: 40
            text: `Error code ${code}: ${message}`
            font.pointSize: 14
            wrapMode: Label.WordWrap
        }
    }
}
