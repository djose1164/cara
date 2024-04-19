import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T
import Cara.Utils

Page {
    id: root

    property alias listView: listView
    property string noDataMessage: "No se ha podido obtener ningun dato."

    states: [
        State {
            name: "fetching"
            PropertyChanges {
                target: listView
                visible: false
            }
            PropertyChanges {
                target: busyIndicator
                running: true
            }
        },
        State {
            name: "contentNotFound"
            PropertyChanges {
                target: errorPane
                visible: true
            }
            PropertyChanges {
                target: busyIndicator
                running: false
            }
        }
    ]

    ListView {
        id: listView
        anchors.fill: parent
    }

    MessagePane {
        id: errorPane
        visible: false
        width: root.width * .8
        height: root.height/2
        anchors.centerIn: parent
        text: noDataMessage
    }

    BusyIndicator {
        id: busyIndicator
        running: false
        anchors.centerIn: parent
    }
}
