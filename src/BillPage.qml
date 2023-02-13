import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/client" as Client

Page {
    id: billPage
    title: "Facturas"

    header: BillPageHeader { clientModel: baseModel }

    StackLayout {
        id: stackLayout
        anchors.fill: parent

        BusyIndicator { id: myBusy }

        ListView {
            id: orderView
            headerPositioning: ListView.OverlayHeader
            model:  baseModel.model
            delegate: BillDelegate {}
            focus: true
        }
    }

    Client.ClientModel {
        id: baseModel
        request.onSucceeded: stackLayout.currentIndex = 1
        onFiltering: stackLayout.currentIndex = 0
        Component.onCompleted: findAll()
    }
}
