import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/client" as Client

Page {
    id: billPage
    title: "Facturas"

    property BusyIndicator myBusy
    readonly property var pageHeader: BillPageHeader { model: baseModel }

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
                    running: true
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
        model:  baseModel.model
        delegate: BillDelegate {}
        focus: true
    }

    Client.ClientModel {
        id: baseModel
        onCodeChanged: myBusy.running = false

        Component.onCompleted: findAll()
    }

}
