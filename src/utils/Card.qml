import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    property int elevation: 8
    property alias source: image.source
    property alias text: textLbl.text

    signal clicked

    Frame {
        id: frame
        width: root.width
        height: root.height
        //border.width: 1

        RowLayout {
            anchors.fill: parent
            anchors.margins: 5
            spacing: 20

            Image {
                id: image
                sourceSize: "32x32"
                Layout.alignment: Qt.AlignCenter
            }

            Label {
                id: textLbl
                wrapMode: Label.Wrap
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
            }
        }

        TapHandler {
            onTapped: clicked()
        }
    }
}
