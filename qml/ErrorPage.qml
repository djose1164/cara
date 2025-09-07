import QtQuick
import QtQuick.Controls

Page {
    id: root

    required property string message

    Label {
        id: label
        width: root.width * .7
        height: root.height / 2
        anchors.centerIn: parent
        text: root.message
        font.pointSize: 14
        wrapMode: Label.WordWrap
        textFormat: Label.RichText
        onLinkActivated: url => Qt.openUrlExternally(url)
    }
}
