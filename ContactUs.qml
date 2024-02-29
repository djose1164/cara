import QtQuick
import QtQuick.Controls

Item {
    id: root

    implicitWidth: column.implicitWidth
    implicitHeight: column.implicitHeight

    Column {
        id: column
        spacing: 15

        Label {
            width: root.width
            text: "Contactanos"
            font.pointSize: 12
            font.bold: true
        }

        Button {
            flat: true
            text: "Twitter"
            font.pointSize: 11
            icon.source: "twitter.svg"
            onClicked: url => Qt.openUrlExternally("https://twitter.com/carardoficial")
        }

        Button {
            flat: true
            text: "Correo electrónico"
            font.pointSize: 11
            icon.source: "mail.svg"
            onClicked: url => Qt.openUrlExternally("mailto:djsoe1164@gmail.com")
        }
    }
}
