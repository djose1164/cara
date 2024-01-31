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
                anchors.horizontalCenter: parent.horizontalCenter
                source: "qrc:/icons/warning.svg"
                sourceSize: "96x96"
            }
            Label {
                width: errorPane.width * .8
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Label.Wrap
                text: noDataMessage
                font.pointSize: 14
                font.bold: true
            }
        }
    }

    BusyIndicator {
        id: busyIndicator
        running: false
        anchors.centerIn: parent
    }
}
