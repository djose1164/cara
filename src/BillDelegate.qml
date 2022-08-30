import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/main"

ItemDelegate {
    width: orderView.width
    height: orderView.height * .15
    property int clientID

    ColumnLayout {
        anchors.fill: parent
        Label {
            //Layout.fillWidth: true
            //Layout.leftMargin: 5
            Layout.topMargin: 15
            text: name
            font.pointSize: 18
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignTop
            Layout.leftMargin: 15

            Label {
                text: "Orden:"
                font.bold: true
                font.pointSize: 16
            }

            Repeater {
                model: orders

                Label {
                    id: leftLbl
                    text: "ID: " + id
                    font.pointSize: 14
                    Component.onCompleted: clientID = id
                }
            }
        }
    }

    MessagePopup {
        id: popup
        width: parent.width * .8
        height: width
        text: "Has hecho click en el clientID " + clientID
    }

    WorkerScript {
        id: billWorker
        source: "qrc:/js/client_request.mjs"

        onMessage: () => myBusy.running = false;
    }

    onClicked: popup.open()

}
