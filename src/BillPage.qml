import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: billPage
    title: "Facturas"
    header: BillPageHeader {}

    ListView {
        id: orderView
        anchors.fill: parent
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
    }

    Component.onCompleted: billWorker.sendMessage({"type": "findAll", "model": billModel})
}
