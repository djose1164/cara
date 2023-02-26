import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/customers" as Customers
import "qrc:/users"
import "qrc:/utils"

Page {
    id: root

    readonly property StackView stackView: StackView.view

    title: "Facturas"
    header: BillPageHeader { onOpenFilter: filter.open() }

    Connections {
        target: TheCurrentUser
        function onUserTypeIdChanged() {
            print("TheCurrentUser.isAdmin: " + TheCurrentUser.isAdmin)
            if (TheCurrentUser.isAdmin)
                baseModel.findAll();
            else if (TheCurrentUser.isCostumer)
                baseModel.setById(TheCurrentUser.userId);
        }
    }

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
            model:  baseModel.model
            delegate: BillDelegate { stackView: root.stackView }
        }
    }

    Customers.CustomerModel {
        id: baseModel
        request.onSucceeded: stackLayout.currentIndex = 2
        onFiltering: stackLayout.currentIndex = 0
        request.onFailed: r => {
            print("request.onFailed: " + r);
        }
    }

    Filter {
        id: filter
        customerModel: baseModel
        onFiltered: data => orderView.model = data
    }
}
