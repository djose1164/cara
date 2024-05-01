import QtQuick
import QtQuick.Controls

Page {
    id: root

    required property string message

    Label {
        id: label
        width: root.width * .9
        anchors.centerIn: parent
        text: message
        font.pointSize: 14
        wrapMode: Label.WordWrap
        textFormat: Label.RichText
        onLinkActivated: url => Qt.openUrlExternally(url)
    }
}
