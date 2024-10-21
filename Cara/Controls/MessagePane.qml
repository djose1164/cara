import QtQuick
import QtQuick.Controls

Item {
    id: root

    property int elevation: 6
    property int type: MessagePane.Warning
    property alias sourceSize: image.sourceSize
    property alias text: label.text

    enum Type {
        Warning,
        Error
    }

    implicitWidth: pane.implicitWidth
    implicitHeight: pane.implicitHeight

    Pane {
        id: pane
        anchors.fill: parent
        Material.elevation: root.elevation

        Column {
            anchors.fill: parent
            spacing: 14

            Image {
                id: image
                anchors.horizontalCenter: parent.horizontalCenter
                source: {
                    switch(root.type) {
                    case MessagePane.Warning:
                        return "qrc:/icons/warning.svg";
                    case MessagePane.Error:
                        return "qrc:/icons/error.svg";
                    default:
                        return "qrc:/icons/warning.svg";
                    }
                }

                sourceSize: "96x96"
            }

            Label {
                id: label
                width: root.width * .8
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Label.Wrap
                font.pointSize: 14
                font.bold: true
            }
        }
    }
}
