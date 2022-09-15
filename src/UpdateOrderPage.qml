import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import "qrc:/search" as Search
import "qrc:/utils" as Utils
import "qrc:/client" as Client

Page {
    id: root
    header: Utils.PageHeader { title: "Actualizar Ordenes" }

    StackLayout {
        id: stackLayout
        anchors.fill: parent
        currentIndex: 1

        BusyIndicator {}

        Search.SearchBar {
            id: searchBar
            Layout.topMargin: 20
            placeholderText: "ID de la orden"
            onFind: handleSearch()
        }

        ListView {
            model: clientModel.model
            delegate: UpdateOrderDelegate {}
        }
    }

    Client.ClientModel {
        id: clientModel
        onStatusReady: status => {
            if (!status)
                messageDialog.open();
        }
    }

    MessageDialog {
        id: messageDialog
        buttons: MessageDialog.Ok
        text: "Ningun cliente con el ID suministrado ha sido encontrado."
        informativeText: "Verifique que el ID ingresado sea correcto."

        onButtonClicked: close()
        onAccepted: close()
    }

    function handleSearch() {
        print("toFind: " + searchBar.toFind);
        stackLayout.currentIndex = 0;
        clientModel.addById(searchBar.toFind);
        stackLayout.currentIndex = 2;
    }
}
