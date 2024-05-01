import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cara.Shopping as Catalogue
import Cara.Utils
import Cara.Product


Popup {
    id: root

    property alias state: pane.state

    anchors.centerIn: Overlay.overlay

    Pane {
        id: pane
        anchors.fill: parent
        states: [
            State {
                name: "busy"
                PropertyChanges {
                    target: stackLayout
                    currentIndex: 0
                }
            },
            State {
                name: "success"
                PropertyChanges {
                    target: stackLayout
                    currentIndex: 1
                }
                PropertyChanges {
                    target: label
                    text: "Operacion realizada con exito!"
                }
            },
            State {
                name: "error"
                PropertyChanges {
                    target: stackLayout
                    currentIndex: 1
                }
                PropertyChanges {
                    target: label
                    text: "No se pudo completar la operacion."
                }
            }
        ]

        StackLayout {
            id: stackLayout
            anchors.centerIn: parent

            BusyIndicator {}

            Label { id: label }
        }
    }
}
