import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: billPage
    title: "Facturas"
    header: BillPageHeader {}
    property var myBusy
    ListView {
        id: orderView
        anchors.fill: parent
        headerPositioning: ListView.OverlayHeader
        header: Component {
            Rectangle {
                width: parent.width
                height: 30
                BusyIndicator {
                    id: busyIndicator
                    anchors.fill: parent
                    running: false
                    Component.onCompleted: myBusy = this
                }

                Label {
                    anchors.centerIn: parent
                    text: "Todas las facturas"
                    font.bold: true
                    font.pointSize: 16
                    visible: !busyIndicator.running
                }
            }
        }
        model:  billModel
        delegate: BillDelegate {}
        focus: true
    }

    ListModel {
        id: billModel
    }

    WorkerScript {
        id: billWorker
        source: "qrc:/js/client_request.mjs"

        onMessage: () => myBusy.running = false;
    }

    Component.onCompleted: {
        myBusy.running = true
        billWorker.sendMessage({"type": "findAll", "model": billModel})
    }
}
