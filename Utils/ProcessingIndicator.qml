import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Popup {
    id: root

    property alias text: label.text

    anchors.centerIn: Overlay.overlay

    RowLayout {
        anchors.fill: parent

        BusyIndicator {
            running: root.visible
        }

        Label {
            id: label
            text: "Procesando..."
            font.pointSize: 12
        }
    }
}

