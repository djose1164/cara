import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Popup {
    id: root

    property alias text: label.text

    implicitWidth: rowLayout.implicitWidth * 1.5
    anchors.centerIn: Overlay.overlay

    RowLayout {
        id: rowLayout
        anchors.fill: parent

        BusyIndicator {}

        Label {
            id: label
            text: "Procesando..."
            font.pointSize: 12
        }
    }
}

