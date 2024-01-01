import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T

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
                visible: true
            }
        },
        State {
            name: "errorOccurred"
            PropertyChanges {
                target: errorPane
                visible: true
            }
        }
    ]

    ListView {
        id: listView
        anchors.fill: parent

        Label {
            width: listView.width * .8
            visible: listView.count === 0
            text: noDataMessage
            font.pointSize: 14
            wrapMode: Label.Wrap
        }
    }

    Pane {
        id: errorPane
        width: root.width * .8
        height: root.height/2
        anchors.centerIn: parent
        visible: false
        Material.elevation: 6

        Column {
            anchors.fill: parent
            spacing: 14
            IconImage {
                source: "qrc:/icons/warning.svg"
            }
            Label {
                width: errorPane.width * .8
                text: "Error al intentar obtener los datos."
                font.pointSize: 14
                font.bold: true
            }
        }
    }

    BusyIndicator {
        id: busyIndicator
        visible: false
        anchors.centerIn: parent
    }
}
