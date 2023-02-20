import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/client" as Client
import "qrc:/users"
Page {
    id: billPage

    readonly property StackView stackView: StackView.view

    title: "Facturas"
    header: BillPageHeader { clientModel: baseModel }

    StackLayout {
        id: stackLayout
        anchors.fill: parent
        currentIndex: TheCurrentUser.isCostumer ? 0 : 1

        BusyIndicator { id: myBusy }

        Item {
            Label {
                width: parent.width * .8
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: height/2
            wrapMode: Label.WordWrap
            text: "No has realizado ninguna compra. Chequea nuestro catalogo"
            font.pointSize: 15
            horizontalAlignment: Label.AlignHCenter
        }
        }
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
        request.onSucceeded: stackLayout.currentIndex = 2
        onFiltering: stackLayout.currentIndex = 0
        request.onFailed: r => {
            print("request.onFailed: " + r);
        }

        Component.onCompleted:
            if (TheCurrentUser.isAdmin)
                findAll();
            else if (TheCurrentUser.isCostumer)
                setById(TheCurrentUser.userId);
    }
}
