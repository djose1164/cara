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

    Component.onCompleted: clientModel.setById(clientId)

    StackLayout {
        id: stackLayout
        anchors.fill: parent

        BusyIndicator {}

        ListView {
            model: clientModel.model
            delegate: UpdateOrderDelegate {
                onDataChanged: {
                    print("UpdateOrderPage: UpdateOrderDelegate: onDataChanged");
                    clientModel.setById(clientId);
                }
            }
        }
    }

    Client.ClientModel {
        id: clientModel
        request.onReceived: stackLayout.currentIndex = 1
    }
}
