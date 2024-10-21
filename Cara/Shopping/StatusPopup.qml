import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


Popup {
    id: root

    property alias state: pane.state
    readonly property alias label: label
    property alias text: textLabel.text

    anchors.centerIn: Overlay.overlay

    Pane {
        id: pane
        anchors.fill: parent
        states: [
            State {
                name: "busy"
                PropertyChanges {
                    stackLayout.currentIndex: 0
                }
            },
            State {
                name: "success"
                PropertyChanges {
                    stackLayout.currentIndex: 1
                }
                PropertyChanges {
                    label.text: "Operacion realizada con exito!"
                }
            },
            State {
                name: "error"
                PropertyChanges {
                    stackLayout.currentIndex: 1
                }
                PropertyChanges {
                    label.text: "No se pudo completar la operacion."
                }
            }
        ]

        StackLayout {
            id: stackLayout
            anchors.fill: parent

            BusyIndicator {}

            ColumnLayout {
                spacing: 10
                Label {
                    id: label
                    wrapMode: Label.Wrap
                    font.bold: true
                    font.pointSize: 12
                    Layout.fillWidth: true
                }

                Label {
                    id: textLabel
                    wrapMode: Label.Wrap
                    Layout.fillWidth: true
                }
            }
        }
    }
}
