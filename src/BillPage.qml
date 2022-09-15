import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils" as Utils

Page {
    id: billPage
    title: "Facturas"

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
                    text: "Clientes con facturas las facturas"
                    font.bold: true
                    font.pointSize: 16
                    visible: !busyIndicator.running
                }
            }
        }
        model:  baseModel.model
        delegate: BillDelegate {}
        focus: true
    }

    Utils.BaseModel {
        id: baseModel
        scriptSource: "qrc:/js/client_request.mjs"

        Component.onCompleted: send({"type": "findAll"})
    }
}
