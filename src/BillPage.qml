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
    header: BillPageHeader {
        stackView: root.stackView
        onOpenFilter: filter.open()
        customerModel: baseModel
    }

    Connections {
        target: TheCurrentUser

        function onUserTypeIdChanged() {
            print("TheCurrentUser.isCustomer: " + TheCurrentUser.isCostumer
                  + "\ncustomerId " + TheCurrentUser.customerId)
            updateCustomerModel();
        }

        function onOrderPosted() {
            baseModel.findOrdersByCustomerId(TheCurrentUser.customerId);
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
            readonly property StackView stackView: root.stackView
            model:  baseModel.model
            delegate: TheCurrentUser.isCostumer ? _.customerBill : _.billDelegate
        }
    }

    Customers.CustomerModel {
        id: baseModel
        request.onSucceeded: stackLayout.currentIndex = 2
        onFiltering: stackLayout.currentIndex = 0
        request.onFailed: r => print("request.onFailed: " + r);
    }

    Filter {
        id: filter
        customerModel: baseModel
        onFiltered: data => orderView.model = data
    }

    QtObject {
        id: _
        property Component billDelegate: Component{
            BillDelegate {
                Component.onCompleted: updateCustomer.connect(baseModel.updateCustomer)
            }
        }
        property Component customerBill: Component {
            CustomerBill {
                Component.onCompleted: updateCustomer.connect(baseModel.updateCustomer)
            }
        }
    }

    function updateCustomerModel() {
        if (TheCurrentUser.isAdmin)
            baseModel.findAll();
        else if (TheCurrentUser.isCostumer)
            baseModel.findOrdersByCustomerId(TheCurrentUser.customerId);
    }
}
