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

    StackLayout {
        id: stackLayout
        anchors.fill: parent

        BusyIndicator {}

        ListView {
            model: clientModel.model
            delegate: UpdateOrderDelegate {}
        }
    }

    Client.ClientModel {
        id: clientModel
        onCodeChanged: stackLayout.currentIndex = 1

        Component.onCompleted: {
            print("## Adding client...");
            clientModel.addById(clientId)
        }
    }

    property var pageHeader: Utils.PageHeader {
        title: "Actualizar ordenes"
        stackView_: myStack
    }

    Component.onCompleted: {
        print("## client_id: " + clientId);
        stackLayoutPage.header = pageHeader
    }
    Component.onDestruction: stackLayoutPage.header = stackLayoutPage.mainHeader

    function resetView() {
        clientModel.clearModel();
        clientModel.addById(clientId);
    }
}
