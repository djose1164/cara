import QtQuick
import QtQuick.Controls
import "qrc:/users"

Page {
    required property string message

    Label {
        id: label
        width: parent.width * .8
        anchors.centerIn: parent
        text: message
        font.pointSize: 14
        wrapMode: Label.WordWrap
        textFormat: CheckLabel.MarkdownText
        onLinkActivated: url => Qt.openUrlExternally(url)
    }
}
