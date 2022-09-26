import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import "qrc:/search" as Search
import "qrc:/utils" as Utils
import "qrc:/client" as Client
import "qrc:/main"

Page {
    id: root
    property int clientId

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
        onStatusReady: status => {
            stackLayout.currentIndex = 1;
            if (!status)
                messageDialog.open();
        }
        Component.onCompleted: clientModel.addById(clientId)
    }

    MessageDialog {
        id: messageDialog
        buttons: MessageDialog.Ok
        text: "Ningun cliente con el ID suministrado ha sido encontrado."
        informativeText: "Verifique que el ID ingresado sea correcto."

        onButtonClicked: close()
        onAccepted: close()
    }

    property var pageHeader: Utils.PageHeader {
        title: "Actualizar ordenes"
        stackView: billStackView
    }

    Component.onCompleted: stackLayoutPage.header = pageHeader
    Component.onDestruction: stackLayoutPage.header = stackLayoutPage.mainHeader
}
