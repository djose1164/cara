import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import "qrc:/search" as Search
import "qrc:/utils" as Utils
import "qrc:/client" as Client
import "qrc:/main"

// TODO: Update the view after effecting a payment.

Page {
    id: root

    property int clientId
    readonly property StackView myStack: StackView.view

    header: Utils.PageHeader { title: "Actualizar ordenes"; stackView: myStack }

    StackLayout {
        id: stackLayout
        anchors.fill: parent

        BusyIndicator {}

        ListView {
            model: clientModel.model
            delegate: UpdateOrderDelegate { onDataHasChanged: clientModel.addById(clientId) }
        }
    }

    Client.ClientModel {
        id: clientModel
        request.onMessageReceived: stackLayout.currentIndex = 1

        Component.onCompleted: addById(clientId)
    }

    function resetView() {
        clientModel.clearModel();
        clientModel.addById(clientId);
    }
}
