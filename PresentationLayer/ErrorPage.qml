import QtQuick
import QtQuick.Controls
import "qrc:/users"

Item {
    required property string message

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
            text: message
            font.pointSize: 14
            wrapMode: Label.WordWrap
            textFormat: CheckLabel.MarkdownText
            onLinkActivated: url => Qt.openUrlExternally(url)
        }
    }
}
